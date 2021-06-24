#pragma once

#include "BMP.hpp"


class Pixel_24{
public:
    unsigned char B, G, R;
};

class Pixel_32{
public:
    unsigned char B, G, R, A;
};

class Pixel_YUV{
public:
    int Y,U,V;
};

template<typename T>
void rgb2yuv(const T& rgb, Pixel_YUV& yuv) {
    yuv.Y = round(0.299 * rgb.R + 0.587 * rgb.G + 0.114 * rgb.B);
    yuv.U = round(0.565 * (rgb.B - yuv.Y));
    yuv.V = round(0.713 * (rgb.R - yuv.Y));
}

void print_pixel(Pixel_24 p) {
    std::cout << "R:" << (int) p.R << ' '
            << "G:" << (int) p.G << ' '
            << "B:" << (int) p.B << std::endl;
}

void print_pixel(Pixel_32 p) {
    std::cout << "R:" << (int) p.R << ' '
              << "G:" << (int) p.G << ' '
              << "B:" << (int) p.B << ' '
              << "A:" << (int) p.A << std::endl;
}

void print_pixel(Pixel_YUV p) {
    std::cout << "Y:" << p.Y << ' '
              << "U:" << p.U << ' '
              << "V:" << p.V << std::endl;
}