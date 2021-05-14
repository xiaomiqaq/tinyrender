#include "Record.h"
vec2_t Record::get_pos_delta(vec2_t old_pos, vec2_t new_pos) 
{
	vec2_t delta = vec2_sub(new_pos, old_pos);
	return vec2_div(delta, (float)WINDOW_HEIGHT);
}
vec2_t Record::get_cursor_pos()
{
	ImGuiIO& io = ImGui::GetIO();
	return vec2_new(io.MousePos.x, io.MousePos.y);
}
void Record::button_call()
{
	ImGuiIO& io = ImGui::GetIO();
	vec2_t cursor_pos = get_cursor_pos();
	bool* mouse_click = io.MouseClicked;
	bool* mouse_down = io.MouseDown;
	vec2_t mouse_delta = vec2_new(io.MouseDelta.x, io.MouseDelta.y);
	float framerate = io.Framerate / 8000.f;
	
	if (mouse_down[0] && io.MouseDownOwned[0]) orbit_delta = vec2_mul(mouse_delta, framerate);
	
	if (mouse_down[1] && io.MouseDownOwned[1]) pan_delta = vec2_mul(vec2_new(-mouse_delta.x, mouse_delta.y), framerate*0.5);
	
	dolly_delta = io.MouseWheel;
}
void Record::reback()
{
	orbit_delta = vec2_new(0, 0);
	pan_delta = vec2_new(0, 0);
	dolly_delta = 0;
}