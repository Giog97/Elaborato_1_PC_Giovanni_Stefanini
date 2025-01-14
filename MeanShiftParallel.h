//
// Created by giost on 14/01/2025.
//

#ifndef MEANSHIFTPARALLEL_H
#define MEANSHIFTPARALLEL_H

#include <opencv2/opencv.hpp>
#include <vector>

// Funzione per calcolare la distanza euclidea al quadrato sui canali RGB
float squaredDistance(const cv::Vec3f& p1, const cv::Vec3f& p2);

// NB la seguente struttura dati è una Structure of Arrays (SoA)
struct SoAData {
    std::vector<float> r, g, b; // Componenti dei colori
};

// Funzione per convertire l'immagine da Array of Structures (AoS) a Structure of Arrays (SoA)
SoAData convertToSoA(const cv::Mat& image);

// Funzione per eseguire il Mean Shift parallelo su dati in formato SoA
void meanShift_parallel(const SoAData& data, SoAData& modes, float bandwidth, float epsilon);

// Funzione per ricostruire un'immagine OpenCV (cv::Mat) da dati in formato SoA
cv::Mat reconstructFromSoA(const SoAData& modes, int rows, int cols);

// Funzione per segmentare un'immagine in parallelo utilizzando SoA
cv::Mat segmentImage_parallel(const cv::Mat& image, float bandwidth, float epsilon);

#endif //MEANSHIFTPARALLEL_H


