#pragma once

#include <cstdio>
#include <iostream>
#include <vector>
#include "Color_Quantization.hpp"
#include "Pixel.hpp"

typedef struct BMP_FILE_HEADER {
    // 这里理应有一个 file_type 但是由于长度是2，所以结构体的size是不正确的
    unsigned int bfSize;    // 文件大小
    unsigned short bfReserved1; // 一般为0
    unsigned short bfReserved2; // 一般为0
    unsigned int bfOffBits;   // 从文件头到像素数据的偏移，也就是这两
} BMP_FILE_HEADER;

typedef struct BMP_INFO_HEADER {
    unsigned int biSize;    // 此结构体的大小
    int biWidth;        // 图像的宽
    int biHeight;        // 图像的高
    unsigned short biPlanes;  // 1
    unsigned short biBitCount; // 一像素所占的位数，一般为24
    unsigned int biCompression; // 0
    unsigned int biSizeImage;  // 像素数据所占大小, 这个值应该等于上面文件头结构中bfSize-bfOffBits
    int biXPelsPerMeter;    // 0
    int biYPelsPerMeter;    // 0
    unsigned int biClrUsed;   // 图像使用的索引数 0代表全部
    unsigned int biClrImportant; // 重要的索引数目 0代表全部
} BMP_INFO_HEADER;

class Point{
public:
    int x, y;
};

Pixel_24 get_random_color_24() {
    srand((unsigned) time(nullptr));
    /* return [a, b] */
    int a = 0;
    int b = 255;

    unsigned char r = (rand() % (b - a + 1)) + a;
    unsigned char g = (rand() % (b - a + 1)) + a;
    unsigned char _b = (rand() % (b - a + 1)) + a;
    Pixel_24 temp = {_b, g, r};

    return temp;
}

Pixel_32 get_random_color_32() {
    srand((unsigned) time(nullptr));
    /* return [a, b] */
    int a = 0;
    int b = 255;

    unsigned char r = (rand() % (b - a + 1)) + a;
    unsigned char g = (rand() % (b - a + 1)) + a;
    unsigned char _b = (rand() % (b - a + 1)) + a;
    Pixel_32 temp = {_b, g, r, 0};

    return temp;
}


class BMP {
protected:
    FILE *fp;

public:
    BMP();
    explicit BMP(const char *file_name);
    ~BMP();
    int32_t width;
    int32_t height;
    int bmp_bits;
    unsigned short file_type;   // 0x4d42，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778
    BMP_FILE_HEADER file_header;
    BMP_INFO_HEADER info_header;
    virtual void translate_image(int delta_x, int delta_y)=0;
    virtual void derive_bmp(const char *target_file_name) = 0;
    virtual void set_new_size(int new_width, int new_height)=0;
    int coordinate2index(int c_x, int c_y) const;
};

// 存像素值的BMP
class BMP_PIXEL_24 : public BMP {
public:
    ~BMP_PIXEL_24();
    explicit BMP_PIXEL_24(const char *file_name);
    Pixel_24 *pixels;
    Pixel_YUV *yuv_pixels;
    void recolor_rec(Pixel_24 target_pixel, Point left_bottom, Point right_top);
    void derive_bmp(const char *target_file_name) override;
    Pixel_24 get_color(int x, int y);
    void translate_image(int delta_x, int delta_y) override;
    void translate_image(int delta_x, int delta_y, Pixel_24 new_bg_color);
    void set_new_size(int new_width, int new_height) override;
    void set_new_size(int new_width, int new_height, Pixel_24 new_bg_color);
};

