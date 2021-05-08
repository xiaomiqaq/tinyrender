#pragma once
#include"../pch.h"

typedef struct {
	vec4_t basecolor;
	float shininess;
	const char *diffuse_map;
	const char *specular_map;
	const char *emission_map;
	/* render settings */
	int double_sided;
	int enable_blend;
	float alpha_cutoff;
} material_t;