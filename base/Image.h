#ifndef IMAGE_H
#define IMAGE_H
#include "../pch.h"

typedef enum {
	FORMAT_LDR,
	FORMAT_HDR
} format_t;

typedef struct {
	format_t format;
	int width, height, channels;
	unsigned char *ldr_buffer;
	float *hdr_buffer;
} image_t;

/* image creating/releasing */
image_t *image_create(int width, int height, int channels, format_t format);
void image_release(image_t *image);
image_t *image_load(const char *filename);
void image_save(image_t *image, const char *filename);

/* image processing */
void image_flip_h(image_t *image);
void image_flip_v(image_t *image);

static void save_tga_image(image_t *image, const char *filename);
static void save_hdr_image(image_t *image, const char *filename);

#endif
//class Image
//{
//typedef enum {
//	FORMAT_LDR,
//	FORMAT_HDR
//} format_t;
//public:
//	format_t format;
//	int width, height, channels;
//	unsigned char *ldr_buffer;
//	float *hdr_buffer;
//public:
//	Image();
//	
//	~Image();
//
//	static Image* image_create(int width, int height, int channels, format_t format);
//	static void image_release(Image* img);
//	static Image* image_load(const char* filename);
//
//	void image_flip_h(Image *image);
//	void image_flip_v(Image *image);
//
//};
//
//Image* Image::image_create(int width, int height, int channels, format_t format) {
//	int num_elems = width * height * channels;
//	Image *image;
//
//	assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);
//	assert(format == FORMAT_LDR || format == FORMAT_HDR);
//
//	image = (Image*)malloc(sizeof(Image));
//	image->format = format;
//	image->width = width;
//	image->height = height;
//	image->channels = channels;
//	image->ldr_buffer = NULL;
//	image->hdr_buffer = NULL;
//
//	if (format == FORMAT_LDR) {
//		int size = sizeof(unsigned char) * num_elems;
//		image->ldr_buffer = (unsigned char*)malloc(size);
//		memset(image->ldr_buffer, 0, size);
//	}
//	else {
//		int size = sizeof(float) * num_elems;
//		image->hdr_buffer = (float*)malloc(size);
//		memset(image->hdr_buffer, 0, size);
//	}
//
//	return image;
//}

