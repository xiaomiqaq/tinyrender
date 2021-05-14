//#pragma once
//#include "glm/glm.hpp"
//#include "Input.h"
//class Camera
//{
//public:
//	Camera() {};
//	Camera(glm::vec3 pos_, float near_, float far_, float fov_, float aspect_ , glm::vec3 up_) : pos(pos_), near(near_), far(far_), fov(fov_), aspect(aspect_),up(up_) {};
//	
//	void LookAt(glm::vec3 center)
//	{
//		glm::vec3 vCentToCam = pos - center;
//		glm::vec3 z_aix = normalize(vCentToCam);  //左手坐标系
//		glm::vec3 x_aix = normalize(glm::cross(up, z_aix));  //叉积的顺序会影响轴的朝向
//		glm::vec3 y_aix = normalize(glm::cross(z_aix, x_aix));
//
//		cameraMat[0] = glm::vec4(x_aix, 0.f);
//		cameraMat[1] = glm::vec4(y_aix, 0.f);
//		cameraMat[2] = glm::vec4(z_aix, 0.f);
//		cameraMat[3] = glm::vec4(vCentToCam,1.0f);   //此时矩阵是相机空间的点在世界空间的表示，取反得到世界空间的点在相机空间表示
//		viewMat = glm::inverse(cameraMat);
//
//	}
//	void Perspective()
//	{
//		ProjectionMat[0] = glm::vec4(1.f / (tan(glm::radians(fov/2)) *aspect), 0.f, 0.f, 0.f);
//		ProjectionMat[1] = glm::vec4(0.f, 1.f / tan(glm::radians(fov/2)), 0.f, 0.f);
//		ProjectionMat[2] = glm::vec4(0.f, 0.f, -(far + near) / (far - near), -2 * near*far / (far - near));
//		ProjectionMat[3] = glm::vec4(0.f, 0.f, -1.f, 0.f);
//	}
//	glm::mat4 View() { return viewMat; }
//	glm::mat4 Projection() { return ProjectionMat; }
//	void Update()
//	{
//		if (INPUT_STATE.mouseDown)
//		{
//			if (abs(INPUT_STATE.mouse_delt.y) > abs(INPUT_STATE.mouse_delt.x)) 
//			{
//				if (INPUT_STATE.mouse_delt.y > 0)
//					pos = length(pos)* normalize((glm::vec3(cameraMat[2]) + glm::vec3(cameraMat[1]) * INPUT_STATE.mouse_sensitivity));   //***公式：相机的 new_z_aix=pos=z_aix+y_aix*delta 
//				else if (INPUT_STATE.mouse_delt.y < 0)
//					pos = length(pos)* normalize((glm::vec3(cameraMat[2]) - glm::vec3(cameraMat[1]) * INPUT_STATE.mouse_sensitivity));
//			}
//			else 
//			{
//				if (INPUT_STATE.mouse_delt.x > 0)
//					pos = length(pos)* normalize((glm::vec3(cameraMat[2]) - glm::vec3(cameraMat[0]) * INPUT_STATE.mouse_sensitivity));   //***公式：相机的 new_z_aix=pos=z_aix+y_aix*delta 
//				else if (INPUT_STATE.mouse_delt.x < 0)
//					pos = length(pos)* normalize((glm::vec3(cameraMat[2]) + glm::vec3(cameraMat[0]) * INPUT_STATE.mouse_sensitivity));
//			}
//		}
//		LookAt(glm::vec3(0.f,0.f,0.f));
//		Perspective();
//	}
//private:
//	glm::mat4 cameraMat = glm::mat4(1.0);
//	glm::mat4 viewMat = glm::mat4(1.0);
//	glm::mat4 ProjectionMat = glm::mat4(1.0f);
//public:
//	glm::vec3 pos = glm::vec3(0.0f, 0.f, 0.f);
//	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);
//	float far = 0.f;
//	float near = 0.f;
//	float fov = 0.f;
//	float aspect = 1.f;
//};