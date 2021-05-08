#pragma once
#include "../pch.h"
#include "mesh.h"
typedef struct model {
	mesh_t *mesh;
	//program_t *program;
	mat4_t transform;
	/* for animation */
	//skeleton_t *skeleton;
	int attached;
	/* for sorting */
	int opaque;
	float distance;
	/* polymorphism */
	//void(*update)(struct model *model, perframe_t *perframe);
	//void(*draw)(struct model *model, framebuffer_t *framebuffer,
	//	int shadow_pass);
	void(*release)(struct model *model);
} model_t;