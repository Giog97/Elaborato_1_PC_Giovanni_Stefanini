//
// Created by giost on 14/01/2025.
//

#ifndef MEANSHIFTPARALLEL_H
#define MEANSHIFTPARALLEL_H

#include <opencv2/opencv.hpp>
#include <vector>

// Funzione per calcolare la distanza euclidea al quadrato sui canali RGB
float squaredDistance(const cv::Vec3f& p1, const cv::Vec3f& p2);

// Implementazione parallela di Mean Shift
void meanShift_parallel(const std::vector<cv::Vec3f>& data, std::vector<cv::Vec3f>& modes, float bandwidth, float epsilon);

// Segmentazione immagine parallela
cv::Mat segmentImage_parallel(const cv::Mat& image, float bandwidth, float epsilon);

#endif //MEANSHIFTPARALLEL_H
