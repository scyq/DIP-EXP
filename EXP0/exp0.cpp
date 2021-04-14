#include "../image_headers/DIP.hpp"

BMP_32 *img;

void test_0()
{
    img->to_txt("1.txt", 5, 5);
}

void test_1(Point l_b, Point r_t)
{
    img->recolor_rec(get_random_color(), l_b, r_t);
    BMP_32::create_new_bmp32(img->bmp_header, (Pixel_32 **) img->pixels, img->width, img->height, "../EXP0/2.bmp");
}

int main()
{
    img = new BMP_32("../EXP0/qiuqiu.bmp");

    Point l_b = {img -> width / 3, img->height / 3};
    Point r_t = {img->width * 2 / 3, img->height * 2 / 3};

    test_1(l_b, r_t);
}