// 存索引值的BMP
class BMP_INDEX : public BMP {
public:
    BMP_INDEX(BMP_PIXEL_24 *img_24, Pixel_32 *palette, unsigned char *pidx);
    ~BMP_INDEX();
    explicit BMP_INDEX(const char *file_name);
    int color_counts;
    unsigned char *palette_index;
    Pixel_32 *palette;
    Pixel_YUV *yuv_palette;
    void recolor_palette(Pixel_32 target_color, int target_pixel_index) const;
    void derive_bmp(const char *target_file_name) override;
    Pixel_32 get_color(int x, int y);
    void translate_image(int delta_x, int delta_y) override;
    void translate_image(int delta_x, int delta_y, unsigned char new_bg_color);
    void set_new_size(int new_width, int new_height) override;
    void set_new_size(int new_width, int new_height, unsigned char new_bg_color);
};


BMP::BMP(const char *file_name) {
    fp = fopen(file_name, "rb");
    // 读头
    fseek(fp, 0, SEEK_SET);
    fread(&file_type,1,sizeof (unsigned short), fp);
    fread(&file_header, sizeof(file_header), 1, fp);
    fread(&info_header, sizeof(info_header), 1, fp);
    width = abs(info_header.biWidth);
    height = abs(info_header.biHeight);
    bmp_bits = info_header.biBitCount;
}

BMP_PIXEL_24::BMP_PIXEL_24(const char *file_name) : BMP(file_name) {
    pixels = new Pixel_24[width * height];
    yuv_pixels = new Pixel_YUV[width * height];
    fread(pixels, 1, width * height * sizeof(Pixel_24), fp);
    fclose(fp);
    for (int i = 0; i < width * height; i++) {
        rgb2yuv(pixels[i], yuv_pixels[i]);
    }
}

BMP_INDEX::BMP_INDEX(const char *file_name) : BMP(file_name) {
    color_counts = pow(2, bmp_bits);
    palette = new Pixel_32[color_counts];
    yuv_palette = new Pixel_YUV[color_counts];
    palette_index = new unsigned char[width * height];
    fread(palette, 1, color_counts * sizeof(Pixel_32), fp);
    fread(palette_index, 1, width * height * sizeof(int), fp);
    fclose(fp);
    for (int i = 0; i < color_counts; i++) {
        rgb2yuv(palette[i], yuv_palette[i]);
    }
}

int BMP::coordinate2index(int c_x, int c_y) const {
    return c_x + c_y * width;
};

Pixel_32 BMP_INDEX::get_color(int x, int y) {
    return palette[palette_index[coordinate2index(x, y)]];
}

Pixel_24 BMP_PIXEL_24::get_color(int x, int y) {
    return pixels[coordinate2index(x, y)];
}

void BMP_PIXEL_24::recolor_rec(Pixel_24 target_pixel, Point left_bottom, Point right_top) {
    for (int i = left_bottom.x; i <= right_top.x; i++) {
        for (int j = left_bottom.y; j <= right_top.y; j++) {
            pixels[coordinate2index(i, j)] = target_pixel;
        }
    }
}

BMP::~BMP() {
    delete (fp);
}

BMP::BMP() {}

void BMP_INDEX::set_new_size(int new_width, int new_height) {
    if (new_height <= height || new_width <= width) {
        std::cout << "暂不支持变小" << std::endl;
        return;
    }
    info_header.biWidth = new_width;
    info_header.biHeight = new_height;
    auto _w = width;
    auto _h = height;
    width = new_width;
    height = new_height;
    auto *new_index = new unsigned char[new_height * new_width]{};
    for (int i = 0; i < _w; i++) {
        for (int j = 0; j < _h; j++) {
            int idx = i + j * _w;
            int new_idx = i + j * new_width;
            new_index[new_idx] = palette_index[idx];
        }
    }
    auto to_delete = palette_index;
    palette_index = new_index;
    delete [] to_delete;
}

