#include "../image_headers/DIP.hpp"

int main()
{
    BMP *img = new BMP("qiuqiu.bmp");
    img->to_txt("1.txt", 5, 5);

    //Point l_b = {img->width / 3, img->height / 3};
    Point l_b = {0, 0};
    Point r_t = {img->width * 2 / 3, img->height * 2 / 3};
    img->changeInRectangle(get_random_color(), l_b, r_t);
    BMP::create_new_bmp(img->bmp_header, (Pixel **)img->pixels, img->width, img->height, "2.bmp");
}