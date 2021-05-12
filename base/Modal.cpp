#include "Modal.h"
void draw_model(model_t *model, framebuffer_t *framebuffer, int shadow_pass)
{
	mesh_t *mesh = model->mesh;
	int num_faces = mesh_get_num_faces(mesh);
	vertex_t *vertices = mesh_get_vertices(mesh);
	program_t *program = model->program;
	blinn_uniforms_t *uniforms;
	blinn_attribs_t *attribs;
	int i, j;

	uniforms = (blinn_uniforms_t*)program_get_uniforms(model->program);
	uniforms->shadow_pass = shadow_pass;
	for (i = 0; i < num_faces; i++) {
		for (j = 0; j < 3; j++) {
			vertex_t vertex = vertices[i * 3 + j];
			//将Mesh的vertics传给program
			attribs = (blinn_attribs_t*)program_get_attribs(program, j);
			attribs->position = vertex.position;
			attribs->texcoord = vertex.texcoord;
			attribs->normal = vertex.normal;
			attribs->joint = vertex.joint;
			attribs->weight = vertex.weight;
		}
		graphics_draw_triangle(framebuffer, program);
	}
};
model_t *blinn_create_model(const char *mesh, mat4_t transform,
	const char *skeleton, int attached, blinn_material_t *material)
{
	int sizeof_attribs = sizeof(blinn_attribs_t);
	int sizeof_varyings = sizeof(blinn_varyings_t);
	int sizeof_uniforms = sizeof(blinn_uniforms_t);
	blinn_uniforms_t *uniforms;
	program_t *program;
	model_t *model;

	program = program_create(blinn_vertex_shader, blinn_fragment_shader,
		sizeof_attribs, sizeof_varyings, sizeof_uniforms,
		material->double_sided, material->enable_blend);

	uniforms = (blinn_uniforms_t*)program_get_uniforms(program);
	uniforms->basecolor = material->basecolor;
	uniforms->shininess = material->shininess;
	uniforms->diffuse_map = acquire_color_texture(material->diffuse_map);
	uniforms->specular_map = acquire_color_texture(material->specular_map);
	uniforms->emission_map = acquire_color_texture(material->emission_map);
	uniforms->alpha_cutoff = material->alpha_cutoff;

	model = (model_t*)malloc(sizeof(model_t));
	model->mesh = cache_acquire_mesh(mesh);
	model->program = program;
	model->transform = transform;
	//model->skeleton = cache_acquire_skeleton(skeleton);
	model->attached = attached;
	model->opaque = !material->enable_blend;
	model->distance = 0;
	model->update = update_model;
	model->draw = draw_model;
	//model->release = release_model;

	return model;
}
static int compare_models(const void *model1p, const void *model2p) {
	model_t *model1 = *(model_t**)model1p;
	model_t *model2 = *(model_t**)model2p;

	if (model1->opaque && model2->opaque) {
		return model1->distance < model2->distance ? -1 : 1;
	}
	else if (model1->opaque && !model2->opaque) {
		return -1;
	}
	else if (!model1->opaque && model2->opaque) {
		return 1;
	}
	else {
		return model1->distance < model2->distance ? 1 : -1;
	}
}
static void sort_models(model_t **models, mat4_t view_matrix) {
	int num_models = darray_size(models);
	int i;
	if (num_models > 1) {
		for (i = 0; i < num_models; i++) {
			model_t *model = models[i];
			vec3_t center = mesh_get_center(model->mesh);
			vec4_t local_pos = vec4_from_vec3(center, 1);
			vec4_t world_pos = mat4_mul_vec4(model->transform, local_pos);
			vec4_t view_pos = mat4_mul_vec4(view_matrix, world_pos);
			model->distance = -view_pos.z;
		}
		qsort(models, num_models, sizeof(model_t*), compare_models);
	}
}
static void update_model(model_t *model, perframe_t *perframe) {
	float ambient_intensity = perframe->ambient_intensity;
	float punctual_intensity = perframe->punctual_intensity;
	//skeleton_t *skeleton = model->skeleton;
	mat4_t model_matrix = model->transform;
	mat3_t normal_matrix;
	mat4_t *joint_matrices;
	mat3_t *joint_n_matrices;
	blinn_uniforms_t *uniforms;

	/*if (skeleton) {
		skeleton_update_joints(skeleton, perframe->frame_time);
		joint_matrices = skeleton_get_joint_matrices(skeleton);
		joint_n_matrices = skeleton_get_normal_matrices(skeleton);
		if (model->attached >= 0) {
			mat4_t node_matrix = joint_matrices[model->attached];
			model_matrix = mat4_mul_mat4(model_matrix, node_matrix);
			joint_matrices = NULL;
			joint_n_matrices = NULL;
		}
	}
	else*/ {
		joint_matrices = NULL;
		joint_n_matrices = NULL;
	}
	normal_matrix = mat3_inverse_transpose(mat3_from_mat4(model_matrix));

	uniforms = (blinn_uniforms_t*)program_get_uniforms(model->program);
	uniforms->light_dir = perframe->light_dir;
	uniforms->camera_pos = perframe->camera_pos;
	uniforms->model_matrix = model_matrix;
	uniforms->normal_matrix = normal_matrix;
	uniforms->light_vp_matrix = mat4_mul_mat4(perframe->light_proj_matrix,
		perframe->light_view_matrix);
	uniforms->camera_vp_matrix = mat4_mul_mat4(perframe->camera_proj_matrix,
		perframe->camera_view_matrix);
	uniforms->joint_matrices = joint_matrices;
	uniforms->joint_n_matrices = joint_n_matrices;
	uniforms->ambient_intensity = float_clamp(ambient_intensity, 0, 5);
	uniforms->punctual_intensity = float_clamp(punctual_intensity, 0, 5);
	uniforms->shadow_map = perframe->shadow_map;
}