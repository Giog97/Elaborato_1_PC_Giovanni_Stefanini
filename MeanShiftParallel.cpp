//
// Created by gioste.
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
    modes.r.resize(data.r.size()); // Assicura che modes abbia la stessa dimensione di data
    modes.g.resize(data.g.size());
    modes.b.resize(data.b.size());

    // First Touch: inizializza modes per ogni thread con barriere implicite (copia manuale in un ciclo parallelo per rispettare il First Touch)
    #pragma omp parallel for // Distribuisce i dati tra le cache locali dei thread senza attendere una sincronizzazione alla fine.
    for (size_t i = 0; i < data.r.size(); ++i) {
        modes.r[i] = data.r[i];
        modes.g[i] = data.g[i];
        modes.b[i] = data.b[i];
    }

    const float bandwidthSquared = bandwidth * bandwidth;

    // Parallelizza il ciclo principale
    #pragma omp parallel // Inizia una regione parallela
    {
        #pragma omp for schedule(dynamic, 64) nowait // divide i cicli per i threads del team, ovvero parallelizza con OpenMP, leva barriere implicite (nowait), Usa blocchi di 64 elementi per distribuire il carico in modo più equo tra i thread
        for (size_t i = 0; i < modes.r.size(); ++i) {
            float pointR = modes.r[i], pointG = modes.g[i], pointB = modes.b[i];

            do {
                // Variabili locali per ridurre la sincronizzazione
                float localShiftR = 0.0f, localShiftG = 0.0f, localShiftB = 0.0f, localTotalWeight = 0.0f;

                // Ciclo interno per calcolare il contributo di ogni punto (provato a parallelizzare e vettorizzazione esplicita, ma rallentano le prestazioni)
                // Non è parallelizzabile
                for (size_t j = 0; j < data.r.size(); ++j) {
                    float distSquared =
                        (pointR - data.r[j]) * (pointR - data.r[j]) +
                        (pointG - data.g[j]) * (pointG - data.g[j]) +
                        (pointB - data.b[j]) * (pointB - data.b[j]);

                    if (distSquared < bandwidthSquared) {
                        float weight = exp(-distSquared / (2 * bandwidthSquared));
                        localShiftR += weight * data.r[j];
                        localShiftG += weight * data.g[j];
                        localShiftB += weight * data.b[j];
                        localTotalWeight += weight;
                    }
                }

                // Calcola i nuovi punti
                if (localTotalWeight > 0) {
                    localShiftR /= localTotalWeight;
                    localShiftG /= localTotalWeight;
                    localShiftB /= localTotalWeight;
                }

                // Calcola la distanza di spostamento
                float shiftDistance =
                    (localShiftR - pointR) * (localShiftR - pointR) +
                    (localShiftG - pointG) * (localShiftG - pointG) +
                    (localShiftB - pointB) * (localShiftB - pointB);

                pointR = localShiftR;
                pointG = localShiftG;
                pointB = localShiftB;

                if (shiftDistance < epsilon * epsilon) break;
            } while (true);

            // Salva il risultato
            modes.r[i] = pointR;
            modes.g[i] = pointG;
            modes.b[i] = pointB;
        }
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