void BMP_INDEX::set_new_size(int new_width, int new_height, unsigned char new_bg_color) {
    if (new_height <= height || new_width <= width) {
        std::cout << "暂不支持变小" << std::endl;
        return;
    }
    info_header.biWidth = new_width;
    info_header.biHeight = new_height;
    auto _w = width;
    auto _h = height;
    width = new_width;
    height = new_height;
    auto *new_index = new unsigned char[new_height * new_width]{};
    for (int i = 0; i < width * height; i++) {
        new_index[i] = new_bg_color;
    }
    for (int i = 0; i < _w; i++) {
        for (int j = 0; j < _h; j++) {
            int idx = i + j * _w;
            int new_idx = i + j * new_width;
            new_index[new_idx] = palette_index[idx];
        }
    }
    auto to_delete = palette_index;
    palette_index = new_index;
    delete [] to_delete;
}

void BMP_PIXEL_24::set_new_size(int new_width, int new_height) {
    if (new_height <= height || new_width <= width) {
        std::cout << "暂不支持变小" << std::endl;
        return;
    }
    info_header.biWidth = new_width;
    info_header.biHeight = new_height;
    auto _w = width;
    auto _h = height;
    width = new_width;
    height = new_height;
    auto *new_pixels = new Pixel_24[new_height * new_width]{};
    for (int i = 0; i < _w; i++) {
        for (int j = 0; j < _h; j++) {
            int idx = i + j * _w;
            int new_idx = i + j * new_width;
            new_pixels[new_idx] = pixels[idx];
        }
    }
    auto to_delete = pixels;
    pixels = new_pixels;
    delete [] to_delete;
}

void BMP_PIXEL_24::set_new_size(int new_width, int new_height, Pixel_24 new_bg_color) {
    if (new_height <= height || new_width <= width) {
        std::cout << "暂不支持变小" << std::endl;
        return;
    }
    info_header.biWidth = new_width;
    info_header.biHeight = new_height;
    auto _w = width;
    auto _h = height;
    width = new_width;
    height = new_height;
    auto *new_pixels = new Pixel_24[new_height * new_width]{};
    for (int i = 0; i < width * height; i++) {
        new_pixels[i] = new_bg_color;
    }
    for (int i = 0; i < _w; i++) {
        for (int j = 0; j < _h; j++) {
            int idx = i + j * _w;
            int new_idx = i + j * new_width;
            new_pixels[new_idx] = pixels[idx];
        }
    }
    auto to_delete = pixels;
    pixels = new_pixels;
    delete [] to_delete;
}

void BMP_INDEX::derive_bmp(const char *target_file_name) {
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(&file_type, sizeof(file_type), 1, fout);
    fwrite(&file_header, sizeof(file_header), 1, fout);
    fwrite(&info_header, sizeof(info_header), 1, fout);
    fwrite(palette, 1, pow(2, bmp_bits) * sizeof(Pixel_32), fout);
    fwrite(palette_index, 1, width * height * sizeof(int), fout);
    fclose(fout);
}

void BMP_PIXEL_24::derive_bmp(const char *target_file_name) {
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(&file_type, sizeof(file_type), 1, fout);
    fwrite(&file_header, sizeof(file_header), 1, fout);
    fwrite(&info_header, sizeof(info_header), 1, fout);
    fwrite(pixels, 1, height * width * sizeof(Pixel_24), fout);
    fclose(fout);
}

BMP_INDEX::~BMP_INDEX() {
    delete [] palette_index;
    delete [] palette;
    delete [] yuv_palette;
}

void BMP_INDEX::recolor_palette(Pixel_32 target_color, int target_pixel_index) const {
    palette[target_pixel_index] = target_color;
}

BMP_PIXEL_24::~BMP_PIXEL_24() {
    delete [] pixels;
    delete [] yuv_pixels;
}

void BMP_PIXEL_24::translate_image(int delta_x, int delta_y) {
    auto *new_pixels = new Pixel_24[width * height]{};
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int new_idx = (i + delta_x) + (j + delta_y) * width;
            if (new_idx < width * height) {
                new_pixels[new_idx] = get_color(i, j);
            }
        }
    }
    auto to_delete = pixels;
    pixels = new_pixels;
    delete [] to_delete;
}

