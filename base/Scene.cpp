#pragma once
#include "Scene.h"

Scene *Scene::scene_from_file(const char *filename, mat4_t root) {
	char scene_type[LINE_SIZE];
	Scene *scene = new Scene();
	FILE *file;
	int items;
	file = fopen(filename, "rb");
	assert(file != NULL);
	items = fscanf(file, " type: %s", scene_type);
	assert(items == 1);
	UNUSED_VAR(items);
	if (StrUtil::cstr_equals(scene_type, "blinn")) {
		scene_light_t light = read_light(file);
		std::vector<scene_material_t> scene_materials = read_blinn_materials(file);
		std::vector<scene_transform_t> transforms = read_transforms(file);
		std::vector<scene_model_t> scene_models = read_models(file);
		scene = create_blinn_scene(&light, scene_materials, transforms, scene_models, root);
		
	}
	fclose(file);

	return scene;
};

scene_light_t Scene::read_light(FILE *file) {
	scene_light_t light;
	char header[LINE_SIZE];
	int items;

	items = fscanf(file, " %s", header);
	assert(StrUtil::cstr_equals(header, "lighting:"));
	items = fscanf(file, " background: %f %f %f",
		&light.background.x,
		&light.background.y,
		&light.background.z);
	assert(items == 3);
	items = fscanf(file, " environment: %s", light.environment);
	assert(items == 1);
	items = fscanf(file, " skybox: %s", light.skybox);
	assert(items == 1);
	items = fscanf(file, " shadow: %s", light.shadow);
	assert(items == 1);
	items = fscanf(file, " ambient: %f", &light.ambient);
	assert(items == 1);
	items = fscanf(file, " punctual: %f", &light.punctual);
	assert(items == 1);

	UNUSED_VAR(items);
	return light;
};

std::vector<scene_material_t> Scene::read_blinn_materials(FILE *file) {
	std::vector<scene_material_t> materials ;
	int num_materials;
	int items;
	int i;

	items = fscanf(file, " materials %d:", &num_materials);
	assert(items == 1);
	UNUSED_VAR(items);
	for (i = 0; i < num_materials; i++) {
		scene_material_t material = read_blinn_material(file);
		assert(material.index == i);
		materials.push_back(material);
	}
	return materials;
}

scene_material_t Scene::read_blinn_material(FILE *file) {
	scene_material_t material;
	int items;

	items = fscanf(file, " material %d:", &material.index);
	assert(items == 1);
	items = fscanf(file, " basecolor: %f %f %f %f",
		&material.basecolor.x,
		&material.basecolor.y,
		&material.basecolor.z,
		&material.basecolor.w);
	assert(items == 4);
	items = fscanf(file, " shininess: %f", &material.shininess);
	assert(items == 1);
	items = fscanf(file, " diffuse_map: %s", material.diffuse_map);
	assert(items == 1);
	items = fscanf(file, " specular_map: %s", material.specular_map);
	assert(items == 1);
	items = fscanf(file, " emission_map: %s", material.emission_map);
	assert(items == 1);
	items = fscanf(file, " double_sided: %s", material.double_sided);
	assert(items == 1);
	items = fscanf(file, " enable_blend: %s", material.enable_blend);
	assert(items == 1);
	items = fscanf(file, " alpha_cutoff: %f", &material.alpha_cutoff);
	assert(items == 1);

	UNUSED_VAR(items);
	return material;
}

std::vector<scene_transform_t> Scene::read_transforms(FILE *file) {
	std::vector<scene_transform_t> transforms ;
	int num_transforms;
	int items;
	int i;

	items = fscanf(file, " transforms %d:", &num_transforms);
	assert(items == 1);
	UNUSED_VAR(items);
	for (i = 0; i < num_transforms; i++) {
		scene_transform_t transform = read_transform(file);
		assert(transform.index == i);
		transforms.push_back(transform);
	}
	return transforms;
};

scene_transform_t Scene::read_transform(FILE *file) {
	scene_transform_t transform;
	int items;
	int i;

	items = fscanf(file, " transform %d:", &transform.index);
	assert(items == 1);
	for (i = 0; i < 4; i++) {
		items = fscanf(file, " %f %f %f %f",
			&transform.matrix.m[i][0],
			&transform.matrix.m[i][1],
			&transform.matrix.m[i][2],
			&transform.matrix.m[i][3]);
		assert(items == 4);
	}

	UNUSED_VAR(items);
	return transform;
};

std::vector<scene_model_t> Scene::read_models(FILE *file) {
	std::vector<scene_model_t> scene_models;
	int num_models;
	int items;
	int i;

	items = fscanf(file, " models %d:", &num_models);
	assert(items == 1);
	UNUSED_VAR(items);
	for (i = 0; i < num_models; i++) {
		scene_model_t model = read_model(file);
		assert(model.index == i);
		scene_models.push_back(model);
	}
	return scene_models;
};

scene_model_t Scene::read_model(FILE *file) {
	scene_model_t model;
	int items;

	items = fscanf(file, " model %d:", &model.index);
	assert(items == 1);
	items = fscanf(file, " mesh: %s", model.mesh);
	assert(items == 1);
	items = fscanf(file, " skeleton: %s", model.skeleton);
	assert(items == 1);
	items = fscanf(file, " attached: %d", &model.attached);
	assert(items == 1);
	items = fscanf(file, " material: %d", &model.material);
	assert(items == 1);
	items = fscanf(file, " transform: %d", &model.transform);
	assert(items == 1);

	UNUSED_VAR(items);
	return model;
};

