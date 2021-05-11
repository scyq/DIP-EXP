#include "../headers/BMP.hpp"

BMP_PIXEL *img_24;
BMP_INDEX *img_8;

void test_0()
{
    output2txt(10, 10, "../EXP0/1.txt", img_24);
}

void test_1()
{
    Point l_b = {img_8 -> width / 3, img_8->height / 3};
    Point r_t = {img_8->width * 2 / 3, img_8->height * 2 / 3};
    img_24->recolor_rec(get_random_color(), l_b, r_t);
    img_24->derive_bmp("../EXP0/2.bmp");
}

int main()
{
    img_8 = new BMP_INDEX("../EXP0/qiuqiu_8.bmp");
    img_24 = new BMP_PIXEL("../EXP0/qiuqiu_24.bmp");
    test_0();
    test_1();
}