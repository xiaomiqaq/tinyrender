#pragma once
#include<glm/glm.hpp>
#include"tgaimage.h"

class IShader {
public:
	virtual glm::vec3 vertex(int iface, int nthvert) = 0;
	virtual bool fragment(glm::vec3 bar, TGAColor &color) = 0;
};