Scene *Scene::create_blinn_scene(scene_light_t *scene_light,std::vector<scene_material_t> scene_materials,
									std::vector<scene_transform_t> scene_transforms, std::vector<scene_model_t> scene_models, mat4_t root_transform) 
{
	int num_materials = scene_materials.size();
	int num_transforms = scene_transforms.size();
	int num_models = scene_models.size();
	std::vector<model_t*> models;
	int i;

	for (i = 0; i < num_models; i++) {
		scene_material_t scene_material;
		scene_transform_t scene_transform;
		scene_model_t scene_model;
		const char *mesh_str;
		const char *skeleton;
		int attached;
		mat4_t transform;
		blinn_material_t material;
		model_t *model;

		scene_model = scene_models[i];
		assert(scene_model.transform < num_transforms);
		assert(scene_model.material < num_materials);
		UNUSED_VAR(num_transforms);
		UNUSED_VAR(num_materials);
		mesh_str = FileUtil::wrap_path(scene_model.mesh);
		skeleton = FileUtil::wrap_path(scene_model.skeleton);
		attached = scene_model.attached;

		scene_transform = scene_transforms[scene_model.transform];
		transform = mat4_mul_mat4(root_transform, scene_transform.matrix);
		scene_material = scene_materials[scene_model.material];

		material.basecolor = scene_material.basecolor;
		material.shininess = scene_material.shininess;
		material.diffuse_map = FileUtil::wrap_path(scene_material.diffuse_map);
		material.specular_map = FileUtil::wrap_path(scene_material.specular_map);
		material.emission_map = FileUtil::wrap_path(scene_material.emission_map);
		material.double_sided = StrUtil::wrap_knob(scene_material.double_sided);
		material.enable_blend = StrUtil::wrap_knob(scene_material.enable_blend);
		material.alpha_cutoff = scene_material.alpha_cutoff;

		model = blinn_create_model(mesh_str, transform, skeleton, attached,
			&material);
		models.push_back(model);

	}

	return create_scene(scene_light, models);
}

Scene *Scene::create_scene(scene_light_t *light, std::vector<model_t*> models) {
	model_t *skybox;
	int shadow_width;
	int shadow_height;

	if (StrUtil::cstr_equals(light->skybox, "off")) {
		skybox = NULL;
	}
	else {
		const char *skybox_name = FileUtil::wrap_path(light->environment);
		int blur_level;
		if (StrUtil::cstr_equals(light->skybox, "ambient")) {
			blur_level = -1;
		}
		else if (StrUtil::cstr_equals(light->skybox, "blurred")) {
			blur_level = 1;
		}
		else {
			assert(StrUtil::cstr_equals(light->skybox, "on"));
			blur_level = 0;
		}
		assert(skybox_name != NULL);
		//skybox = skybox_create_model(skybox_name, blur_level);
	}

	if (StrUtil::cstr_equals(light->shadow, "off")) {
		shadow_width = -1;
		shadow_height = -1;
	}
	else {
		if (StrUtil::cstr_equals(light->shadow, "on")) {
			shadow_width = 512;
			shadow_height = 512;
		}
		else {
			int items;
			items = sscanf(light->shadow, "%dx%d",
				&shadow_width, &shadow_height);
			assert(items == 2 && shadow_width > 0 && shadow_height > 0);
			UNUSED_VAR(items);
		}
	}

	return scene_create(light->background, skybox, models,
		light->ambient, light->punctual,
		shadow_width, shadow_height);
}

Scene *Scene::scene_create(vec3_t background, model_t *skybox, std::vector<model_t*> models,
	float ambient_intensity, float punctual_intensity,
	int shadow_width, int shadow_height) 
{
	Scene *scene = new Scene() ;
	scene->background = vec4_from_vec3(background, 1);
	scene->skybox = skybox;
	scene->m_models = models;
	scene->m_models.push_back(models[0]);

	//scene->models.assign(models.begin(), models.end()-1);
	//scene->models.swap(m_models);
	scene->ambient_intensity = ambient_intensity;
	scene->punctual_intensity = punctual_intensity;
	if (shadow_width > 0 && shadow_height > 0) {
		//scene->shadow_buffer = framebuffer_create(shadow_width, shadow_height);
		//scene->shadow_map = texture_create(shadow_width, shadow_height);
	}
	else {
		//scene->shadow_buffer = NULL;
		//scene->shadow_map = NULL;
	}
	return scene;
}



void Scene::scene_release(Scene *scene) {
	int num_models = scene->m_models.size();
	int i;
	if (scene->skybox) {
		model_t *skybox = scene->skybox;
		skybox->release(skybox);
	}
	for (i = 0; i < num_models; i++) {
		model_t *model = scene->m_models[i];
		model->release(model);
	}
	scene->m_models.clear();
	//
	/*if (scene->shadow_buffer) {
		framebuffer_release(scene->shadow_buffer);
	}
	if (scene->shadow_map) {
		texture_release(scene->shadow_map);
	}*/
	free(scene);
};


