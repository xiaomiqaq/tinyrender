#pragma once
#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "myGL.h"
#include "M_Shader.h"

using namespace glm;


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
		line(p0, p1, image, TR::white);
	}


	line(t0, t1, image, TR::white);
	line(t1, t2, image, TR::white);
	line(t2, t0, image, TR::red);
}


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

/* 光栅化三角形 */
//void triangle(vec3 vertexs[], IShader* shader, Framebuffer& frame)
//{
//	TR::tri_num++;
//	//找到包围三角形的包围盒
//	glm::vec2 bboxmin(frame.getWidth()-1 , frame.getHeight() - 1);
//	glm::vec2 bboxmax(0, 0);
//	glm::vec2 screenBorder(frame.getWidth() - 1, frame.getHeight() - 1);
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			bboxmin[j] = (int)(std::max)(0.0f, (std::min)(bboxmin[j], vertexs[i][j]));
//			bboxmax[j] =  (int)(std::min)(screenBorder[j],(std::max)(bboxmax[j], vertexs[i][j]) );
//		}
//	}
//	//遍历包围盒中的点
//	glm::vec3 curSV;
//	for (curSV.x = bboxmin.x; curSV.x <= bboxmax.x; curSV.x++)
//	{
//		for (curSV.y = bboxmin.y; curSV.y <= bboxmax.y; curSV.y++)
//		{
//			glm::vec3 bc_p = barycentric(vertexs, curSV);  //curSV的重心坐标
//			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
//			curSV.z = 0;
//			for (int i = 0; i < 3; i++)
//			{
//				curSV.z += vertexs[i].z * bc_p[i];  //计算深度值
//			}
//			if (curSV.z < frame.getDepth(curSV.x,curSV.y) )   //z值小的靠近相机，故显示
//			{
//				TGAColor color = TGAColor(0,0,0,0);
//				bool isDraw = shader->fragment(bc_p, color);
//				frame.setDepth(curSV.x, curSV.y, curSV.z);
//				frame.setColor(curSV.x, curSV.y, color);
//			}
//		}
//	}
//}