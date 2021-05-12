#pragma once
#include"glm/glm.hpp";
using namespace glm;
struct InputState {
	vec2 mouse_pos = vec2(0.f, 0.f);
	vec2 mouse_prePos = vec2(0.f, 0.f);
	vec2 mouse_delt = vec2(0.f, 0.f);
	bool mouseDown = false;
	bool mouseClicked = false;
	bool mouseRelased = true;
	float mouse_sensitivity = 0.1f;
};
static InputState INPUT_STATE;