cmake_minimum_required(VERSION 3.16)
project(Digital_Image_Processing)

set(CMAKE_CXX_STANDARD 14)

add_executable(Digital_Image_Processing headers/BMP.hpp headers/Color_Quantization.hpp headers/Pixel.hpp EXP1/exp1.cpp headers/Histogram_Equalization.hpp)