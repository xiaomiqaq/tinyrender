#pragma once
#include "../pch.h"
#include "../base/Texture.h"
#include "../base/Cache.h"
#include "../base/framebuffer.h"
#include "../base/Material.h"
typedef struct {
	vec3_t world_position;
	vec3_t depth_position;
	vec2_t texcoord;
	vec3_t normal;
} blinn_varyings_t;

typedef struct {
	vec3_t position;
	vec2_t texcoord;
	vec3_t normal;
	vec4_t joint;
	vec4_t weight;
} blinn_attribs_t;

typedef struct {
	vec3_t light_dir;
	vec3_t camera_pos;
	mat4_t model_matrix;
	mat3_t normal_matrix;
	mat4_t light_vp_matrix;
	mat4_t camera_vp_matrix;
	mat4_t *joint_matrices;
	mat3_t *joint_n_matrices;
	float ambient_intensity;
	float punctual_intensity;
	texture_t *shadow_map;
	/* surface parameters */
	vec4_t basecolor;
	float shininess;
	texture_t *diffuse_map;
	texture_t *specular_map;
	texture_t *emission_map;
	/* render controls */
	float alpha_cutoff;
	int shadow_pass;
} blinn_uniforms_t;


static mat4_t get_model_matrix(blinn_attribs_t *attribs, blinn_uniforms_t *uniforms);
static mat3_t get_normal_matrix(blinn_attribs_t *attribs,blinn_uniforms_t *uniforms);
static material_t get_material(blinn_varyings_t *varyings, blinn_uniforms_t *uniforms, int backface);

static vec3_t get_view_dir(blinn_varyings_t *varyings,blinn_uniforms_t *uniforms);
static int is_in_shadow(blinn_varyings_t *varyings,blinn_uniforms_t *uniforms,float n_dot_l);
static int is_zero_vector(vec3_t v);
static vec3_t get_specular(vec3_t light_dir, vec3_t view_dir,material_t material);


static vec4_t common_vertex_shader(blinn_attribs_t *attribs,blinn_varyings_t *varyings,blinn_uniforms_t *uniforms);
static vec4_t common_fragment_shader(blinn_varyings_t *varyings,blinn_uniforms_t *uniforms,int *discard,int backface);
vec4_t blinn_vertex_shader(void *attribs_, void *varyings_, void *uniforms_);
vec4_t blinn_fragment_shader(void *varyings_, void *uniforms_,int *discard, int backface);
texture_t *acquire_color_texture(const char *filename);
