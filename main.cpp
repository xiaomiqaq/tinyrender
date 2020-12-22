#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
using namespace glm;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800; 
vec3 light_dir(0, 0, -1);
int* zbuffer = new int[width * height];
//计算重心坐标
glm::fvec3 barycentric(std::vector<vec3> pts, glm::vec2 p)
{
    //这是根据{u,v,1} 点乘 {ABx,ACx,PAx} =0 点乘 {ABy,ACy,PAy} = 0 得来的一步计算，相当于计算一个叉积。 计算结果除以u.z即可得到真正的{u,v,1}
    glm::vec3 u = glm::cross(glm::vec3(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x), glm::vec3(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y));
    if (std::abs(u[2]) < 1) return glm::fvec3(-1, 1, 1);
    return glm::fvec3(1 - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}
void line(glm::vec2 t0, glm::vec2 t1, TGAImage& image, TGAColor color) {
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
//扫描线算法
void triangle(glm::vec2 t0, glm::vec2 t1, glm::vec2 t2, TGAImage& image, TGAColor color)
{
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);
    float k0 = (t2.x - t0.x) / (t2.y - t0.y);
    float dx = 0;
    glm::vec2 p0 (t0.x, t0.y);
    glm::vec2 p1 (t0.x, t0.y);
    int total_height = t2.y - t0.y;
    int first_height = t1.y - t0.y;
    int second_height = t2.y - t1.y;
    for (int y = t0.y; y <= t2.y; y++)
    {
        bool second_half = y > first_height || t0.y == t1.y;
        int segment_height = second_half ? second_height : first_height;
        float k0 = (float)(y - t0.y) / total_height;
        float k1 = (float)(y - ( second_half ? t1.y : t0.y)) / segment_height;
        glm::vec2 p0 = t0 + (t2 - t0) * k0;
        glm::vec2 p1 = second_half ? t1 + (t2 - t1) * k1 : t0 + (t1 - t0) * k1;
        line(p0, p1, image, white);
    }
   

    line(t0, t1, image, white);
    line(t1, t2, image, white);
    line(t2, t0, image, red);
}

void triangle(std::vector<vec3> pts, TGAImage& image,TGAColor color)
{
    //找到包围三角形的包围盒
    glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
    glm::vec2 bboxmax(0, 0);
    glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0,(int) std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min((int)screenBorder[j], (int)std::max(bboxmax[j], pts[i][j]));
        }
        //遍历包围盒中的点
        glm::vec3 p;
        for (p.x = bboxmin.x; p.x <= bboxmax.x ; p.x++)
        {
            for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
            {
                glm::fvec3 bc = barycentric(pts, p);
                if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
                p.z = 0;
                for (int i = 0; i < 3; i++) p.z += pts[i].z * bc[i];
                if(zbuffer[int(p.x+p.y*width)] < p.z)
                {
                    zbuffer[int(p.x + p.y * width)] = p.z;
                    image.set(p.x, p.y, color);
                }
                else
                {
                    int i = 0;
                }

            }
        }
    }
}
void DrawModel(Model* model,TGAImage& image)
{
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        std::vector<glm::vec3> screen_face;
        std::vector<glm::vec3> world_face;
        for (int j = 0; j < 3; j++)
        {
            glm::vec3 world_coord = model->vert(face[j]);
            glm::vec3 screen_coord = glm::vec3((world_coord.x + 1.) * width / 2, (world_coord.y + 1.) * height / 2 , world_coord.z);
            screen_face.push_back(screen_coord);
            world_face.push_back(world_coord);
        }
        vec3 normal = cross((world_face[2] - world_face[0]), (world_face[1] - world_face[0]));
        normal = normalize(normal);
        float intensity = dot(normal, light_dir);
        
        triangle(screen_face, image, TGAColor(255 * intensity, 255 * intensity, 255 * intensity));
    }
}
int main(int argc, char** argv) {
   
    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    Model* model = new Model("../obj/african_head/african_head.obj");
    DrawModel(model,image);
    image.write_tga_file("head2.tga"); 
    delete model;
    return 0;
}