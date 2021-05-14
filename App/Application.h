#pragma once
#include "../Graphics/Perframe.h"
#include "../base/Scene.h"
#include <d3d11.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <tchar.h>
#include <cstdint>
#include <vector>
#include "../base/framebuffer.h"
#include "Record.h"
typedef void tickfunc_t(context_t *context, void *userdata);

// Data

static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static HWND hwnd;
static ID3D11Device*            g_pd3dDevice;
static ID3D11DeviceContext*     g_pd3dDeviceContext;
static IDXGISwapChain*          g_pSwapChain;
static ID3D11RenderTargetView*  g_mainRenderTargetView;

static ID3D11Texture2D* output_texture;
static ID3D11ShaderResourceView* output_source_view;


static const vec3_t CAMERA_POSITION = { 0, 0, 1.5f };
static const vec3_t CAMERA_TARGET = { 0, 0, 0 };

static const float LIGHT_THETA = TO_RADIANS(45);
static const float LIGHT_PHI = TO_RADIANS(45);
static const float LIGHT_SPEED = PI;

static Scene* m_scene;
static framebuffer_t *framebuffer;


bool create_d3d_device(HWND hWnd);
void cleanup_d3d_device();
void create_render_target();
void cleanup_render_target();
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void initialize_wnd();
void init_imgui();
bool init_render_texture(framebuffer_t* frame);
void render_d3d_imgui();

static vec3_t get_light_dir();
void init_device();
void cleanup_device();
void init_resource();
void tick_function(context_t *context, void *userdata);
void test_enter_mainloop(tickfunc_t *tickfunc/*, void *userdata, framebuffer_t *framebuffer*/);

