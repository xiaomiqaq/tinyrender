#pragma once
#include"../pch.h"

typedef struct {
	vec3_t diffuse;
	vec3_t specular;
	float alpha;
	float shininess;
	vec3_t normal;
	vec3_t emission;
} material_t;

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
} blinn_material_t;