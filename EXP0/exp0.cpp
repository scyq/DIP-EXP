#include "../image_headers/DIP.hpp"

int main()
{
    Image *img = new Image("qiuqiu.bmp");
    img->to_txt("1.txt", 5, 5);
}