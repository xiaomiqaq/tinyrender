#pragma once
#include"Perframe.h"
#include "../base/Scene.h"
static mat4_t get_light_view_matrix(vec3_t light_dir) {
	vec3_t light_pos = vec3_negate(light_dir);
	vec3_t light_target = vec3_new(0, 0, 0);
	vec3_t light_up = vec3_new(0, 1, 0);
	return mat4_lookat(light_pos, light_target, light_up);
}

static mat4_t get_light_proj_matrix(float half_w, float half_h,
	float z_near, float z_far) {
	return mat4_orthographic(half_w, half_h, z_near, z_far);
}
perframe_t build_perframe(Scene *scene, context_t *context)
{
	vec3_t light_dir = vec3_normalize(context->light_dir);
	camera_t *camera = context->camera;
	perframe_t perframe;

	perframe.frame_time = context->frame_time;
	perframe.delta_time = context->delta_time;
	perframe.light_dir = light_dir;
	perframe.camera_pos = camera_get_position(camera);
	perframe.light_view_matrix = get_light_view_matrix(light_dir);
	perframe.light_proj_matrix = get_light_proj_matrix(1, 1, 0, 2);
	perframe.camera_view_matrix = camera_get_view_matrix(camera);
	perframe.camera_proj_matrix = camera_get_proj_matrix(camera);
	perframe.ambient_intensity = scene->ambient_intensity;
	perframe.punctual_intensity = scene->punctual_intensity;
	//perframe.shadow_map = scene->shadow_map;
	perframe.layer_view = -1;

	return perframe;
}
void test_draw_scene(Scene *scene, framebuffer_t *framebuffer, perframe_t *perframe)
{
	model_t *skybox = scene->skybox;
	std::vector<model_t*>models = scene->m_models;
	int num_models = models.size();
	int i;

	for (i = 0; i < num_models; i++) {
		model_t *model = models[i];
		model->update(model, perframe);
	}
	/*
	if (skybox != NULL) {
		skybox->update(skybox, perframe);
	}

	if (scene->shadow_buffer && scene->shadow_map) {
		sort_models(models, perframe->light_view_matrix);
		framebuffer_clear_depth(scene->shadow_buffer, 1);
		for (i = 0; i < num_models; i++) {
			model_t *model = models[i];
			if (model->opaque) {
				model->draw(model, scene->shadow_buffer, 1);
			}
		}
		texture_from_depthbuffer(scene->shadow_map, scene->shadow_buffer);
	}
	*/
	//sort队列是用来渲染透明物体
	//sort_models(models, perframe->camera_view_matrix);
	framebuffer_clear_color(framebuffer, scene->background);
	framebuffer_clear_depth(framebuffer, 1);
	if (skybox == NULL || perframe->layer_view >= 0) {
		for (i = 0; i < num_models; i++) {
			model_t *model = models[i];
			model->draw(model, framebuffer, 0);
		}
	}
	else {
		int num_opaques = 0;
		for (i = 0; i < num_models; i++) {
			model_t *model = models[i];
			if (model->opaque) {
				num_opaques += 1;
			}
			else {
				break;
			}
		}

		for (i = 0; i < num_opaques; i++) {
			model_t *model = models[i];
			model->draw(model, framebuffer, 0);
		}
		skybox->draw(skybox, framebuffer, 0);
		for (i = num_opaques; i < num_models; i++) {
			model_t *model = models[i];
			model->draw(model, framebuffer, 0);
		}
	}
};