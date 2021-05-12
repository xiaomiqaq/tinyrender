#pragma once
#include "../base/Modal.h"
#include "../base/framebuffer.h"
#include "Program.h"
#include "Shader.h"

typedef struct { int min_x, min_y, max_x, max_y; } bbox_t;
typedef enum {
	POSITIVE_W,
	POSITIVE_X,
	NEGATIVE_X,
	POSITIVE_Y,
	NEGATIVE_Y,
	POSITIVE_Z,
	NEGATIVE_Z
} plane_t;

static int is_inside_plane(vec4_t coord, plane_t plane);
static float get_intersect_ratio(vec4_t prev, vec4_t curr, plane_t plane);
static int clip_against_plane(
	plane_t plane, int in_num_vertices, int varying_num_floats,
	vec4_t in_coords[MAX_VARYINGS], void *in_varyings[MAX_VARYINGS],
	vec4_t out_coords[MAX_VARYINGS], void *out_varyings[MAX_VARYINGS]);
#define CLIP_IN2OUT(plane)                                                  \
    do {                                                                    \
        num_vertices = clip_against_plane(                                  \
            plane, num_vertices, varying_num_floats,                        \
            in_coords, in_varyings, out_coords, out_varyings);              \
        if (num_vertices < 3) {                                             \
            return 0;                                                       \
        }                                                                   \
    } while (0)
#define CLIP_OUT2IN(plane)                                                  \
    do {                                                                    \
        num_vertices = clip_against_plane(                                  \
            plane, num_vertices, varying_num_floats,                        \
            out_coords, out_varyings, in_coords, in_varyings);              \
        if (num_vertices < 3) {                                             \
            return 0;                                                       \
        }                                                                   \
    } while (0)

static int is_vertex_visible(vec4_t v);

static int is_back_facing(vec3_t ndc_coords[3]);
static vec3_t viewport_transform(int width, int height, vec3_t ndc_coord);
static int min_integer(int a, int b);

static int max_integer(int a, int b);
static bbox_t find_bounding_box(vec2_t abc[3], int width, int height);
static vec3_t calculate_weights(vec2_t abc[3], vec2_t p);
static float interpolate_depth(float screen_depths[3], vec3_t weights);
static void interpolate_varyings(
	void *src_varyings[3], void *dst_varyings,
	int sizeof_varyings, vec3_t weights, float recip_w[3]);
static void draw_fragment(framebuffer_t *framebuffer, program_t *program,
	int backface, int index, float depth);
int clip_triangle(int sizeof_varyings,vec4_t in_coords[MAX_VARYINGS], void *in_varyings[MAX_VARYINGS],
									vec4_t out_coords[MAX_VARYINGS], void *out_varyings[MAX_VARYINGS]);
int rasterize_triangle(framebuffer_t *framebuffer, program_t *program,
	vec4_t clip_coords[3], void *varyings[3]);
void graphics_draw_triangle(framebuffer_t *framebuffer, program_t *program);
