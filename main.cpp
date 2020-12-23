#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
using namespace glm;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor gray = TGAColor(0, 0, 255, 100);
const int width = 800;
const int height = 800; 
vec3 light_dir(0, 0, -1);
float* zbuffer = new float[width * height];
int draw = 0;
int cull = 0;
//计算重心坐标
glm::fvec3 barycentric(glm::vec3* tri_points, glm::vec3 p)
{
    //这是根据{u,v,1} 点乘 {ABx,ACx,PAx} =0 点乘 {ABy,ACy,PAy} = 0 得来的一步计算，相当于计算一个叉积。 计算结果除以u.z即可得到真正的{u,v,1}
    glm::vec3 u = glm::cross(glm::vec3(tri_points[2].x - tri_points[0].x, tri_points[1].x - tri_points[0].x, tri_points[0].x - p.x), glm::vec3(tri_points[2].y - tri_points[0].y, tri_points[1].y - tri_points[0].y, tri_points[0].y - p.y));
    if (std::abs(u[2]) > 1e-2) 
        return glm::fvec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return glm::fvec3(-1, 1, 1);
}
glm::vec3 barycentric(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P)
{
    glm::vec3 s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    glm::vec3 u = glm::cross(s[0], s[1]);
    if(std::abs(u[2]>1e-2)) 
        return glm::fvec3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return glm::fvec3(-1, 1, 1);
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

void triangle(glm::vec3 tri[], TGAImage& image, TGAColor color)
{
    //找到包围三角形的包围盒
    glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
    glm::vec2 bboxmax(0, 0);
    glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0,(int) std::min(bboxmin[j], tri[i][j]));
            bboxmax[j] = std::min((int)screenBorder[j], (int)std::max(bboxmax[j], tri[i][j]));
        }
    }
    //遍历包围盒中的点
    glm::vec3 p;
    for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
    {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            glm::vec3 bc_screen = barycentric(tri[0], tri[1], tri[2], p);
           // glm::vec3 bc_screen = barycentric(tri, p);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
            p.z = 0;
            for (int i = 0; i < 3; i++) p.z += tri[i].z * bc_screen[i];
            if (zbuffer[int(p.x + p.y * width)] < p.z)
            {
                zbuffer[int(p.x + p.y * width)] = p.z;
                image.set(p.x, p.y, color);
                draw++;
            }
            else
            {
                cull++;
            }

        }
    }
}

glm::vec3 world2screen(glm::vec3 v)
{
    return glm::vec3(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), v.z);
}
void DrawModel(Model* model,TGAImage& image)
{
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        glm::vec3 pts[3];
        for (int j = 0; j < 3; j++) pts[j] = world2screen(model->vert(face[j]));
       
        vec3 normal = cross((pts[2] - pts[0]), (pts[1] - pts[0]));
        normal = normalize(normal);
        light_dir = normalize(light_dir);
        float intensity = dot(normal, light_dir);
        
        //triangle(pts, image, TGAColor(255 * intensity,255* intensity,255 * intensity));
        triangle(pts, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }
}
int main(int argc, char** argv) {
   
    TGAImage image(width, height, TGAImage::RGB);
    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    Model* model = new Model("../obj/african_head/african_head.obj");
    DrawModel(model,image);
    image.write_tga_file("african_head.tga"); 
    delete model;

    std::cout << "draw" << draw
        <<"\n cull"<<cull<<std::endl;

    return 0;
}