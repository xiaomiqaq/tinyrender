//#pragma once
//#include <d3d11.h>
//#include "imgui.h"
//#include "imgui_impl_win32.h"
//#include "imgui_impl_dx11.h"
//#include <d3d11.h>
//#include <tchar.h>
//#include <cstdint>
//#include <vector>
//#include "../base/framebuffer.h"
//static const int WINDOW_WIDTH = 800;
//static const int WINDOW_HEIGHT = 600;
//static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//static HWND hwnd;
//static ID3D11Device*            g_pd3dDevice;
////static ID3D11DeviceContext*     g_pd3dDeviceContext;
//static IDXGISwapChain*          g_pSwapChain;
//static ID3D11RenderTargetView*  g_mainRenderTargetView;
//
//static ID3D11Texture2D* output_texture;
//static ID3D11ShaderResourceView* output_source_view;
//
//
////class Device
////{
////public:
////	 bool create_d3d_device(HWND hWnd/*,Framebuffer& framebuffer*/);
////	 void cleanup_d3d_device();
////	 void create_render_target(/*Framebuffer& frame*/);
////	 void cleanup_render_target();
////
////	// Win32 message handler
////	 LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
////
////	//初始化窗口 IMGUI
////	 void initialize_wnd();
////	 void init_imgui();
////	 bool init_render_texture(framebuffer_t* frame);
////
////	 void render_d3d_imgui();
////public:
////	 HWND hwnd;
////	 ID3D11Device*            g_pd3dDevice ;
////	 ID3D11DeviceContext*     g_pd3dDeviceContext ;
////	 IDXGISwapChain*          g_pSwapChain ;
////	 ID3D11RenderTargetView*  g_mainRenderTargetView ;
////
////	 ID3D11Texture2D* output_texture ;
////	 ID3D11ShaderResourceView* output_source_view ;
////
////};
//
//
//bool create_d3d_device(HWND hWnd, ID3D11DeviceContext*     g_pd3dDeviceContext);
//void cleanup_d3d_device(ID3D11DeviceContext*     g_pd3dDeviceContext);
//void create_render_target(/*Framebuffer& frame*/);
//void cleanup_render_target();
//// Forward declare message handler from imgui_impl_win32.cpp
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//// Win32 message handler
//LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
////初始化窗口 IMGUI
////void initialize_wnd(HWND hwnd);
////void init_imgui(HWND hwnd, ID3D11Device*  g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext);
////bool init_render_texture(framebuffer_t* frame);
//
//void initialize_wnd(ID3D11DeviceContext*     g_pd3dDeviceContext);
//void init_imgui(ID3D11DeviceContext*     g_pd3dDeviceContext);
//bool init_render_texture(framebuffer_t* frame);
//void render_d3d_imgui(ID3D11DeviceContext*     g_pd3dDeviceContext);
//
//
