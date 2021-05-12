#pragma once
#include "../pch.h"
#include "Shader.h"
#define MAX_VARYINGS 10


typedef vec4_t vertex_shader_t(void *attribs, void *varyings, void *uniforms);
typedef vec4_t fragment_shader_t(void *varyings, void *uniforms,int *discard, int backface);

//struct program 
//{
//	vertex_shader_t *vertex_shader;
//	fragment_shader_t *fragment_shader;
//	int sizeof_attribs;
//	int sizeof_varyings;
//	int sizeof_uniforms;
//	int double_sided;
//	int enable_blend;
//	/* for shaders */
//	void *shader_attribs[3];
//	void *shader_varyings;
//	void *shader_uniforms;
//	/* for clipping */
//	vec4_t in_coords[MAX_VARYINGS];
//	vec4_t out_coords[MAX_VARYINGS];
//	void *in_varyings[MAX_VARYINGS];
//	void *out_varyings[MAX_VARYINGS];
//};
//
//Program *program_create(vertex_shader_t *vertex_shader, fragment_shader_t *fragment_shader,
//					int sizeof_attribs, int sizeof_varyings, int sizeof_uniforms,int double_sided, int enable_blend);
//
//void program_release(Program *program);
//
//void *program_get_attribs(Program *program, int nth_vertex);
//
//void *program_get_uniforms(Program *program);
class Program
{
public:
	static Program *program_create(vertex_shader_t *vertex_shader, fragment_shader_t *fragment_shader,
						int sizeof_attribs, int sizeof_varyings, int sizeof_uniforms, int double_sided, int enable_blend);

	static void program_release(Program *program);

	void *program_get_attribs(int nth_vertex);

	void *program_get_uniforms();
public:
	vertex_shader_t *vertex_shader;
	fragment_shader_t *fragment_shader;
	int sizeof_attribs;
	int sizeof_varyings;
	int sizeof_uniforms;
	int double_sided;
	int enable_blend;
	/* for shaders */
	void *shader_attribs[3];   //position,texcoord,normal
	void *shader_varyings;
	void *shader_uniforms;
	/* for clipping */
	vec4_t in_coords[MAX_VARYINGS];
	vec4_t out_coords[MAX_VARYINGS];
	void *in_varyings[MAX_VARYINGS];
	void *out_varyings[MAX_VARYINGS];
};

