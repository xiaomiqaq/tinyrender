#pragma once
#include "../pch.h"
#include "../base/Texture.h"
#include "../base/Camera.h"
//#include "../base/Scene.h"
class Scene;
typedef struct {
	float frame_time;
	float delta_time;
	vec3_t light_dir;
	vec3_t camera_pos;
	mat4_t light_view_matrix;
	mat4_t light_proj_matrix;
	mat4_t camera_view_matrix;
	mat4_t camera_proj_matrix;
	float ambient_intensity;
	float punctual_intensity;
	texture_t *shadow_map;
	int layer_view;
} perframe_t;

typedef struct {
	framebuffer_t *framebuffer;
	camera_t *camera;
	vec3_t light_dir;
	vec2_t click_pos;
	int single_click;
	int double_click;
	float frame_time;
	float delta_time;
} context_t;

perframe_t build_perframe(Scene *scene, context_t *context);

void test_draw_scene(Scene* scene, framebuffer_t *framebuffer,perframe_t *perframe);