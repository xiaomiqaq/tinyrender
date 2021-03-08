#pragma once
#include<glm/glm.hpp>
#include"tgaimage.h"

class IShader {
public:
	glm::mat4 View = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 ViewPort = glm::mat4(1.0f);

	void lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up)
	{
		glm::vec3 z_aix = normalize(eye - target);
		glm::vec3 x_aix = normalize(glm::cross(up, z_aix));  //叉积的顺序会影响轴的朝向
		glm::vec3 y_aix = normalize(glm::cross(z_aix, x_aix));
		View[0] = vec4(x_aix, 0.0f);
		View[1] = vec4(y_aix, 0.0f);
		View[2] = vec4(z_aix, 0.0f);
		View = inverse(View);
	}
	void projection(float coeff)
	{
		Projection[2].w = -(1.0 / coeff);
	}
	void viewPort(int width, int height, int depth)
	{
		ViewPort[0] = vec4(width / 2.0f, 0.0f, 0.0f, 0.0f);
		ViewPort[1] = vec4(0.0f, height / 2.0f, 0.0f, 0.0f);
		ViewPort[2] = vec4(0.0f, 0.0f, depth / 2.0f, 0.0f);
		ViewPort[3] = vec4(width / 2.0f, height / 2.0f, depth / 2.0f, 1);
	}
public:
	virtual glm::vec4 vertex(int iface, int nthvert) = 0;
	virtual bool fragment(glm::vec3 bar, TGAColor &color) = 0;
};

