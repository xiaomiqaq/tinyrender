#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "myGL.h"
#include "Shader.h"
using namespace glm;
const std::string OutputPath = "../output/tangentuv2.tga";

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor gray = TGAColor(0, 0, 255, 100);
const int width = 800;
const int height = 800; 
const int depth = 255;
vec3 light_dir(0.0, 1.0, 1.0);
vec3 eye(1.0f, 0.0f, 1.0f);
vec3 center(0.0f, 0.0f, 0.0f);
float* zbuffer = new float[width * height];

Model* model = NULL;

glm::vec3 barycentric(glm::vec3* tri_points, glm::vec3 p)
{
	//这是根据{u,v,1} 点乘 {ABx,ACx,PAx} =0 点乘 {ABy,ACy,PAy} = 0 得来的一步计算，相当于计算一个叉积。 计算结果除以u.z即可得到真正的{u,v,1}
	glm::vec3 vectorX(tri_points[2].x - tri_points[0].x, tri_points[1].x - tri_points[0].x, tri_points[0].x - p.x);
	glm::vec3 vectorY(tri_points[2].y - tri_points[0].y, tri_points[1].y - tri_points[0].y, tri_points[0].y - p.y);
	glm::vec3 u = glm::cross(vectorX, vectorY);
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
	glm::vec3 vectorX = glm::vec3(C.x - A.x, B.x - A.x, A.x - P.x);
	glm::vec3 vectorY = glm::vec3(C.y - A.y, B.y - A.y, A.y - P.y);
	glm::vec3 u = glm::cross(vectorX, vectorY);
	if (std::abs(u[2] > 1e-2))
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
	glm::vec2 p0(t0.x, t0.y);
	glm::vec2 p1(t0.x, t0.y);
	int total_height = t2.y - t0.y;
	int first_height = t1.y - t0.y;
	int second_height = t2.y - t1.y;
	for (int y = t0.y; y <= t2.y; y++)
	{
		bool second_half = y > first_height || t0.y == t1.y;
		int segment_height = second_half ? second_height : first_height;
		float k0 = (float)(y - t0.y) / total_height;
		float k1 = (float)(y - (second_half ? t1.y : t0.y)) / segment_height;
		glm::vec2 p0 = t0 + (t2 - t0) * k0;
		glm::vec2 p1 = second_half ? t1 + (t2 - t1) * k1 : t0 + (t1 - t0) * k1;
		line(p0, p1, image, white);
	}


	line(t0, t1, image, white);
	line(t1, t2, image, white);
	line(t2, t0, image, red);
}

void triangle(glm::vec3 vertexs[], TGAImage& image, TGAColor color)
{
	//找到包围三角形的包围盒
	glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	glm::vec2 bboxmax(0, 0);
	glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = std::max(0, (int)std::min(bboxmin[j], vertexs[i][j]));
			bboxmax[j] = std::min((int)screenBorder[j], (int)std::max(bboxmax[j], vertexs[i][j]));
		}
	}
	//遍历包围盒中的点
	glm::vec3 p;
	for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
	{
		for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
		{
			//glm::vec3 bc_p = barycentric(vertexs[0], vertexs[1], vertexs[2], pv);
			glm::vec3 bc_screen = barycentric(vertexs, p);
			if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0) continue;
			p.z = 0;
			for (int i = 0; i < 3; i++) p.z += vertexs[i].z * bc_screen[i];  //计算深度值,用
			if (zbuffer[int(p.x + p.y * width)] < p.z)
			{
				zbuffer[int(p.x + p.y * width)] = p.z;
				image.set(p.x, p.y, color);
			}
		}
	}
}
void triangle(glm::vec3 vertexs[], vec2 uv[] , TGAImage& image , Model* model)
{
	//找到包围三角形的包围盒
	glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	glm::vec2 bboxmax(0, 0);
	glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = std::max(0, (int)std::min(bboxmin[j], vertexs[i][j]));
			bboxmax[j] = std::min((int)screenBorder[j], (int)std::max(bboxmax[j], vertexs[i][j]));
		}
	}
	//遍历包围盒中的点
	glm::vec3 pv;
	vec2 pu;
	for (pv.x = bboxmin.x; pv.x <= bboxmax.x; pv.x++)
	{
		for (pv.y = bboxmin.y; pv.y <= bboxmax.y; pv.y++)
		{
			glm::vec3 bc_p = barycentric(vertexs, pv);  //pv的重心坐标
			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
			pv.z = 0;
			for (int i = 0; i < 3; i++)
			{
				pv.z += vertexs[i].z * bc_p[i];  //计算深度值
				pu = uv[0] * bc_p[0] + uv[1]*bc_p[1] + uv[2] * bc_p[2];
			}
			if (zbuffer[int(pv.x + pv.y * width)] < pv.z)
			{
				zbuffer[int(pv.x + pv.y * width)] = pv.z;
				image.set(pv.x, pv.y, model->diffuse(pu));
			}
		}
	}
}


