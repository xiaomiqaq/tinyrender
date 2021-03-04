#pragma once
#include "glm/glm.hpp"
class Camera
{
public:
	Camera();
	void LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
	{
		glm::vec3 z_aix = normalize(eye - center);
		glm::vec3 x_aix = normalize(glm::cross(up, z_aix));  //叉积的顺序会影响轴的朝向
		glm::vec3 y_aix = normalize(glm::cross(z_aix, x_aix));
		CameraMatrix[0] = glm::vec4(x_aix, 0.0f);
		CameraMatrix[1] = glm::vec4(y_aix, 0.0f);
		CameraMatrix[2] = glm::vec4(z_aix, 0.0f);
	}
private:
	glm::mat4 CameraMatrix = glm::mat4(1.0);
};