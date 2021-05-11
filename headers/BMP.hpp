#pragma once

#include <cstdio>
#include <iostream>
#include <vector>

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
    unsigned int biClrUsed;   // 0
    unsigned int biClrImportant; // 0
} BMP_INFO_HEADER;

typedef struct Pixel {
    unsigned char B, G, R;
} Pixel;

typedef struct Point {
    int x;
    int y;
} Point;

void print_pixel(Pixel p) {
    std::cout << (int) p.R << ' ' << (int) p.G << ' ' << (int) p.B << std::endl;
}

Pixel get_random_color() {
    srand((unsigned) time(nullptr));
    /* return [a, b] */
    int a = 0;
    int b = 255;

    unsigned char r = (rand() % (b - a + 1)) + a;
    unsigned char g = (rand() % (b - a + 1)) + a;
    unsigned char _b = (rand() % (b - a + 1)) + a;
    Pixel temp = {_b, g, r};

    return temp;
}

class BMP {
protected:
    FILE *fp;

public:
    explicit BMP(const char *file_name);

    ~BMP();

    int32_t width;
    int32_t height;
    int bmp_bits;
    unsigned short file_type;   // 0x4d42，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778
    BMP_FILE_HEADER file_header;
    BMP_INFO_HEADER info_header;

    virtual void derive_bmp(const char *target_file_name) = 0;

    virtual Pixel get_color(int x, int y) = 0;

    int coordinate2index(int c_x, int c_y);
};

// 存像素值的BMP
class BMP_PIXEL : public BMP {
public:
    ~BMP_PIXEL();

    explicit BMP_PIXEL(const char *file_name);

    Pixel *pixels;

    void recolor_rec(Pixel target_pixel, Point left_bottom, Point right_top);

    void derive_bmp(const char *target_file_name);

    Pixel get_color(int x, int y);
};

// 存索引值的BMP
class BMP_INDEX : public BMP {
public:
    ~BMP_INDEX();

    explicit BMP_INDEX(const char *file_name);

    int *palette_index;

    Pixel *palette;

    void recolor_palette(Pixel target_color, int target_pixel_index);

    void derive_bmp(const char *target_file_name);

    Pixel get_color(int x, int y);
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

BMP_PIXEL::BMP_PIXEL(const char *file_name) : BMP(file_name) {
    pixels = new Pixel[width * height];
    fread(pixels, 1, width * height * sizeof(Pixel), fp);
    fclose(fp);
}

BMP_INDEX::BMP_INDEX(const char *file_name) : BMP(file_name) {
    palette = new Pixel[pow(2, bmp_bits)];
    palette_index = new int[width * height];
    fread(palette, 1, pow(2, bmp_bits) * sizeof(Pixel), fp);
    fread(palette_index, 1, width * height * sizeof(int), fp);
    fclose(fp);
}

int BMP::coordinate2index(int c_x, int c_y) {
    return c_x + c_y * width;
};

Pixel BMP_INDEX::get_color(int x, int y) {
    return palette[palette_index[coordinate2index(x, y)]];
}

Pixel BMP_PIXEL::get_color(int x, int y) {
    return pixels[coordinate2index(x, y)];
}

void BMP_PIXEL::recolor_rec(Pixel target_pixel, Point left_bottom, Point right_top) {
    for (int i = left_bottom.x; i <= right_top.x; i++) {
        for (int j = left_bottom.y; j <= right_top.y; j++) {
            pixels[coordinate2index(i, j)] = target_pixel;
        }
    }
}

BMP::~BMP() {
    delete (fp);
}

void BMP_INDEX::derive_bmp(const char *target_file_name) {
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(&file_type, sizeof(file_type), 1, fout);
    fwrite(&file_header, sizeof(file_header), 1, fout);
    fwrite(&info_header, sizeof(info_header), 1, fout);
    fwrite(palette, 1, pow(2, bmp_bits) * sizeof(Pixel), fout);
    fwrite(palette_index, 1, width * height * sizeof(int), fout);
    fclose(fout);
}

void BMP_PIXEL::derive_bmp(const char *target_file_name) {
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(&file_type, sizeof(file_type), 1, fout);
    fwrite(&file_header, sizeof(file_header), 1, fout);
    fwrite(&info_header, sizeof(info_header), 1, fout);
    fwrite(pixels, 1, height * width * sizeof(Pixel), fout);
    fclose(fout);
}

BMP_INDEX::~BMP_INDEX() {
    delete[] palette_index;
    delete[] palette;
}

void BMP_INDEX::recolor_palette(Pixel target_color, int target_pixel_index) {
    palette[target_pixel_index] = target_color;
}

BMP_PIXEL::~BMP_PIXEL() {
    delete[] pixels;
}

void output2txt(int x_max, int y_max, const char *target_txt, BMP *img) {
    if (x_max > img->width || y_max > img->height) {
        std::cout << "图像数据不够" << std::endl;
        return;
    }
    FILE *fp = fopen(target_txt, "w");
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            auto pix = img->get_color(x, y);
            fprintf(fp, "(%d, %d, %d)", pix.R, pix.G, pix.B);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}