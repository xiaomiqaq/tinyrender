#pragma once

typedef struct {
	int width, height;
	unsigned char *color_buffer;
	float *depth_buffer;
} framebuffer_t;