#pragma once
#include "../pch.h"
#include "Modal.h"
#include "Material.h"
#include "Cache.h"
#include "../Graphics/Draw.h"



typedef struct {
	int index;
	char mesh[LINE_SIZE];
	char skeleton[LINE_SIZE];
	int attached;
	int material;
	int transform;
} scene_model_t;

typedef struct {
	int index;
	vec4_t basecolor;
	float shininess;
	char diffuse_map[LINE_SIZE];
	char specular_map[LINE_SIZE];
	char emission_map[LINE_SIZE];
	char double_sided[LINE_SIZE];
	char enable_blend[LINE_SIZE];
	float alpha_cutoff;
} scene_material_t;

typedef struct {
	int index;
	mat4_t matrix;
} scene_transform_t;


typedef struct {
	vec3_t background;
	char environment[LINE_SIZE];
	char skybox[LINE_SIZE];
	char shadow[LINE_SIZE];
	float ambient;
	float punctual;
} scene_light_t;



class Scene
{
public:
	static Scene *scene_from_file(const char *filename, mat4_t root);
	static scene_light_t read_light(FILE *file);
	static std::vector<scene_material_t> read_blinn_materials(FILE *file);
	static scene_material_t read_blinn_material(FILE *file);
	static std::vector<scene_transform_t> read_transforms(FILE *file);
	static scene_transform_t read_transform(FILE *file);
	static std::vector<scene_model_t> read_models(FILE *file);
	static scene_model_t read_model(FILE *file);
	static Scene *create_blinn_scene(scene_light_t *scene_light,std::vector<scene_material_t> scene_materials,
							std::vector<scene_transform_t> scene_transforms,std::vector<scene_model_t> scene_models,mat4_t root_transform);
	static Scene *create_scene(scene_light_t *light, std::vector<model_t*> models);
	static Scene *scene_create(vec3_t background, model_t *skybox, std::vector<model_t*> models,
							float ambient_intensity, float punctual_intensity,
						int shadow_width, int shadow_height);

	
	void scene_release(Scene *scene);
public:
	vec4_t background;
	model_t *skybox;
	
	std::vector<model_t*> m_models{};
	/* light intensity */
	float ambient_intensity;
	float punctual_intensity;
	/* shadow mapping */
	//framebuffer_t *shadow_buffer;
	//texture_t *shadow_map;
};