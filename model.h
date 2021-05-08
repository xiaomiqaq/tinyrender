#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "geometry.h"
#include "tgaimage.h"
#include"glm/glm.hpp"

//struct Vertex
//{
//	vec3 pos;
//	vec3 normal;
//	vec2 uv;
//};
using namespace glm;
class Model {
private:
	std::vector<vec3> verts_;     // array of vertices
	std::vector<vec2> uv_;        // array of tex coords
	std::vector<vec3> norms_;     // array of normal vectors
	std::vector<int> facet_vrt_;
	std::vector<int> facet_tex_;  // indices 
	std::vector<int> facet_nrm_;
	TGAImage diffusemap_;         
	TGAImage normalmap_;         
	TGAImage specularmap_;      
	void load_texture(const std::string filename, const std::string suffix, TGAImage &img);
public:
	Model(const std::string filename);
	int nverts() const;
	int nfaces() const;
	vec3 normal(const int iface, const int nthvert) const;  // 顶点法线
	vec3 normal(const vec2 &uv) const;                      // 法线贴图
	vec3 vert(const int i) const;
	vec3 vert(const int iface, const int nthvert) const;
	vec2 uv(const int iface, const int nthvert) const;
	TGAColor diffuse(const vec2 &uv) const;
	double specular(const vec2 &uv) const;
};
#endif //__MODEL_H__

