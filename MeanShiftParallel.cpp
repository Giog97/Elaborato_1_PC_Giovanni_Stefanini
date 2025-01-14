//
// Created by giost on 14/01/2025.
//

#include "MeanShiftParallel.h"
#include <omp.h>
#include <cmath>
#include "Utils.h"

// Implementazione parallela di Mean Shift
void meanShift_parallel(const std::vector<cv::Vec3f>& data, std::vector<cv::Vec3f>& modes, float bandwidth, float epsilon) {
    modes = data; // Copia dei dati originali
    const float bandwidthSquared = bandwidth * bandwidth;

    // Parallelizza il ciclo principale su tutti i punti
    #pragma omp parallel for
    for (size_t i = 0; i < modes.size(); ++i) {
        cv::Vec3f point = modes[i];
        cv::Vec3f shift;

        do {
            shift = cv::Vec3f(0, 0, 0); // Inizializza shift
            float totalWeight = 0.0f;

            // Ciclo interno non è parallelizzabile (dipendenze con shift)
            for (const auto& otherPoint : data) {
                float distSquared = squaredDistance(point, otherPoint);
                if (distSquared < bandwidthSquared) {
                    float weight = exp(-distSquared / (2 * bandwidthSquared));
                    shift += weight * otherPoint;
                    totalWeight += weight;
                }
            }

            if (totalWeight > 0) {
                shift /= totalWeight;
            }

            float shiftDistance = std::sqrt(squaredDistance(point, shift));
            point = shift;

            if (shiftDistance < epsilon) break;
        } while (true);

        modes[i] = point;
    }
}

// Segmentazione immagine parallela
cv::Mat segmentImage_parallel(const cv::Mat& image, float bandwidth, float epsilon) {
    // Convertiamo l'immagine in uno spazio (x, y, r, g, b)
    std::vector<cv::Vec3f> data;
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b color = image.at<cv::Vec3b>(y, x);
            data.push_back(cv::Vec3f(color[0], color[1], color[2])); // Usa solo i canali RGB
        }
    }

    // Applichiamo il Mean Shift parallelo
    std::vector<cv::Vec3f> modes;
    meanShift_parallel(data, modes, bandwidth, epsilon);

    // Riorganizziamo i risultati in una nuova immagine
    cv::Mat result = cv::Mat::zeros(image.size(), image.type());
    for (size_t i = 0; i < data.size(); ++i) {
        int x = static_cast<int>(i % image.cols); // Calcola la posizione x
        int y = static_cast<int>(i / image.cols); // Calcola la posizione y
        cv::Vec3b color = cv::Vec3b(
            static_cast<uchar>(modes[i][0]),
            static_cast<uchar>(modes[i][1]),
            static_cast<uchar>(modes[i][2])
        );
        result.at<cv::Vec3b>(y, x) = color;
    }

    return result;
}
