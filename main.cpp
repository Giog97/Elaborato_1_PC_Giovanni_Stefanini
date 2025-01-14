#include "MeanShiftSeq.h"
#include "MeanShiftParallel.h"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono> // Necessario per misurare il tempo
#include <omp.h> // Necessario per parallelizzare (OpenMP)

namespace fs = std::filesystem;

int main() {
    // Imposta i parametri per Mean Shift
    float bandwidth = 32.0f; // Ampiezza della finestra di ricerca
    float epsilon = 1.0f;    // Tolleranza per la convergenza

    int resize_factor = 4; // indica di che fattore scaleremo le immagini. 1:512x512, 2: 256x256, 4:128x128, 8:64x64 //ottimale è 4
    std::string imageDirectory = "./img"; // Specifica la directory delle immagini
    std::string outputDirectory = "./img_results"; // Directory per le immagini segmentate

    int maxThreads = omp_get_max_threads(); // Numero massimo di thread disponibili (uso OpenMP)
    std::cout << "Thread disponibili: " << maxThreads << std::endl;

    //ToDo aggiungere la scelta del numero di Threads da usare
    omp_set_num_threads(maxThreads); // Usa tutti i threads disponibili (set del numero di threads disponibili)

    // Verifica che la directory esista
    if (!fs::exists(imageDirectory) || !fs::is_directory(imageDirectory)) {
        std::cerr << "Errore: La directory " << imageDirectory << " non esiste o non è una directory!" << std::endl;
        return -1;
    }

    // Scansiona la directory per trovare immagini
    std::vector<std::string> imageFiles;
    for (const auto& entry : fs::directory_iterator(imageDirectory)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".jpg" || ext == ".png" || ext == ".bmp") {
                imageFiles.push_back(entry.path().string());
            }
        }
    }

    // Verifica se ci sono immagini disponibili
    if (imageFiles.empty()) {
        std::cerr << "Errore: Nessuna immagine trovata nella directory " << imageDirectory << "!" << std::endl;
        return -1;
    }

    // Mostra la lista di immagini all'utente
    std::cout << "Immagini disponibili nella directory " << imageDirectory << ":\n";
    for (size_t i = 0; i < imageFiles.size(); ++i) {
        std::cout << i + 1 << ": " << imageFiles[i] << std::endl;
    }

    // Chiedi all'utente di selezionare un'immagine
    int choice = 0;
    do {
        std::cout << "Scegli un'immagine (1-" << imageFiles.size() << "): ";
        std::cin >> choice;
    } while (choice < 1 || choice > static_cast<int>(imageFiles.size()));

    std::string imagePath = imageFiles[choice - 1];
    std::cout << "Hai scelto: " << imagePath << std::endl;

    // Carica l'immagine
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Errore: Impossibile caricare l'immagine!" << std::endl;
        return -1;
    }

    // Riduci la risoluzione dell'immagine (ad esempio, a metà dimensione)
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(image.cols / resize_factor, image.rows / resize_factor)); // Riduce l'immagine 512x512:Z dove Z è il fattore di ridimensionamento

    // Esegui Mean Shift sequenziale
    auto start_seq = std::chrono::high_resolution_clock::now(); // inizio timer sequenziale
    cv::Mat segmented_seq = segmentImage_seq(resizedImage, bandwidth, epsilon);     // esecuzione segmentazione con MeanShift sequenziale
    auto end_seq = std::chrono::high_resolution_clock::now();   // fine timer sequenziale
    std::chrono::duration<double> elapsed_seq = end_seq - start_seq;                // calcolo tempo sequenziale
    std::cout << "Tempo di esecuzione (sequenziale): " << elapsed_seq.count() << " secondi." << std::endl;

    // Esegui Mean Shift parallelo
    auto start_par = std::chrono::high_resolution_clock::now();  // inizio timer parallelo
    cv::Mat segmented_par = segmentImage_parallel(resizedImage, bandwidth, epsilon); // esecuzione segmentazione con MeanShift parallelo
    auto end_par = std::chrono::high_resolution_clock::now();    // fine timer parallelo
    std::chrono::duration<double> elapsed_par = end_par - start_par;                 // calcolo tempo parallelo
    std::cout << "Tempo di esecuzione (parallelo): " << elapsed_par.count() << " secondi." << std::endl;
    //ToDO aggiungere il numero di threads usati come output


    // Mostra il risultato
    //cv::imshow("Original Image", resizedImage);  // Mostra l'immagine ridimensionata
    //cv::imshow("Segmented Image", segmented); // Mostra immagine segmentata
    //cv::waitKey(0);

    // Crea il nome del file di output
    std::string outputFileName_segmented_seq = outputDirectory + "/sequential_segmented_image_" + std::to_string(choice) + ".jpg"; // prende la directory (in modo che stia nella cartella img_results + il 'segemnted' + il numero della scelta)
    std::string outputFileName_segmented_par = outputDirectory + "/parallel_segmented_image_" + std::to_string(choice) + ".jpg";
    std::string outputFileName_resized = outputDirectory + "/resized_image_" + std::to_string(choice) + ".jpg";

    // Salva immagini risultanti nella cartella img_results
    cv::imwrite(outputFileName_segmented_seq, segmented_seq); // Salva l'immagine segmentata seq nella directory e con il nome desiderato
    cv::imwrite(outputFileName_segmented_par, segmented_seq); // Salva l'immagine segmentata par nella directory e con il nome desiderato
    cv::imwrite(outputFileName_resized, resizedImage); // Salva l'immagine ridimensionata

    return 0;
}



