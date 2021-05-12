#pragma once
#include"../pch.h"
#include "mesh.h"
#include "Texture.h"
//缓存机制避免mesh,texture等资源被重复加载

typedef struct {
	char *filename;
	mesh_t *mesh;
	int references;
} cached_mesh_t;
typedef struct {
	char *filename;
	usage_t usage;
	texture_t *texture;
	int references;
} cached_texture_t;
static std::vector<cached_mesh_t> g_meshes ;
static std::vector<cached_texture_t> g_textures ;

mesh_t *cache_acquire_mesh(const char *filename);


void cache_release_mesh(mesh_t *mesh);

texture_t *cache_acquire_texture(const char *filename, usage_t usage);

void cache_release_texture(texture_t *texture);