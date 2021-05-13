#include "../headers/BMP.hpp"

BMP_PIXEL_24 *img_24;
BMP_INDEX *img_8;
#include <unistd.h>
//#include <chrono>
//#include <ctime>

//using std::cout; using std::endl;
//using std::chrono::duration_cast;
//using std::chrono::milliseconds;
//using std::chrono::seconds;
//using std::chrono::system_clock;

void exp_1()
{
    FILE *fp = fopen("../EXP0/1.txt", "w");
    int x_max = 10;
    int y_max = 10;
    for (int y = 0; y < y_max; y++) {
        for (int x = 0; x < x_max; x++) {
            auto pix = img_24->get_color(x, y);
            fprintf(fp, "(%d, %d, %d)", pix.R, pix.G, pix.B);
        }
        fprintf(fp, "\n");
    }
}

void exp_2()
{
    Point l_b = {img_24 -> width / 3, img_24->height / 3};
    Point r_t = {img_24->width * 2 / 3, img_24->height * 2 / 3};
    img_24->recolor_rec(get_random_color_24(), l_b, r_t);
    img_24->derive_bmp("../EXP0/2.bmp");
}

void exp_3() {
    for (int i = 0; i < img_8->color_counts; i++) {
        img_8->recolor_palette(get_random_color_32(), i);
        usleep(10000);
    }
    img_8->derive_bmp("../EXP0/3.bmp");
}

void exp_4() {
    img_24->set_new_size(img_24->width + 120, img_24->height + 120, {0, 255, 0});
    img_24->translate_image(60, 60, {0, 255, 0});
    img_24->derive_bmp("../EXP0/4_24.bmp");

    img_8->set_new_size(img_8->width + 120, img_8->height + 120, 23);
    img_8->translate_image(60, 60, 23);
    img_8->derive_bmp("../EXP0/4_8.bmp");
}

void bonus_1() {
    img_8->info_header.biClrUsed = 128;
    img_8->derive_bmp("../EXP0/bonus1_BiColorUsed.bmp");
    img_8->info_header.biClrUsed = 0;
    img_8->info_header.biClrImportant = 1;
    img_8->derive_bmp("../EXP0/bonus1_BiColorImportant.bmp");
}

void bonus_2() {
    auto converted = BMP24_to_BMP8(img_24);
    converted->derive_bmp("../EXP0/bonus2.bmp");
}

int main()
{
    img_8 = new BMP_INDEX("../EXP0/bmp_8.bmp");
    img_24 = new BMP_PIXEL_24("../EXP0/bmp_24.bmp");
//    exp_1();
//    exp_2();
//    exp_3();
//    exp_4();
    bonus_1();
//    auto now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//    bonus_2();
//    auto end = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
//    std::cout << end - now;
}