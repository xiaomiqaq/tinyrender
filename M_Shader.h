//#pragma once
//#include<glm/glm.hpp>
//#include "tgaimage.h"
//#include "glFunction.h"
//#include <cmath>
//#include "model.h"
//#include "FrameBuffer.h"
//vec3 light_pos(2.0, 2.0, 1.0);
//vec3 light_target(0.0, 0.0, 0.0);
//vec3 eye(1.0f, 0.0f, 1.0f);
//vec3 center(0.0f, 0.0f, 0.0f);
//
//class IShader {
//public:
//	Model* model;
//	Camera* camera;
//
//	glm::mat4 ViewPort = glm::mat4(1.0f);
//
//	void viewPort(int width, int height, int depth)
//	{
//		ViewPort[0] = vec4(width / 2.0f, 0.0f, 0.0f, 0.0f);
//		ViewPort[1] = vec4(0.0f, -height / 2.0f, 0.0f, 0.0f);
//		ViewPort[2] = vec4(0.0f, 0.0f, depth / 2.0f, 0.0f);
//		ViewPort[3] = vec4(width / 2.0f, height / 2.0f, depth / 2.0f, 1);
//	}
//public:
//	virtual glm::vec3 vertex(int iface, int nthvert) = 0;
//	virtual bool fragment(glm::vec3 bar, TGAColor &color) = 0;
//
//};
//
////class DepthShader : public IShader
////{
////private:
////	vec3 v2f_pos[3];
////	vec3 ndc_pos[3];
////
////public:
////	mat4 ShadowMapMat;
////	DepthShader(Model* mo)
////	{
////		model = mo;
////		lookAt(light_pos, light_target, vec3(0.0, 1.0, 0.0));
////		//projection  不需要
////		viewPort(width, height, depth);
////
////		ShadowMapMat = ViewPort * Projection * View;
////	}
////	virtual vec4 vertex(int iface, int i)
////	{
////		v2f_pos[i] = model->vert(iface, i);
////		vec4 glVertex = ShadowMapMat * vec4(v2f_pos[i], 1.0);
////		glVertex = glVertex / glVertex.w;
////		ndc_pos[i] = glVertex;
////		return glVertex;
////	}
////	virtual bool fragment(vec3 bar, TGAColor& color)
////	{
////		vec3 pos(0.0);
////		for (int i = 0; i < 3; i++)
////		{
////			pos += ndc_pos[i] * bar[i];
////		}
////		color = TGAColor(255, 255, 255)*(pos.z / depth);
////		return true;
////	}
////};
//
////class ShadowShader : public IShader {
////
////private:
////	glm::vec3 v2f_pos[3];
////	glm::vec3 v2f_normal[3];
////	glm::vec2 v2f_uv[3];
////	glm::vec3 ndc_ver[3];
////	TGAColor ambient = TGAColor(5, 5, 5, 255);
////	TGAColor specColor = TGAColor(100, 100, 100, 1);
////public:
////	mat4 ShadowMat;
////	ShadowShader(Model* mo,Camera* cam) 
////	{
////		model = mo;
////		camera = cam;
////	}
////	virtual glm::vec3 vertex(int iface, int index)
////	{
////		
////		v2f_uv[index] = model->uv(iface, index);
////		v2f_pos[index] = model->vert(iface, index);
////		v2f_normal[index] = model->normal(iface, index);
////		glm::vec4 gl_Vertex = vec4(model->vert(iface, index), 1.0f); // read the vertex from .obj file
////		gl_Vertex = ViewPort * camera->Projection() * camera->View() * gl_Vertex;
////		ndc_ver[index] = gl_Vertex / gl_Vertex.w;
////		return  ndc_ver[index]; // 屏幕空间坐标
////	}
////	glm::vec3* vertex(int iface)
////	{
////		for (int i = 0; i < 3; i++)
////		{
////			v2f_uv[i] = model->uv(iface, i);
////			v2f_pos[i] = model->vert(iface, i);
////			v2f_normal[i] = model->normal(iface, i);
////			glm::vec4 gl_Vertex = vec4(model->vert(iface, i), 1.0f); // read the vertex from .obj file
////			gl_Vertex = ViewPort * camera->Projection() * camera->View() * gl_Vertex;
////			ndc_ver[i] = gl_Vertex / gl_Vertex.w;
////		}
////		return  ndc_ver; // 屏幕空间坐标
////	}
////
////	virtual bool fragment(vec3 bar, TGAColor &color)
////	{
////		vec2 uv(0.0);
////		vec3 pos(0.0);
////		vec3 normal(0.0);
////		mat3 BTN;  //TANGENT TO MODEL
////		for (int i = 0; i < 3; i++) {
////			uv += v2f_uv[i] * bar[i];
////			pos += v2f_pos[i] * bar[i];
////			normal += v2f_normal[i] * bar[i];//根据顶点的法线计算法向量，是为了构建A矩阵
////		}
////
////		//caculateTangent(v2f_pos, v2f_uv);
////		mat3 A;
////		A[0] = v2f_pos[1] - v2f_pos[0];//向量AC;
////		A[1] = v2f_pos[2] - v2f_pos[0];
////		A[2] = normal;
////		A = transpose(A);
////		A = inverse(A);
////
////		vec3 tangent = normalize(A * vec3(v2f_uv[1].x - v2f_uv[0].x, v2f_uv[2].x - v2f_uv[0].x, 0.0)); //u的梯度
////		vec3 binTangent = normalize(A * vec3(v2f_uv[1].y - v2f_uv[0].y, v2f_uv[2].y - v2f_uv[0].y, 0.0)); //v的梯度
////		BTN[0] = tangent;
////		BTN[1] = binTangent;
////		BTN[2] = normal;
////
////		//shadow
////		/*vec4 shadowP = ShadowMat * vec4(pos, 1.0);
////		shadowP = shadowP / shadowP.w;
////
////		float shadow = 0.3 + 0.7*(shadowBuffer[int(shadowP.x) + int(shadowP.y*width)] < shadowP.z + 54.34);*/
////		vec3 worldViewDir = normalize(eye - center);
////		vec3 worldNormal = normalize(BTN * model->normal(uv));
////		vec3 worldLightDir = normalize(light_pos - light_target);
////		vec3 worldReflect = 2.0f*worldNormal*dot(worldNormal, worldLightDir) - worldLightDir;
////		float diff = dot(worldNormal, worldLightDir);
////		float spec =std::pow(std::max(dot(worldReflect, worldViewDir), 0.0f), model->specular(uv));
////
////		//color = ambient + (model->diffuse(uv).toRGB() * diff + specColor * spec * 60.0)*shadow;
////		color = ambient + model->diffuse(uv).toRGB() * diff + specColor * spec ;
////		return true;
////	}
////
////};
//class PhoneShader : public IShader {
//
//private:
//	glm::vec3 v2f_pos[3];
//	glm::vec3 v2f_normal[3];
//	glm::vec2 v2f_uv[3];
//	glm::vec3 ndc_ver[3];
//	TGAColor ambient = TGAColor(5, 5, 5, 255);
//	TGAColor specColor = TGAColor(100, 100, 100, 1);
//public:
//	mat4 ShadowMat;
//	PhoneShader(Model* mo, Camera* cam)
//	{
//		model = mo;
//		camera = cam;
//	}
//	virtual glm::vec3 vertex(int iface, int index)
//	{
//
//		v2f_uv[index] = model->uv(iface, index);
//		v2f_pos[index] = model->vert(iface, index);
//		v2f_normal[index] = model->normal(iface, index);
//		glm::vec4 gl_Vertex = vec4(model->vert(iface, index), 1.0f); // read the vertex from .obj file
//		gl_Vertex = ViewPort * camera->Projection() * camera->View() * gl_Vertex;
//		ndc_ver[index] = gl_Vertex / gl_Vertex.w;
//		return  ndc_ver[index]; // 屏幕空间坐标
//	}
//	
//
//	virtual bool fragment(vec3 bar, TGAColor &color)
//	{
//		vec2 uv(0.0);
//		vec3 pos(0.0);
//		vec3 normal(0.0);
//		mat3 BTN;  //TANGENT TO MODEL
//		for (int i = 0; i < 3; i++) {
//			uv += v2f_uv[i] * bar[i];
//			pos += v2f_pos[i] * bar[i];
//			normal += v2f_normal[i] * bar[i];//根据顶点的法线计算法向量，是为了构建A矩阵
//		}
//
//		//caculateTangent(v2f_pos, v2f_uv);
//		mat3 A;
//		A[0] = v2f_pos[1] - v2f_pos[0];//向量AC;
//		A[1] = v2f_pos[2] - v2f_pos[0];
//		A[2] = normal;
//		A = transpose(A);
//		A = inverse(A);
//
//		vec3 tangent = normalize(A * vec3(v2f_uv[1].x - v2f_uv[0].x, v2f_uv[2].x - v2f_uv[0].x, 0.0)); //u的梯度
//		vec3 binTangent = normalize(A * vec3(v2f_uv[1].y - v2f_uv[0].y, v2f_uv[2].y - v2f_uv[0].y, 0.0)); //v的梯度
//		BTN[0] = tangent;
//		BTN[1] = binTangent;
//		BTN[2] = normal;
//
//		//shadow
//		/*vec4 shadowP = ShadowMat * vec4(pos, 1.0);
//		shadowP = shadowP / shadowP.w;
//
//		float shadow = 0.3 + 0.7*(shadowBuffer[int(shadowP.x) + int(shadowP.y*width)] < shadowP.z + 54.34);*/
//		vec3 worldViewDir = normalize(eye - center);
//		vec3 worldNormal = normalize(BTN * model->normal(uv));
//		vec3 worldLightDir = normalize(light_pos - light_target);
//		vec3 worldReflect = 2.0f*worldNormal*dot(worldNormal, worldLightDir) - worldLightDir;
//		float diff = dot(worldNormal, worldLightDir);
//		float spec = std::pow(std::max(dot(worldReflect, worldViewDir), 0.0f), model->specular(uv));
//
//		//color = ambient + (model->diffuse(uv).toRGB() * diff + specColor * spec * 60.0)*shadow;
//		color = ambient + model->diffuse(uv).toRGB() * diff + specColor * spec;
//		return true;
//	}
//
//};
