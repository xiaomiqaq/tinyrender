#include "Shader.h"

mat4_t get_model_matrix(blinn_attribs_t *attribs,blinn_uniforms_t *uniforms) {
	if (uniforms->joint_matrices) {
		mat4_t joint_matrices[4];
		mat4_t skin_matrix;

		joint_matrices[0] = uniforms->joint_matrices[(int)attribs->joint.x];
		joint_matrices[1] = uniforms->joint_matrices[(int)attribs->joint.y];
		joint_matrices[2] = uniforms->joint_matrices[(int)attribs->joint.z];
		joint_matrices[3] = uniforms->joint_matrices[(int)attribs->joint.w];

		skin_matrix = mat4_combine(joint_matrices, attribs->weight);
		return mat4_mul_mat4(uniforms->model_matrix, skin_matrix);
	}
	else {
		return uniforms->model_matrix;
	}
}
mat3_t get_normal_matrix(blinn_attribs_t *attribs,blinn_uniforms_t *uniforms) {
	if (uniforms->joint_n_matrices) {
		mat3_t joint_n_matrices[4];
		mat3_t skin_n_matrix;

		joint_n_matrices[0] = uniforms->joint_n_matrices[(int)attribs->joint.x];
		joint_n_matrices[1] = uniforms->joint_n_matrices[(int)attribs->joint.y];
		joint_n_matrices[2] = uniforms->joint_n_matrices[(int)attribs->joint.z];
		joint_n_matrices[3] = uniforms->joint_n_matrices[(int)attribs->joint.w];

		skin_n_matrix = mat3_combine(joint_n_matrices, attribs->weight);
		return mat3_mul_mat3(uniforms->normal_matrix, skin_n_matrix);
	}
	else {
		return uniforms->normal_matrix;
	}
}
material_t get_material(blinn_varyings_t *varyings, blinn_uniforms_t *uniforms, int backface)
{
	vec2_t texcoord = varyings->texcoord;
	vec3_t diffuse, specular, normal, emission;
	float alpha, shininess;
	material_t material;

	diffuse = vec3_from_vec4(uniforms->basecolor);
	alpha = uniforms->basecolor.w;
	if (uniforms->diffuse_map) {
		vec4_t sample = texture_sample(uniforms->diffuse_map, texcoord);
		diffuse = vec3_modulate(diffuse, vec3_from_vec4(sample));
		alpha *= sample.w;
	}

	specular = vec3_new(0, 0, 0);
	if (uniforms->specular_map) {
		vec4_t sample = texture_sample(uniforms->specular_map, texcoord);
		specular = vec3_from_vec4(sample);
	}
	shininess = uniforms->shininess;

	normal = vec3_normalize(varyings->normal);
	if (backface) {
		normal = vec3_negate(normal);
	}

	emission = vec3_new(0, 0, 0);
	if (uniforms->emission_map) {
		vec4_t sample = texture_sample(uniforms->emission_map, texcoord);
		emission = vec3_from_vec4(sample);
	}

	material.diffuse = diffuse;
	material.specular = specular;
	material.alpha = alpha;
	material.shininess = shininess;
	material.normal = normal;
	material.emission = emission;
	return material;
}
vec3_t get_view_dir(blinn_varyings_t *varyings,blinn_uniforms_t *uniforms) {
	vec3_t camera_pos = uniforms->camera_pos;
	vec3_t world_pos = varyings->world_position;
	return vec3_normalize(vec3_sub(camera_pos, world_pos));
}

int is_in_shadow(blinn_varyings_t *varyings, blinn_uniforms_t *uniforms, float n_dot_l) 
 {
	 return 0;
	if (uniforms->shadow_map) {
		float u = (varyings->depth_position.x + 1) * 0.5f;
		float v = (varyings->depth_position.y + 1) * 0.5f;
		float d = (varyings->depth_position.z + 1) * 0.5f;

		float depth_bias = float_max(0.05f * (1 - n_dot_l), 0.005f);
		float current_depth = d - depth_bias;
		vec2_t texcoord = vec2_new(u, v);
		float closest_depth = texture_sample(uniforms->shadow_map, texcoord).x;

		return current_depth > closest_depth;
	}
	else {
		return 0;
	}
}
int is_zero_vector(vec3_t v) {
	return v.x == 0 && v.y == 0 && v.z == 0;
}
vec3_t get_specular(vec3_t light_dir, vec3_t view_dir,material_t material) {
	if (!is_zero_vector(material.specular)) {
		vec3_t half_dir = vec3_normalize(vec3_add(light_dir, view_dir));
		float n_dot_h = vec3_dot(material.normal, half_dir);
		if (n_dot_h > 0) {
			float strength = (float)pow(n_dot_h, material.shininess);
			return vec3_mul(material.specular, strength);
		}
	}
	return vec3_new(0, 0, 0);
}

