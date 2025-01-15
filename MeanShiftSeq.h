//
// Created by giost on 14/01/2025.
//

#ifndef MEANSHIFTSEQ_H
#define MEANSHIFTSEQ_H

#include <opencv2/opencv.hpp>
#include <vector>

// NB std::vector<Vec3f> data; è un Array di strutture (AoS) fornito da openCV

// Implementazione del Mean Shift Clustering Sequenziale
void meanShift_seq(const std::vector<cv::Vec3f>& data, std::vector<cv::Vec3f>& modes, float bandwidth, float epsilon);

// Segmentazione immagine con Mean Shift Sequenziale
cv::Mat segmentImage_seq(const cv::Mat& image, float bandwidth, float epsilon);

#endif //MEANSHIFTSEQ_H
