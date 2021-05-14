#pragma once
#include "../pch.h"
#include "imgui.h"
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;
;
class Record
{
public:
	static vec2_t get_pos_delta(vec2_t old_pos, vec2_t new_pos);
	static vec2_t get_cursor_pos();
	void button_call();
	void reback();
public:
	/* ¹ìµÀÐý×ª */
	vec2_t orbit_delta;
	/* Æ½ÒÆ */
	vec2_t pan_delta;
	/* zoom */
	float dolly_delta;
	/* light */
	float light_theta;
	float light_phi;
	/* click */
	float press_time;
	float release_time;
	vec2_t press_pos;
	vec2_t release_pos;
	int single_click;
	int double_click;
	vec2_t click_pos;
} ;