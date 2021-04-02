#include <cstdio>
#include <iostream>
#include <vector>

#define HEADER_LENGTH 54
#define COLOR_COPONENTS 3

// 最大支持8K图片
#define MAX_ROW 7680
#define MAX_COL 4320

typedef struct Pixel
{
    unsigned char B, G, R;
} Pixel;

typedef struct Point
{
    int x;
    int y;
} Point;

class Image
{
private:
    FILE *fp;
    char image_header[HEADER_LENGTH];
    int32_t width;
    int32_t height;

public:
    Image(const char *file_name);
    ~Image();
    Pixel pixels[MAX_ROW][MAX_COL];
    void to_txt(const char *outfile_name, int row, int col);
    void changeInRectangle(Pixel target_pixel, Point left_top, Point right_bottom);
};

Image::Image(const char *file_name)
{
    // 获取图像长宽
    const int offset = sizeof(uint16_t) * 3 + sizeof(uint32_t) * 3;
    fp = fopen(file_name, "rb");
    fseek(fp, offset, SEEK_SET);
    fread(&width, 1, sizeof(width), fp);
    fread(&height, 1, sizeof(height), fp);
    width = abs(width);
    height = abs(height);

    if (width > MAX_ROW || height > MAX_COL)
    {
        std::cout << "图像太大，无法读取";
        return;
    }

    fseek(fp, 0, SEEK_SET);
    fread(image_header, 1, HEADER_LENGTH, fp);
    fread(pixels, COLOR_COPONENTS, width * height, fp);
    fclose(fp);
}

void Image::to_txt(const char *outfile_name, int row, int col)
{
    if (row > width || col > height)
    {
        std::cout << "图像数据不够";
        return;
    }
    fp = fopen(outfile_name, "w");
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            fprintf(fp, "(%d, %d, %d)", pixels[i][j].R, pixels[i][j].G, pixels[i][j].B);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void Image::changeInRectangle(Pixel target_pixel, Point left_top, Point right_bottom)
{
}

Image::~Image()
{
    delete (fp);
}