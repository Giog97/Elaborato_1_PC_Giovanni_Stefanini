//
// Created by giost on 14/01/2025.
//

#ifndef MEANSHIFTSEQ_H
#define MEANSHIFTSEQ_H

#include <opencv2/opencv.hpp>
#include <vector>

// Funzione per calcolare la distanza euclidea al quadrato sui canali RGB
float squaredDistance(const cv::Vec3f& p1, const cv::Vec3f& p2);

// Implementazione del Mean Shift Clustering Sequenziale
void meanShift_seq(const std::vector<cv::Vec3f>& data, std::vector<cv::Vec3f>& modes, float bandwidth, float epsilon);

// Segmentazione immagine con Mean Shift Sequenziale
cv::Mat segmentImage_seq(const cv::Mat& image, float bandwidth, float epsilon);

#endif //MEANSHIFTSEQ_H
