#include <cstdio>
#include <iostream>
#include <vector>

#define BMP_HEADER_LENGTH 54

// 最大支持8K图片
#define MAX_ROW 7680
#define MAX_COL 4320

typedef struct Pixel_32
{
    unsigned char B, G, R, A;
} Pixel;

typedef struct Point
{
    int x;
    int y;
} Point;

void print_pixel(Pixel_32 p) {
    std::cout << (int)p.R << ' ' << (int)p.G << ' ' << (int)p.B << std::endl;
}

// bmp坐标转pixels数组下标
int coordinate2index(int c_x, int c_y, int width) {
    return c_x + c_y * width;
}

class BMP_32
{
private:
    FILE *fp;

public:
    BMP_32(const char *file_name);
    ~BMP_32();
    int32_t width;
    int32_t height;
    Pixel_32 pixels[MAX_ROW * MAX_COL];
    char bmp_header[BMP_HEADER_LENGTH];
    void to_txt(const char *outfile_name, int row, int col);
    void recolor_rec(Pixel_32 target_pixel, Point left_bottom, Point right_top);

    static void create_new_bmp32(char *header, Pixel_32 **bmp_pixels, int width, int height, const char *target_file_name);
};

BMP_32::BMP_32(const char *file_name)
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
    fread(bmp_header, 1, BMP_HEADER_LENGTH, fp);
    fread(pixels, 1, width * height * sizeof(Pixel_32), fp);
    fclose(fp);
}

void BMP_32::to_txt(const char *outfile_name, int row, int col)
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
            fprintf(fp, "(%d, %d, %d)",
                    pixels[coordinate2index(i, j, width)].R,
                    pixels[coordinate2index(i, j, width)].G,
                    pixels[coordinate2index(i, j, width)].B);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void BMP_32::create_new_bmp32(char *header, Pixel_32 **bmp_pixels, int width, int height, const char *target_file_name)
{
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(header, BMP_HEADER_LENGTH, 1, fout);
    fwrite(bmp_pixels, 1, height * width * sizeof(Pixel_32), fout);
    fclose(fout);
}

// 重新设置一个矩形区域的像素颜色
void BMP_32::recolor_rec(Pixel_32 target_pixel, Point left_bottom, Point right_top)
{
    for (int i = left_bottom.x; i <= right_top.x; i++)
    {
        for (int j = left_bottom.y; j <= right_top.y; j++)
        {
            pixels[coordinate2index(i, j, width)] = target_pixel;
        }
    }
}

BMP_32::~BMP_32()
{
    delete (fp);
}

Pixel_32 get_random_color()
{
    srand((unsigned)time(NULL));
    /* return [a, b] */
    int a = 0;
    int b = 255;

    unsigned char r = (rand() % (b - a + 1)) + a;
    unsigned char g = (rand() % (b - a + 1)) + a;
    unsigned char _b = (rand() % (b - a + 1)) + a;
    Pixel_32 temp = {_b, g, r};

    return temp;
}