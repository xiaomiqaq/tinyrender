#include "FrameBuffer.h"

Framebuffer::Framebuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	color_buffer = new uint8_t[width*height*4];
	depth_buffer = new float[width*height];
	this->ClearColorbuffer(TGAColor(255, 255, 255, 255));
	this->ClearDepthbuffer(-100.0f);
}
void Framebuffer::ClearColorbuffer(TGAColor color)
{
	for (int i = 0; i < width*height; i++)
	{
		color_buffer[i * 4 + 0] = color[0];
		color_buffer[i * 4 + 1] = color[1];
		color_buffer[i * 4 + 2] = color[2];
		color_buffer[i * 4 + 3] = color[3];
	}
}
void Framebuffer::ClearDepthbuffer(float val)
{
	for (int i = 0; i < width*height; i++)
		depth_buffer[i] = val;
}
Framebuffer::~Framebuffer()
{
	delete color_buffer;
	delete depth_buffer;
}