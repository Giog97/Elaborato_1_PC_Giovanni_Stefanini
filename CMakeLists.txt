cmake_minimum_required(VERSION 3.29)
project(Elaborato_1_Giovanni_Stefanini)

set(CMAKE_CXX_STANDARD 20)

# Percorso di installazione di OpenCV
set(OpenCV_DIR "C:/opencv-build/install") # Percorso di OpenCV
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") # Serve per usare OpenMP

# Trova OpenCV
find_package(OpenCV REQUIRED)

# Aggiungi l'eseguibile
add_executable(Elaborato_1_Giovanni_Stefanini main.cpp
        MeanShiftParallel.cpp
        MeanShiftParallel.h
        MeanShiftSeq.cpp
        MeanShiftSeq.h
        Utils.cpp
        Utils.h)

# Aggiungi le directory di include e librerie di OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Elaborato_1_Giovanni_Stefanini ${OpenCV_LIBS})
