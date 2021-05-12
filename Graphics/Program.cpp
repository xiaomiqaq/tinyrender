#include "Program.h"

Program *Program::program_create(vertex_shader_t *vertex_shader, fragment_shader_t *fragment_shader,
						int sizeof_attribs, int sizeof_varyings, int sizeof_uniforms,int double_sided, int enable_blend) 
{
	Program *program;
	int i;

	assert(sizeof_attribs > 0 && sizeof_varyings > 0 && sizeof_uniforms > 0);
	assert(sizeof_varyings % sizeof(float) == 0);

	program = new Program();

	program->vertex_shader = vertex_shader;
	program->fragment_shader = fragment_shader;
	program->sizeof_attribs = sizeof_attribs;
	program->sizeof_varyings = sizeof_varyings;
	program->sizeof_uniforms = sizeof_uniforms;
	program->double_sided = double_sided;
	program->enable_blend = enable_blend;

	for (i = 0; i < 3; i++) {
		program->shader_attribs[i] = malloc(sizeof_attribs);
		memset(program->shader_attribs[i], 0, sizeof_attribs);
	}
	program->shader_varyings = malloc(sizeof_varyings);
	memset(program->shader_varyings, 0, sizeof_varyings);
	program->shader_uniforms = malloc(sizeof_uniforms);
	memset(program->shader_uniforms, 0, sizeof_uniforms);
	for (i = 0; i < MAX_VARYINGS; i++) {
		program->in_varyings[i] = malloc(sizeof_varyings);
		memset(program->in_varyings[i], 0, sizeof_varyings);
		program->out_varyings[i] = malloc(sizeof_varyings);
		memset(program->out_varyings[i], 0, sizeof_varyings);
	}

	return program;
}

void Program::program_release(Program *program) {
	int i;
	for (i = 0; i < 3; i++) {
		free(program->shader_attribs[i]);
	}
	free(program->shader_varyings);
	free(program->shader_uniforms);
	for (i = 0; i < MAX_VARYINGS; i++) {
		free(program->in_varyings[i]);
		free(program->out_varyings[i]);
	}
	free(program);
}

void *Program::program_get_attribs(int nth_vertex) {
	assert(nth_vertex >= 0 && nth_vertex < 3);
	return program->shader_attribs[nth_vertex];
}

void *Program::program_get_uniforms() {
	return program->shader_uniforms;
}