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

typedef struct Pixel_24 {
    unsigned char B, G, R;
} Pixel_24;

typedef struct Pixel_32 {
    unsigned char B, G, R, A;
} Pixel_32;

typedef struct Point {
    int x;
    int y;
} Point;

void print_pixel(Pixel_24 p) {
    std::cout << (int) p.R << ' ' << (int) p.G << ' ' << (int) p.B << std::endl;
}

void print_pixel(Pixel_32 p) {
    std::cout << (int) p.R << ' ' << (int) p.G << ' ' << (int) p.B << " " << (int) p.A << std::endl;
}

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
    explicit BMP(const char *file_name);

    ~BMP();

    int32_t width;
    int32_t height;
    int bmp_bits;
    unsigned short file_type;   // 0x4d42，必须是BM字符串，对应的十六进制为0x4d42,十进制为19778
    BMP_FILE_HEADER file_header;
    BMP_INFO_HEADER info_header;

    virtual void derive_bmp(const char *target_file_name) = 0;
    int coordinate2index(int c_x, int c_y);
};

// 存像素值的BMP
class BMP_PIXEL_24 : public BMP {
public:
    ~BMP_PIXEL_24();

    explicit BMP_PIXEL_24(const char *file_name);

    Pixel_24 *pixels;

    void recolor_rec(Pixel_24 target_pixel, Point left_bottom, Point right_top);

    void derive_bmp(const char *target_file_name);

    Pixel_24 get_color(int x, int y);
};

// 存索引值的BMP
class BMP_INDEX : public BMP {
public:
    ~BMP_INDEX();

    explicit BMP_INDEX(const char *file_name);

    int color_counts;

    unsigned char *palette_index;

    Pixel_32 *palette;

    void recolor_palette(Pixel_32 target_color, int target_pixel_index);

    void derive_bmp(const char *target_file_name);

    Pixel_32 get_color(int x, int y);
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
    fread(pixels, 1, width * height * sizeof(Pixel_24), fp);
    fclose(fp);
}

BMP_INDEX::BMP_INDEX(const char *file_name) : BMP(file_name) {
    color_counts = pow(2, bmp_bits);
    palette = new Pixel_32[color_counts];
    palette_index = new unsigned char[width * height];
    fread(palette, 1, color_counts * sizeof(Pixel_32), fp);
    fread(palette_index, 1, width * height * sizeof(int), fp);
    fclose(fp);
}

int BMP::coordinate2index(int c_x, int c_y) {
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
    delete[] palette_index;
    delete[] palette;
}

void BMP_INDEX::recolor_palette(Pixel_32 target_color, int target_pixel_index) {
    palette[target_pixel_index] = target_color;
}

BMP_PIXEL_24::~BMP_PIXEL_24() {
    delete[] pixels;
}