vec4_t common_vertex_shader(blinn_attribs_t *attribs, blinn_varyings_t *varyings, blinn_uniforms_t *uniforms) 
{
	mat4_t model_matrix = get_model_matrix(attribs, uniforms);
	mat3_t normal_matrix = get_normal_matrix(attribs, uniforms);
	mat4_t camera_vp_matrix = uniforms->camera_vp_matrix;
	mat4_t light_vp_matrix = uniforms->light_vp_matrix;

	vec4_t input_position = vec4_from_vec3(attribs->position, 1);
	vec4_t world_position = mat4_mul_vec4(model_matrix, input_position);
	vec4_t clip_position = mat4_mul_vec4(camera_vp_matrix, world_position);
	vec4_t depth_position = mat4_mul_vec4(light_vp_matrix, world_position);

	vec3_t input_normal = attribs->normal;
	vec3_t world_normal = mat3_mul_vec3(normal_matrix, input_normal);

	varyings->world_position = vec3_from_vec4(world_position);
	varyings->depth_position = vec3_from_vec4(depth_position);
	varyings->texcoord = attribs->texcoord;
	varyings->normal = vec3_normalize(world_normal);
	return clip_position;
}
vec4_t common_fragment_shader(blinn_varyings_t *varyings, blinn_uniforms_t *uniforms, int *discard, int backface) 
 {
	material_t material = get_material(varyings, uniforms, backface);
	if (uniforms->alpha_cutoff > 0 && material.alpha < uniforms->alpha_cutoff) {
		*discard = 1;
		return vec4_new(0, 0, 0, 0);
	}
	else {
		vec3_t color = material.emission;

		if (uniforms->ambient_intensity > 0) {
			vec3_t ambient = material.diffuse;
			float intensity = uniforms->ambient_intensity;
			color = vec3_add(color, vec3_mul(ambient, intensity));
		}

		if (uniforms->punctual_intensity > 0) {
			vec3_t light_dir = vec3_negate(uniforms->light_dir);
			float n_dot_l = vec3_dot(material.normal, light_dir);
			if (n_dot_l > 0 && !is_in_shadow(varyings, uniforms, n_dot_l)) {
				vec3_t view_dir = get_view_dir(varyings, uniforms);
				vec3_t specular = get_specular(light_dir, view_dir, material);
				vec3_t diffuse = vec3_mul(material.diffuse, n_dot_l);
				vec3_t punctual = vec3_add(diffuse, specular);
				float intensity = uniforms->punctual_intensity;
				color = vec3_add(color, vec3_mul(punctual, intensity));
			}
		}

		return vec4_from_vec3(color, material.alpha);
	}
}
vec4_t blinn_vertex_shader(void *attribs_, void *varyings_, void *uniforms_) {
	blinn_attribs_t *attribs = (blinn_attribs_t*)attribs_;
	blinn_varyings_t *varyings = (blinn_varyings_t*)varyings_;
	blinn_uniforms_t *uniforms = (blinn_uniforms_t*)uniforms_;

	/*if (uniforms->shadow_pass) {
		return shadow_vertex_shader(attribs, varyings, uniforms);
	}
	else {
		return common_vertex_shader(attribs, varyings, uniforms);
	}*/
	return common_vertex_shader(attribs, varyings, uniforms);
}
vec4_t blinn_fragment_shader(void *varyings_, void *uniforms_,int *discard, int backface) {
	blinn_varyings_t *varyings = (blinn_varyings_t*)varyings_;
	blinn_uniforms_t *uniforms = (blinn_uniforms_t*)uniforms_;

	/*if (uniforms->shadow_pass) {
		return shadow_fragment_shader(varyings, uniforms, discard);
	}
	else {

	}*/
	return common_fragment_shader(varyings, uniforms, discard, backface);
}

texture_t *acquire_color_texture(const char *filename) {
	return cache_acquire_texture(filename, USAGE_LDR_COLOR);
};