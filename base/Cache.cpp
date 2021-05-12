#include "Cache.h"

mesh_t *cache_acquire_mesh(const char *filename)
{
	if (filename == NULL) return NULL;
	cached_mesh_t cached_mesh;
	int num_meshes = g_meshes.size();
	int i;

	for (i = 0; i < num_meshes; i++) {
		if (StrUtil::cstr_equals(g_meshes[i].filename, filename))
		{
			if (g_meshes[i].references > 0) {
				g_meshes[i].references += 1;
			}
			else {
				assert(g_meshes[i].references == 0);
				assert(g_meshes[i].mesh == NULL);
				g_meshes[i].mesh = mesh_load(filename);
				g_meshes[i].references = 1;
			}
			return g_meshes[i].mesh;
		}
	}

	cached_mesh.filename = StrUtil::duplicate_string(filename);
	cached_mesh.mesh = mesh_load(filename);
	cached_mesh.references = 1;
	g_meshes.push_back(cached_mesh);
	return cached_mesh.mesh;
}

void cache_release_mesh(mesh_t *mesh)
{
	if (mesh != NULL) {
		int num_meshes = g_meshes.size();
		int i;
		for (i = 0; i < num_meshes; i++) {
			if (g_meshes[i].mesh == mesh) {
				assert(g_meshes[i].references > 0);
				g_meshes[i].references -= 1;
				if (g_meshes[i].references == 0) {
					mesh_release(g_meshes[i].mesh);
					g_meshes[i].mesh = NULL;
				}
				return;
			}
		}
		assert(0);
	}
}

texture_t *cache_acquire_texture(const char *filename, usage_t usage) {
	if (filename != NULL) {
		cached_texture_t cached_texture;
		int num_textures = g_textures.size();
		int i;

		for (i = 0; i < num_textures; i++) {
			if (strcmp(g_textures[i].filename, filename) == 0) {
				if (g_textures[i].usage == usage) {
					if (g_textures[i].references > 0) {
						g_textures[i].references += 1;
					}
					else {
						assert(g_textures[i].references == 0);
						assert(g_textures[i].texture == NULL);
						g_textures[i].texture = texture_from_file(filename,
							usage);
						g_textures[i].references = 1;
					}
					return g_textures[i].texture;
				}
			}
		}

		cached_texture.filename = StrUtil::duplicate_string(filename);
		cached_texture.usage = usage;
		cached_texture.texture = texture_from_file(filename, usage);
		cached_texture.references = 1;
		g_textures.push_back(cached_texture);
		return cached_texture.texture;
	}
	else {
		return NULL;
	}
}

void cache_release_texture(texture_t *texture) {
	if (texture != NULL) {
		int num_textures = g_textures.size();
		int i;
		for (i = 0; i < num_textures; i++) {
			if (g_textures[i].texture == texture) {
				assert(g_textures[i].references > 0);
				g_textures[i].references -= 1;
				if (g_textures[i].references == 0) {
					texture_release(g_textures[i].texture);
					g_textures[i].texture = NULL;
				}
				return;
			}
		}
		assert(0);
	}
}