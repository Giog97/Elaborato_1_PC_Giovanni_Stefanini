//
// Created by giost on 14/01/2025.
//

#include "MeanShiftParallel.h"
#include <omp.h>
#include <cmath>
#include "Utils.h"

// NB struct SoAData; la seguente struttura dati è una Structure of Arrays (SoA)

// Converte i dati dell'immagine da AoS (Array of Structures) a SoA
SoAData convertToSoA(const cv::Mat& image) {
    SoAData data;
    data.r.reserve(image.rows * image.cols);
    data.g.reserve(image.rows * image.cols);
    data.b.reserve(image.rows * image.cols);

    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b color = image.at<cv::Vec3b>(y, x);
            data.r.push_back(static_cast<float>(color[0]));
            data.g.push_back(static_cast<float>(color[1]));
            data.b.push_back(static_cast<float>(color[2]));
        }
    }
    return data;
}



// Implementazione parallela di Mean Shift (lavora con SoA)
void meanShift_parallel(const SoAData& data, SoAData& modes, float bandwidth, float epsilon) {
    modes = data; // Copia iniziale dei dati
    const float bandwidthSquared = bandwidth * bandwidth;

    // Parallelizza il ciclo principale
    #pragma omp parallel for
    for (size_t i = 0; i < modes.r.size(); ++i) {
        float pointR = modes.r[i], pointG = modes.g[i], pointB = modes.b[i];
        float shiftR, shiftG, shiftB;

        do {
            shiftR = 0.0f; shiftG = 0.0f; shiftB = 0.0f;
            float totalWeight = 0.0f;

            // Ciclo interno per calcolare il contributo di ogni punto
            for (size_t j = 0; j < data.r.size(); ++j) {
                float distSquared =
                    (pointR - data.r[j]) * (pointR - data.r[j]) +
                    (pointG - data.g[j]) * (pointG - data.g[j]) +
                    (pointB - data.b[j]) * (pointB - data.b[j]);

                if (distSquared < bandwidthSquared) {
                    float weight = exp(-distSquared / (2 * bandwidthSquared));
                    shiftR += weight * data.r[j];
                    shiftG += weight * data.g[j];
                    shiftB += weight * data.b[j];
                    totalWeight += weight;
                }
            }

            if (totalWeight > 0) {
                shiftR /= totalWeight;
                shiftG /= totalWeight;
                shiftB /= totalWeight;
            }

            float shiftDistance =
                (shiftR - pointR) * (shiftR - pointR) +
                (shiftG - pointG) * (shiftG - pointG) +
                (shiftB - pointB) * (shiftB - pointB);
            pointR = shiftR;
            pointG = shiftG;
            pointB = shiftB;

            if (shiftDistance < epsilon * epsilon) break;
        } while (true);

        modes.r[i] = pointR;
        modes.g[i] = pointG;
        modes.b[i] = pointB;
    }
}

// Ricostruisce l'immagine, dopo aver applicato il Mean Shift parallelo
cv::Mat reconstructFromSoA(const SoAData& modes, int rows, int cols) {
    cv::Mat result = cv::Mat::zeros(rows, cols, CV_8UC3);

    for (size_t i = 0; i < modes.r.size(); ++i) {
        int x = static_cast<int>(i % cols);
        int y = static_cast<int>(i / cols);

        cv::Vec3b color = cv::Vec3b(
            static_cast<uchar>(modes.r[i]),
            static_cast<uchar>(modes.g[i]),
            static_cast<uchar>(modes.b[i])
        );
        result.at<cv::Vec3b>(y, x) = color;
    }
    return result;
}

// Segmentazione immagine parallela (usa SoA per la segmentazione parallela)
cv::Mat segmentImage_parallel(const cv::Mat& image, float bandwidth, float epsilon) {
    SoAData data = convertToSoA(image);
    SoAData modes;

    // Applica Mean Shift parallelo
    meanShift_parallel(data, modes, bandwidth, epsilon);

    // Ricostruisci immagine da SoA
    return reconstructFromSoA(modes, image.rows, image.cols);
}
