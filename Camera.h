#pragma once
#include "glm/glm.hpp"
class Camera
{
public:
	Camera() {};
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	{
		glm::vec3 vCenterToEye = eye - center;
		glm::vec3 z_aix = normalize(vCenterToEye);
		glm::vec3 x_aix = normalize(glm::cross(up, z_aix));  //叉积的顺序会影响轴的朝向
		glm::vec3 y_aix = normalize(glm::cross(z_aix, x_aix));
		/*viewMat[0] = glm::vec4(x_aix, vCenterToEye.x);
		viewMat[1] = glm::vec4(y_aix, vCenterToEye.y);
		viewMat[2] = glm::vec4(z_aix, vCenterToEye.z);*/
		viewMat[0] = glm::vec4(x_aix, 0.f);
		viewMat[1] = glm::vec4(y_aix, 0.f);
		viewMat[2] = glm::vec4(z_aix, 0.f);
		viewMat = glm::inverse(viewMat);

		float coeff = glm::length(eye-center);
		ProjectionMat[2].w = -(1.0 / coeff);
	}
	glm::mat4 View() { return viewMat; }
	glm::mat4 Projection() { return ProjectionMat; }
private:
	glm::mat4 viewMat = glm::mat4(1.0);
	glm::mat4 ProjectionMat = glm::mat4(1.0f);
	glm::vec3 pos = glm::vec3(0.0f, 0.f, 0.f);
	float far = 0.f;
	float near = 0.f;
	float fov = 0.f;
	float aspect = 1.f;
};