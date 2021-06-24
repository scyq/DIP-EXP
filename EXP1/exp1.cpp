#include "../headers/BMP.hpp"
#include "../headers/Histogram_Equalization.hpp"

BMP_PIXEL_24 *img;

int main()
{
    img = new BMP_PIXEL_24("../EXP1/bmp_24.bmp");
    histogram_equalization(img);
    img->derive_bmp("../EXP1/he_img.bmp");
    return 0;
}