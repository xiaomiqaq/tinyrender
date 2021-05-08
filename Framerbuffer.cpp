#include "FrameBuffer.h"



Framebuffer::Framebuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	cur_color_buffer = new uint8_t[width*height*4];
	depth_buffer = new uint8_t[width*height];
	this->ClearColorbuffer(TGAColor(255, 255, 255, 255));
	this->ClearDepthbuffer(255);
}
void Framebuffer::ClearColorbuffer(TGAColor color)
{
	
	memset(cur_color_buffer,255,height*width*4*sizeof(uint8_t));
}
void Framebuffer::ClearDepthbuffer(uint8_t val)
{
	
	memset(depth_buffer, val, height*width*sizeof(uint8_t));

}
Framebuffer::~Framebuffer()
{
	delete cur_color_buffer;
	delete depth_buffer;
}