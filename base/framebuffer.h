#pragma once
#include "../pch.h"

typedef struct {
	int width, height;
	unsigned char *color_buffer;
	float *depth_buffer;
} framebuffer_t;

framebuffer_t *framebuffer_create(int width, int height);
void framebuffer_release(framebuffer_t *framebuffer);
void framebuffer_clear_color(framebuffer_t *framebuffer, vec4_t color);
void framebuffer_clear_depth(framebuffer_t *framebuffer, float depth);