//class GouraudShader : public IShader {
//	
//private:
//	glm::vec3 v2f_pos[3];  //model space
//	glm::vec2 v2f_uv[3];
//	TGAColor ambient = TGAColor(5, 5, 5, 255);
//	TGAColor specColor = TGAColor(100, 100, 100, 1);
//public:
//	glm::vec4 ndc_pos[3];
//	virtual glm::vec3 vertex(int iface, int nthvert) 
//	{
//		v2f_uv[nthvert] = model->uv(iface, nthvert);
//		v2f_pos[nthvert] = model->vert(iface, nthvert);
//		glm::vec4 gl_Vertex = vec4(model->vert(iface, nthvert),1.0f);
//		ndc_pos[nthvert] = Projection * View * gl_Vertex;
//		ndc_pos[nthvert].x = ndc_pos[nthvert].x / ndc_pos[nthvert].w;
//		ndc_pos[nthvert].y = ndc_pos[nthvert].y / ndc_pos[nthvert].w;
//		return ViewPort * ndc_pos[nthvert]; // 屏幕空间坐标
//	}
//
//	virtual bool fragment(vec3 bar, TGAColor &color) 
//	{
//		vec2 uv(0.0);
//		vec3 pos(0.0);
//		for (int i = 0; i < 3; i++) {
//			uv += v2f_uv[i] * bar[i];
//			pos += v2f_pos[i] * bar[i];
//		}
//		vec3 worldViewDir = normalize(eye - center);
//		vec3 worldNormal = normalize(vec4(model->normal(uv), 0.0f)); 
//		vec3 worldLightDir = normalize(inverse(Projection*View) * vec4(light_dir, 0.0f));
//		vec3 worldReflect = 2.0f*worldNormal*dot(worldNormal, worldLightDir) - worldLightDir;
//		float diff = dot(worldNormal, worldLightDir);
//		float spec = pow(std::max(dot(worldReflect,worldViewDir),0.0f), model->specular(uv));
//		color = ambient + model->diffuse(uv) * diff + specColor * spec * 60.0;
//		return true;                              
//	}
//
//};

class GouraudShader : public IShader {

private:
	glm::vec3 v2f_pos[3];
	glm::vec3 v2f_normal[3];
	glm::vec2 v2f_uv[3];
	glm::vec3 ndc_ver[3];
	TGAColor ambient = TGAColor(5, 5, 5, 255);
	TGAColor specColor = TGAColor(100, 100, 100, 1);
public:

	virtual glm::vec4 vertex(int iface, int nthvert)
	{
		v2f_uv[nthvert] = model->uv(iface, nthvert);
		v2f_pos[nthvert] = model->vert(iface, nthvert);
		v2f_normal[nthvert] = model->normal(iface, nthvert);
		glm::vec4 gl_Vertex = vec4(model->vert(iface, nthvert), 1.0f); // read the vertex from .obj file
		gl_Vertex = Projection * View * gl_Vertex;    //相机空间
		ndc_ver[nthvert] = gl_Vertex / gl_Vertex.w;
		ndc_ver[nthvert].z = gl_Vertex.z;
		return ViewPort * vec4(ndc_ver[nthvert],1.0); // 屏幕空间坐标
	}

