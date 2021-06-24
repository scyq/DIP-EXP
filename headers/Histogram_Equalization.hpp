//
// Created by 陈宇卿 on 2021/6/24.
//
#include "BMP.hpp"
#include <vector>
#include <set>

void histogram_equalization(BMP_PIXEL_24* bmp) {
    int pixel_cnts = bmp->width * bmp->height;
    auto yuv = bmp->yuv_pixels;
    std::vector<int> times_Y(256, 0);
    std::vector<double> freq_Y(256, 0);
    std::vector<double> cbf(256, 0);
    std::vector<int> y_map(256, 0);

    for (int i = 0; i < pixel_cnts; i++) {
        // 统计频数
        times_Y[yuv[i].Y]++;
        freq_Y[yuv[i].Y] = (double)times_Y[yuv[i].Y] / pixel_cnts;
    }

    for (int i = 0; i < 256; i++) {
        if (i == 0)
        {
            cbf[0] = freq_Y[0];
        }
        else
        {
            cbf[i] = cbf[i - 1] + freq_Y[i];
        }
        y_map[i] = floor(cbf[i] * 255.0);
    }


    for (int i = 0; i < pixel_cnts; i++) {
        bmp->yuv_pixels[i].Y = y_map[yuv[i].Y];
        auto y = bmp->yuv_pixels[i].Y;
        auto u = bmp->yuv_pixels[i].U;
        auto v = bmp->yuv_pixels[i].V;
        int r = round((y + 1.403 * v));
        if (r < 0) r = 0;
        if (r > 255) r = 255;
        int g = round((y - 0.344 * u - 0.714 * v));
        if (g < 0) g = 0;
        if (g > 255) g = 255;
        int b = round((y + 1.770 * u));
        if (b < 0) b = 0;
        if (g > 255) g = 255;
        bmp->pixels[i].R = r;
        bmp->pixels[i].G = g;
        bmp->pixels[i].B = b;
    }

}