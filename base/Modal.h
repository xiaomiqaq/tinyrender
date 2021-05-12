#pragma once
#include "../pch.h"
#include "../Graphics/Program.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Perframe.h"
#include "mesh.h"

typedef struct model {
	mesh_t *mesh;
	program_t *program;
	mat4_t transform;
	/* for animation */
	//skeleton_t *skeleton;
	int attached;
	/* for sorting */
	int opaque;
	float distance;
	/* polymorphism */
	void(*update)(struct model *model, perframe_t *perframe);
	void(*draw)(struct model *model, framebuffer_t *framebuffer , int shadow_pass);
	void(*release)(struct model *model);
} model_t;
void draw_model(model_t *model, framebuffer_t *framebuffer, int shadow_pass);
void update_model(model_t *model, perframe_t *perframe);
model_t *blinn_create_model(const char *mesh, mat4_t transform, const char *skeleton, int attached, blinn_material_t *material);
static int compare_models(const void *model1p, const void *model2p);
static void sort_models(model_t **models, mat4_t view_matrix);