//
// Created by giost on 14/01/2025.
//

#include "MeanShiftSeq.h"
#include <cmath>
#include "Utils.h"

// NB std::vector<Vec3f> data; è un Array di strutture (AoS) fornito da openCV

// Implementazione del Mean Shift Clustering Sequenziale
void meanShift_seq(const std::vector<cv::Vec3f>& data, std::vector<cv::Vec3f>& modes, float bandwidth, float epsilon) {
    modes = data;
    const float bandwidthSquared = bandwidth * bandwidth;

    for (size_t i = 0; i < modes.size(); ++i) {
        cv::Vec3f point = modes[i];
        cv::Vec3f shift;
        do {
            shift = cv::Vec3f(0, 0, 0); // Inizializza shift
            float totalWeight = 0.0f;

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

// Segmentazione immagine con Mean Shift Sequenziale
cv::Mat segmentImage_seq(const cv::Mat& image, float bandwidth, float epsilon) {
    // Convertiamo l'immagine in uno spazio (x, y, r, g, b)
    std::vector<cv::Vec3f> data;
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec3b color = image.at<cv::Vec3b>(y, x);
            data.push_back(cv::Vec3f(color[0], color[1], color[2])); // Usa solo i canali RGB
        }
    }

    // Applichiamo il Mean Shift sequenziale
    std::vector<cv::Vec3f> modes;
    meanShift_seq(data, modes, bandwidth, epsilon);

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
