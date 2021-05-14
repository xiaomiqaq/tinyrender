#pragma once
#include "../pch.h"
#include "../App/Record.h"
//struct camera {
//	vec3_t position;
//	vec3_t target;
//	float aspect;
//};
//typedef struct camera Camera;
//typedef struct { vec2_t orbit; vec2_t pan; float dolly; } motion_t;
//
///* camera creating/releasing */
//Camera *camera_create(vec3_t position, vec3_t target, float aspect);
//void camera_release(Camera *camera);
//
///* camera updating */
//void camera_set_transform(Camera *camera, vec3_t position, vec3_t target);
//void camera_update_transform(Camera *camera, motion_t motion);
//
///* property retrieving */
//vec3_t camera_get_position(Camera *camera);
//vec3_t camera_get_forward(Camera *camera);
//mat4_t camera_get_view_matrix(Camera *camera);
//mat4_t camera_get_proj_matrix(Camera *camera);

typedef struct 
{ 
	vec2_t orbit; 
	vec2_t pan; 
	float dolly; 
} motion_t;

class Camera 
{
public:
	/* camera creating/releasing */
	static Camera *camera_create(vec3_t position, vec3_t target, float aspect);
	static void camera_release(Camera *camera);

	/* camera updating */
	void camera_set_transform(vec3_t position, vec3_t target);
	void camera_update_transform(motion_t motion);

	void update_camera(Record *record);

	/* property retrieving */
	vec3_t camera_get_position();
	vec3_t camera_get_forward();
	mat4_t camera_get_view_matrix();
	mat4_t camera_get_proj_matrix();
public:
	vec3_t position;
	vec3_t target;
	float aspect;
};


