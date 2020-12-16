#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800; 


void line(Vec2i t0, Vec2i t1, TGAImage& image, TGAColor color) {
    int x0 = t0.x, y0 = t0.y, x1 = t1.x, y1 = t1.y;
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    float derror = std::abs(dy / float(dx));
    float error = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error += derror;
        if (error > .5) {
            y += (y1 > y0 ? 1 : -1);
            error -= 1.;
        }
    }
}
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
{
    //let t0 in the upmost
    /*if (t0.x > t1.x) std::swap(t0, t1);
    if (t0.x > t2.x) std::swap(t0, t2);
    if (t1.x > t2.x) std::swap(t1, t2);*/
    if (t0.y < t1.y) std::swap(t0, t1);
    if (t0.y < t2.y) std::swap(t0, t2);
    if (t1.y < t2.y) std::swap(t1, t2);
    int linesNum = t0.y - t2.y ;
    float k0 = (t0.x - t2.x) / (t0.y - t2.y);
    float dx = 0;
    Vec2i p0 = Vec2i(t0.x, t0.y);
    Vec2i p1 = Vec2i(t0.x, t0.y);
    int total_height = t0.y - t2.y;
    int segment_height1 = t0.y - t1.y;
    int segment_height2 = t1.y - t2.y;
    for (int y = t0.y; y >= t1.y; y--)
    {
        float k0 = (float)(t0.y - y) / total_height;
        float k1 = (float)(t0.y - y) / segment_height1;
        Vec2i p0 = t0 - (t0 - t2) * k0;
        Vec2i p1 = t0 - (t0 - t1) * k1;
        line(p0, p1, image, white);
    }
    for (int y = t1.y; y >= t2.y; y--) 
    {
        float k0 = (float)(t0.y - y) / total_height;
        float k2 = (float)(t1.y - y) / segment_height2;
        Vec2i p0 = t0 - (t0 - t2) * k0;
        Vec2i p1 = t1 - (t1 - t2) * k2;
        line(p0, p1, image, red);
    }

    line(t0, t1, image, white);
    line(t1, t2, image, white);
    line(t2, t0, image, red);
}


int main(int argc, char** argv) {
    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    Vec2i t0[3] = { Vec2i(0,0),Vec2i(40,10), Vec2i(20,40)};
    triangle(t0[0], t0[1], t0[2], image, white);
   
    image.write_tga_file("line.tga"); 
    return 0;
}