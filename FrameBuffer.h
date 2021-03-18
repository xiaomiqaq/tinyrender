#pragma once
//#include <stdint.h>
#include<algorithm>
//typedef uint32_t Color;
typedef unsigned char uchar;
struct TGAColor {
	std::uint8_t rgba[4] = { 0,0,0,0 };
	std::uint8_t bytespp = { 0 };

	TGAColor() = default;
	TGAColor(const std::uint8_t  R, const std::uint8_t G, const std::uint8_t B, const std::uint8_t A = 255) : rgba{ R,G,B,A }, bytespp(4) { }
	TGAColor(const std::uint8_t v) : rgba{ v,0,0,0 }, bytespp(1) { }

	TGAColor(const std::uint8_t *p, const std::uint8_t bpp) : rgba{ 0,0,0,0 }, bytespp(bpp) {
		for (int i = 0; i < bpp; i++)
			rgba[i] = p[i];
	}

	std::uint8_t& operator[](const int i) { return rgba[i]; }

	TGAColor operator *(const double intensity) const {
		TGAColor res = *this;
		double clamped = (std::max)(0., std::min(intensity, 1.));
		for (int i = 0; i < 4; i++) res.rgba[i] = rgba[i] * clamped;
		return res;
	}
	TGAColor operator +(const TGAColor& c2)
	{
		TGAColor res(0, 0, 0, 0);
		for (int i = 0; i < 4; i++) res.rgba[i] = std::min(this->rgba[i] + c2.rgba[i], 255);
		return res;
	}
	TGAColor toRGB()
	{
		return TGAColor(rgba[2], rgba[1], rgba[0], rgba[3]);
	}
};
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
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
	uint8_t* color_buffer; //rgb
	float* depth_buffer;
public:
	
	void ClearColorbuffer(TGAColor color);
	void ClearDepthbuffer(float val);
	void setDepth(int x, int y, float val) { depth_buffer[y*width + x] = val; }
	void setColor(int x, int y, TGAColor col) 
	{ 
		//tgaÊÇbgr ×ªÎªrgb
		color_buffer[(y*width + x) * 4 + 0] = col[0];
		color_buffer[(y*width + x) * 4 + 1] = col[1];
		color_buffer[(y*width + x) * 4 + 2] = col[2];
		color_buffer[(y*width + x) * 4 + 3] = col[3];
	}

	int getWidth() { return width; }
	int getHeight() { return height; }
	float getDepth(int x, int y) { return depth_buffer[y*width + x]; }
	uint8_t* getColorbufferAddr() { return color_buffer; }
	float* getDepthBufferAddr() { return depth_buffer; }
	Framebuffer(int widht, int height);
	~Framebuffer();
};
