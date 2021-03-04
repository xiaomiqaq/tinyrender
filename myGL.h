#pragma once
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
using namespace glm;
//计算重心坐标

class Shader
{

	virtual ~Shader();
	virtual glm::vec3 Vertex();
	virtual void Fragment();
};
class TinyGL
{

public:
	TinyGL() {}
	~TinyGL() {}
	glm::vec3 model2screen(glm::vec3 v_model)
	{
		return ViewPort * Projection * ViewModel * glm::vec4(v_model, 1.0f);
	}
	void lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up)
	{
		glm::vec3 z_aix = normalize(eye - target);
		glm::vec3 x_aix = normalize(glm::cross(up, z_aix));  //叉积的顺序会影响轴的朝向
		glm::vec3 y_aix = normalize(glm::cross(z_aix, x_aix));
		ViewModel[0] = vec4(x_aix, 0.0f);
		ViewModel[1] = vec4(y_aix, 0.0f);
		ViewModel[2] = vec4(z_aix, 0.0f);
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
	
private:
	glm::mat4 ViewModel = glm::mat4(1.0f);
	glm::mat4 Projection = glm::mat4(1.0f);
	glm::mat4 ViewPort = glm::mat4(1.0f);

	
};