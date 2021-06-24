//
// Created by 陈宇卿 on 2021/6/24.
//
#include "BMP.hpp"
#include <vector>
#include <set>

void histogram_equalization_single_channel(BMP_PIXEL_24* bmp, int channel_index) {
    int non_zero_prob_cnts = 0;
    double max_prob = 0;
    double min_prob = 1;
    double ave_prob = 1.0 / 256;
    int prob_lager_ave = 0;
    double sum_non_zero = 0;
    double ave_non_zero_prob;
    double var_prob = 0;

    int pixel_cnts = bmp->width * bmp->height;
    std::vector<int> times(256, 0);
    std::vector<double> freq(256, 0);
    std::vector<double> cbf(256, 0);
    std::vector<int> level_map(256, 0);

    for (int i = 0; i < pixel_cnts; i++) {
        int channel = 0;
        if (channel_index == 0) channel = bmp->pixels[i].R;
        else if (channel_index == 1) channel = bmp->pixels[i].G;
        else if (channel_index == 2) channel = bmp->pixels[i].B;
        // 统计频数
        times[channel]++;
        freq[channel] = (double)times[channel] / pixel_cnts;
        auto p = freq[channel];
        if (p > max_prob) max_prob = p;
        if (p < min_prob) min_prob = p;
    }

    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            non_zero_prob_cnts++;
            sum_non_zero += freq[i];
        }
        if (freq[i] > ave_prob) prob_lager_ave++;

        if (i == 0)
        {
            cbf[0] = freq[0];
        }
        else
        {
            cbf[i] = cbf[i - 1] + freq[i];
        }
        level_map[i] = floor(cbf[i] * 255.0);
        std::cout << "Y before: " << i << "\t" << "Y after: " << level_map[i] << std::endl;
    }

    ave_non_zero_prob = sum_non_zero / non_zero_prob_cnts;
    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            var_prob += pow(freq[i] - ave_non_zero_prob, 2);
        }
    }
    var_prob /= non_zero_prob_cnts;

    for (int i = 0; i < pixel_cnts; i++) {
        int channel = 0;
        if (channel_index == 0) channel = bmp->pixels[i].R;
        else if (channel_index == 1) channel = bmp->pixels[i].G;
        else if (channel_index == 2) channel = bmp->pixels[i].B;
        auto r = level_map[channel];
        if (channel_index == 0) bmp->pixels[i].R = r;
        else if (channel_index == 1) bmp->pixels[i].G = r;
        else if (channel_index == 2) bmp->pixels[i].B = r;
    }

    std::cout << "概率非零灰度数：" << non_zero_prob_cnts << std::endl;
    std::cout << "概率非零最大值：" << max_prob << std::endl;
    std::cout << "概率非零最小值：" << min_prob << std::endl;
    std::cout << "最大最小概率之比：" << max_prob / min_prob << std::endl;
    std::cout << "图像中概率大于平均概率的灰度级数：" << prob_lager_ave << std::endl;
    std::cout << "非零概率的平均值：" << ave_non_zero_prob << std::endl;
    std::cout << "非零概率的方差：" << var_prob << std::endl;
}

void histogram_equalization_grey(BMP_PIXEL_24* bmp) {
    int non_zero_prob_cnts = 0;
    double max_prob = 0;
    double min_prob = 1;
    double ave_prob = 1.0 / 256;
    int prob_lager_ave = 0;
    double sum_non_zero = 0;
    double ave_non_zero_prob;
    double var_prob = 0;

    int pixel_cnts = bmp->width * bmp->height;
    std::vector<int> times(256, 0);
    std::vector<double> freq(256, 0);
    std::vector<double> cbf(256, 0);
    std::vector<int> level_map(256, 0);
    for (int i = 0; i < pixel_cnts; i++) {
        // 统计频数
        times[bmp->pixels[i].R]++;
        freq[bmp->pixels[i].R] = (double)times[bmp->pixels[i].R] / pixel_cnts;
        auto p = freq[bmp->pixels[i].R];
        if (p > max_prob) max_prob = p;
        if (p < min_prob) min_prob = p;
    }

    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            non_zero_prob_cnts++;
            sum_non_zero += freq[i];
        }
        if (freq[i] > ave_prob) prob_lager_ave++;

        if (i == 0)
        {
            cbf[0] = freq[0];
        }
        else
        {
            cbf[i] = cbf[i - 1] + freq[i];
        }
        level_map[i] = floor(cbf[i] * 255.0);
        std::cout << "Y before: " << i << "\t" << "Y after: " << level_map[i] << std::endl;
    }

    ave_non_zero_prob = sum_non_zero / non_zero_prob_cnts;
    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            var_prob += pow(freq[i] - ave_non_zero_prob, 2);
        }
    }
    var_prob /= non_zero_prob_cnts;

    for (int i = 0; i < pixel_cnts; i++) {
        auto r = level_map[bmp->pixels[i].R];
        bmp->pixels[i].R = r;
        bmp->pixels[i].G = r;
        bmp->pixels[i].B = r;
    }

    std::cout << "概率非零灰度数：" << non_zero_prob_cnts << std::endl;
    std::cout << "概率非零最大值：" << max_prob << std::endl;
    std::cout << "概率非零最小值：" << min_prob << std::endl;
    std::cout << "最大最小概率之比：" << max_prob / min_prob << std::endl;
    std::cout << "图像中概率大于平均概率的灰度级数：" << prob_lager_ave << std::endl;
    std::cout << "非零概率的平均值：" << ave_non_zero_prob << std::endl;
    std::cout << "非零概率的方差：" << var_prob << std::endl;
}

void histogram_equalization(BMP_PIXEL_24* bmp) {

    int non_zero_prob_cnts = 0;
    double max_prob = 0;
    double min_prob = 1;
    double ave_prob = 1.0 / 256;
    int prob_lager_ave = 0;
    double sum_non_zero = 0;
    double ave_non_zero_prob;
    double var_prob = 0;

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
        auto p = freq_Y[yuv[i].Y];
        if (p > max_prob) max_prob = p;
        if (p < min_prob) min_prob = p;
    }


    for (int i = 0; i < 256; i++) {
        if (freq_Y[i] != 0) {
            non_zero_prob_cnts++;
            sum_non_zero += freq_Y[i];
        }
        if (freq_Y[i] > ave_prob) prob_lager_ave++;

        if (i == 0)
        {
            cbf[0] = freq_Y[0];
        }
        else
        {
            cbf[i] = cbf[i - 1] + freq_Y[i];
        }
        y_map[i] = floor(cbf[i] * 255.0);
        std::cout << "Y before: " << i << "\t" << "Y after: " << y_map[i] << std::endl;
    }

    ave_non_zero_prob = sum_non_zero / non_zero_prob_cnts;
    for (int i = 0; i < 256; i++) {
        if (freq_Y[i] != 0) {
            var_prob += pow(freq_Y[i] - ave_non_zero_prob, 2);
        }
    }
    var_prob /= non_zero_prob_cnts;

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

    std::cout << "概率非零灰度数：" << non_zero_prob_cnts << std::endl;
    std::cout << "概率非零最大值：" << max_prob << std::endl;
    std::cout << "概率非零最小值：" << min_prob << std::endl;
    std::cout << "最大最小概率之比：" << max_prob / min_prob << std::endl;
    std::cout << "图像中概率大于平均概率的灰度级数：" << prob_lager_ave << std::endl;
    std::cout << "非零概率的平均值：" << ave_non_zero_prob << std::endl;
    std::cout << "非零概率的方差：" << var_prob << std::endl;
}