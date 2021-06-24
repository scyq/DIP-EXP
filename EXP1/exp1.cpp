#include "../headers/BMP.hpp"
#include "../headers/Histogram_Equalization.hpp"

BMP_PIXEL_24 *img;

void exp_0() {
    histogram_equalization(img);
    img->derive_bmp("../EXP1/he_img.bmp");
}

void exp_1() {
    img->to_grey();
    histogram_equalization_grey(img);
    img->derive_bmp("../EXP1/he_grey.bmp");
}

void exp_2() {
    histogram_equalization_single_channel(img, 2);
    img->derive_bmp("../EXP1/he_B.bmp");
}


int main()
{
    img = new BMP_PIXEL_24("../EXP1/wsy_bmp.bmp");
//    exp_0();
//    exp_1();
    exp_2();
    return 0;
}