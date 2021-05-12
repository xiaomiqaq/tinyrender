#pragma once
#include<algorithm>
#include "tgaimage.h"

typedef unsigned char uchar;


//Color ColorRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
//{
//	return Color(r, g, b, a);
//}

//Color ColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
//{
//	return (r | (g << 8) | (b << 16) | (a << 24));
//}



class Framebuffer
{
private:
	int width = 0;
	int height = 0;
	uint8_t* cur_color_buffer; //rgb
	uint8_t* back_color_buffer;
	uint8_t* depth_buffer;
public:
	
	void ClearColorbuffer(TGAColor color);
	void ClearDepthbuffer(uint8_t val);
	void setDepth(int x, int y, float val) { depth_buffer[y*width + x] = val; }
	void setColor(int x, int y, TGAColor col) 
	{ 
		//tgaÊÇbgr ×ªÎªrgb
		cur_color_buffer[(y*width + x) * 4 + 0] = col[0];
		cur_color_buffer[(y*width + x) * 4 + 1] = col[1];
		cur_color_buffer[(y*width + x) * 4 + 2] = col[2];
		cur_color_buffer[(y*width + x) * 4 + 3] = col[3];
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
	uint8_t getDepth(int x, int y) { return depth_buffer[y*width + x]; }
	uint8_t* getColorbufferAddr() { return cur_color_buffer; }
	uint8_t* getDepthBufferAddr() { return depth_buffer; }
	Framebuffer(int widht, int height);
	~Framebuffer();
};