void BMP_PIXEL_24::translate_image(int delta_x, int delta_y, Pixel_24 new_bg_color) {
    auto *new_pixels = new Pixel_24[width * height]{};
    for (int i = 0; i < width * height; i++) {
        new_pixels[i] = new_bg_color;
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int new_idx = (i + delta_x) + (j + delta_y) * width;
            if (new_idx < width * height) {
                new_pixels[new_idx] = get_color(i, j);
            }
        }
    }
    auto to_delete = pixels;
    pixels = new_pixels;
    delete [] to_delete;
}

void BMP_INDEX::translate_image(int delta_x, int delta_y) {
    auto *new_index = new unsigned char [width * height]{};
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int new_idx = (i + delta_x) + (j + delta_y) * width;
            if (new_idx < width * height) {
                new_index[new_idx] = palette_index[i + j * width];
            }
        }
    }
    auto to_delete = palette_index;
    palette_index = new_index;
    delete [] to_delete;
}

void BMP_INDEX::translate_image(int delta_x, int delta_y, unsigned char new_bg_color) {
    auto *new_index = new unsigned char [width * height]{};
    for (int i = 0; i < width * height; i++) {
        new_index[i] = new_bg_color;
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int new_idx = (i + delta_x) + (j + delta_y) * width;
            if (new_idx < width * height) {
                new_index[new_idx] = palette_index[i + j * width];
            }
        }
    }
    auto to_delete = palette_index;
    palette_index = new_index;
    delete [] to_delete;
}

BMP_INDEX::BMP_INDEX(BMP_PIXEL_24 *img_24, Pixel_32* new_palette, unsigned char* pidx):BMP() {
    file_type = img_24->file_type;
    file_header = img_24->file_header;
    info_header = img_24->info_header;
    info_header.biBitCount = log2(max_color);
    info_header.biSizeImage = width * height;
    file_header.bfOffBits = 54 + max_color * 4;
    file_header.bfSize = file_header.bfOffBits + info_header.biSizeImage;
    width = img_24->width;
    height = img_24->height;
    bmp_bits = info_header.biBitCount;
    palette = new_palette;
    palette_index = pidx;
}

BMP_INDEX* BMP24_to_BMP8(BMP_PIXEL_24* img_24){
    auto* o_tree = new octree();
    auto* root = new octree_node(0);
    o_tree->leaf_nodes.emplace_back(root);
    o_tree->layer_nodes->emplace_back(0);
    auto* palette = new Pixel_32[max_color]{};
    auto* new_index = new unsigned char [img_24->width * img_24->height];
    BMP_INDEX* img_8 = nullptr;

    // 八叉树索引 - 调色版索引 配对，判断这个颜色是否存在
    auto* nidx_pidx_pair = new int[pow(8, 8) + 1];
    memset(nidx_pidx_pair, -1, sizeof(int) * (pow(8, 8) + 1));

    int palette_num = 0;

    for (int i = 0; i < img_24->width * img_24->height; i++) {
        auto pixel = img_24->pixels[i];
        o_tree->add_color(root, pixel.R, pixel.G, pixel.B, false);
    }

    for (int i = 0; i < img_24->width * img_24->height; i++) {
        auto pixel = img_24->pixels[i];
        auto nidx = o_tree->query_result_index(root, pixel.R, pixel.G, pixel.B);
        // 该颜色存在于调色版中
        if (nidx_pidx_pair[nidx] > 0) {
            new_index[i] = (unsigned char)nidx_pidx_pair[nidx];
        } else {
            auto o_node = o_tree->leaf_nodes[nidx];
            palette[palette_num].R = o_node->red_sum / o_node->count;
            palette[palette_num].G = o_node->green_sum / o_node->count;
            palette[palette_num].B = o_node->blue_sum / o_node->count;
            palette[palette_num].A = 0;
            new_index[i] = palette_num;
            nidx_pidx_pair[nidx] = palette_num;
            palette_num++;
        }
    }

    img_8 = new BMP_INDEX(img_24, palette, new_index);
    return img_8;
}
