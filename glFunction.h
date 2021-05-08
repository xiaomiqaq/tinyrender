#pragma once
#include <vector>
#include <cmath>
#include "model.h"
#include "tgaimage.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "myGL.h"
#include "Shader.h"
#include "base.h"
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
//ɨ�����㷨
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
	//���Ǹ���{u,v,1} ��� {ABx,ACx,PAx} =0 ��� {ABy,ACy,PAy} = 0 ������һ�����㣬�൱�ڼ���һ������� ����������u.z���ɵõ�������{u,v,1}
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