	virtual bool fragment(vec3 bar, TGAColor &color)
	{
		vec2 uv(0.0);
		vec3 pos(0.0);
		vec3 normal(0.0);
		mat3 BTN;  //TANGENT TO MODEL
		for (int i = 0; i < 3; i++) {
			uv += v2f_uv[i] * bar[i];
			pos += v2f_pos[i] * bar[i];
			normal += v2f_normal[i] * bar[i];//根据顶点的法线计算法向量，是为了构建A矩阵
		}

		//caculateTangent(v2f_pos, v2f_uv);
		mat3 A;
		A[0] = v2f_pos[1] - v2f_pos[0];//向量AC;
		A[1] = v2f_pos[2] - v2f_pos[0];
		A[2] = normal;
		A = transpose(A);
		A = inverse(A);

		vec3 tangent = normalize(A * vec3(v2f_uv[1].x - v2f_uv[0].x, v2f_uv[2].x - v2f_uv[0].x, 0.0)); //u的梯度
		vec3 binTangent = normalize(A * vec3(v2f_uv[1].y - v2f_uv[0].y, v2f_uv[2].y - v2f_uv[0].y, 0.0)); //v的梯度
		BTN[0] = tangent;
		BTN[1] = binTangent;
		BTN[2] = normal;
		
		

		vec3 worldViewDir = normalize(eye - center);
		vec3 worldNormal = normalize(BTN * model->normal(uv));
		vec3 worldLightDir = normalize(inverse(Projection*View) * vec4(light_dir, 0.0f));
		vec3 worldReflect = 2.0f*worldNormal*dot(worldNormal, worldLightDir) - worldLightDir;
		float diff = dot(worldNormal, worldLightDir);
		float spec = pow(std::max(dot(worldReflect, worldViewDir), 0.0f), model->specular(uv));
		color = ambient + model->diffuse(uv) * diff ;
		return true;
	}

};

void triangle(glm::vec3 vertexs[], GouraudShader shader, TGAImage& image, TGAImage& zBuffer)
{
	//找到包围三角形的包围盒
	glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
	glm::vec2 bboxmax(0, 0);
	glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = std::max(0, (int)std::min(bboxmin[j], vertexs[i][j]));
			bboxmax[j] = std::min((int)screenBorder[j], (int)std::max(bboxmax[j], vertexs[i][j]));
		}
	}
	//遍历包围盒中的点
	glm::vec3 pv;
	for (pv.x = bboxmin.x; pv.x <= bboxmax.x; pv.x++)
	{
		for (pv.y = bboxmin.y; pv.y <= bboxmax.y; pv.y++)
		{
			glm::vec3 bc_p = barycentric(vertexs, pv);  //pv的重心坐标
			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
			pv.z = 0;
			for (int i = 0; i < 3; i++)
			{
				pv.z += vertexs[i].z * bc_p[i];  //计算深度值
			}
			if (zbuffer[int(pv.x + pv.y * width)] < pv.z)
			{
				TGAColor color;
				bool isDraw = shader.fragment(bc_p, color);
				if (isDraw)
				{
					zbuffer[int(pv.x + pv.y * width)] = pv.z;
					zBuffer.set(pv.x, pv.y, pv.z);
					image.set(pv.x, pv.y, color);
				}
			}
		}
	}
}




int main(int argc, char** argv) {
	GouraudShader shader;
	vec3 eye(1, 1, 3);
	vec3 target(0, 0, 0);
	shader.lookAt(eye, target, vec3(0, 1, 0));
	shader.projection(distance(eye,target));
	shader.viewPort(width/2, height/2, depth);
    TGAImage image(width, height, TGAImage::RGB);
	TGAImage zBuffer(width, height, TGAImage::GRAYSCALE);  //深度图

    image.flip_vertically(); // 左下角为原点
    model = new Model("../obj/african_head/african_head.obj");
	for (int iFace = 0; iFace < model->nfaces(); iFace++)
	{
		vec3 screenPos[3];
		for (int j = 0; j < 3; j++)
		{
			screenPos[j] = shader.vertex(iFace, j);
		}
		
		triangle(screenPos, shader, image, zBuffer);
	}
    image.write_tga_file(OutputPath); 
	zBuffer.write_tga_file("../output/african_head_per2_depth.tga");
    delete model;
	delete[] zbuffer ;
    return 0;
}


