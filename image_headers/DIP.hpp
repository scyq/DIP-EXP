#include <cstdio>
#include <iostream>
#include <vector>

#define BMP_HEADER_LENGTH 54
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

class BMP
{
private:
    FILE *fp;

public:
    BMP(const char *file_name);
    ~BMP();
    int32_t width;
    int32_t height;
    Pixel pixels[MAX_ROW][MAX_COL];
    char bmp_header[BMP_HEADER_LENGTH];
    void to_txt(const char *outfile_name, int row, int col);
    void changeInRectangle(Pixel target_pixel, Point left_bottom, Point right_top);

    static void create_new_bmp(char *header, Pixel **pixels, int width, int height, const char *target_file_name);
};

BMP::BMP(const char *file_name)
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
    fread(pixels, COLOR_COPONENTS, width * height, fp);
    fclose(fp);
}

void BMP::to_txt(const char *outfile_name, int row, int col)
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

void BMP::create_new_bmp(char *header, Pixel **bmp_pixels, int width, int height, const char *target_file_name)
{
    FILE *fout = fopen(target_file_name, "wb");
    fwrite(header, BMP_HEADER_LENGTH, 1, fout);
    fwrite(bmp_pixels, COLOR_COPONENTS, width * height, fout);
    fclose(fout);
}

void BMP::changeInRectangle(Pixel target_pixel, Point left_bottom, Point right_top)
{
    std::cout << (int)target_pixel.R << ' ' << (int)target_pixel.G << ' ' << (int)target_pixel.B << std::endl;
    for (int i = left_bottom.x; i < right_top.x; i++)
    {
        for (int j = left_bottom.y; j < right_top.y; j++)
        {
            pixels[i][j] = target_pixel;
        }
    }
}

BMP::~BMP()
{
    delete (fp);
}

Pixel get_random_color()
{
    srand((unsigned)time(NULL));
    /* return [a, b] */
    int a = 0;
    int b = 255;

    int r = (rand() % (b - a + 1)) + a;
    int g = (rand() % (b - a + 1)) + a;
    int _b = (rand() % (b - a + 1)) + a;
    Pixel temp = {_b, g, r};

    return temp;
}