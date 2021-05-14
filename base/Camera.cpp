#pragma once
#include "Camera.h"
static const float NEAR = 0.1f;
static const float FAR = 10000;
static const float FOVY = TO_RADIANS(60);
static const vec3_t UP = { 0, 1, 0 };

static vec3_t calculate_pan(vec3_t from_camera, motion_t motion) {
	vec3_t forward = vec3_normalize(from_camera);
	vec3_t left = vec3_cross(UP, forward);
	vec3_t up = vec3_cross(forward, left);

	float distance = vec3_length(from_camera);
	float factor = distance * (float)tan(FOVY / 2) * 2;
	vec3_t delta_x = vec3_mul(left, motion.pan.x * factor);
	vec3_t delta_y = vec3_mul(up, motion.pan.y * factor);
	return vec3_add(delta_x, delta_y);
}

static vec3_t calculate_offset(vec3_t from_target, motion_t motion) {
	float radius = vec3_length(from_target);
	float theta = (float)atan2(from_target.x, from_target.z);  /* azimuth */
	float phi = (float)acos(from_target.y / radius);           /* polar */
	float factor = PI * 2;
	vec3_t offset;

	radius *= (float)pow(0.95, motion.dolly);
	theta -= motion.orbit.x * factor;
	phi -= motion.orbit.y * factor;
	phi = float_clamp(phi, EPSILON, PI - EPSILON);

	offset.x = radius * (float)sin(phi) * (float)sin(theta);
	offset.y = radius * (float)cos(phi);
	offset.z = radius * (float)sin(phi) * (float)cos(theta);

	return offset;
}
/* camera creating/releasing */

Camera *Camera::camera_create(vec3_t position, vec3_t target, float aspect) {
	Camera *camera;

	assert(vec3_length(vec3_sub(position, target)) > EPSILON && aspect > 0);

	camera = (Camera*)malloc(sizeof(Camera));
	camera->position = position;
	camera->target = target;
	camera->aspect = aspect;

	return camera;
}
void Camera::update_camera(Record *record)
{
	motion_t motion;
	motion.orbit = record->orbit_delta;
	motion.pan = record->pan_delta;
	motion.dolly = record->dolly_delta;
	camera_update_transform(motion);
}
void Camera::camera_release(Camera *camera) {
	free(camera);
}

/* camera updating */

void Camera::camera_set_transform(vec3_t position, vec3_t target) {
	assert(vec3_length(vec3_sub(position, target)) > EPSILON);
	this->position = position;
	this->target = target;
}



void Camera::camera_update_transform(motion_t motion) 
{
	vec3_t from_target = vec3_sub(this->position, this->target);
	vec3_t from_camera = vec3_sub(this->target, this->position);
	vec3_t pan = calculate_pan(from_camera, motion);
	vec3_t offset = calculate_offset(from_target, motion);
	this->target = vec3_add(this->target, pan);
	this->position = vec3_add(this->target, offset);
}

/* property retrieving */

vec3_t Camera::camera_get_position() {
	return position;
}

vec3_t Camera::camera_get_forward() {
	return vec3_normalize(vec3_sub(target,position));
}

mat4_t Camera::camera_get_view_matrix() {
	return mat4_lookat(position, target, UP);
}

mat4_t Camera::camera_get_proj_matrix() {
	return mat4_perspective(FOVY, aspect, NEAR, FAR);
}
