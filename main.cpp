#include <vector>
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"
#include "M_Camera.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "M_Shader.h"
#include "glFunction.h"
#include "App/Application.h"
#include <d3d11.h>


using namespace glm;
using namespace std;
static string PROJECT_DIECTORY = "E:/VS2017/MyTinyRender/tinyrender/";
namespace TR {
	int tri_num = 0;

	//void RenderUI() 
	//{
	//	ImGui::Begin("Inspecter");
	//	ImGui::Text("mouse down: %s", INPUT_STATE.mouseDown ? "TRUE" : "FALSE");
	//	ImGui::Text("Face number: %d", TR::tri_num);
	//	ImGui::Text("Camera");
	//	ImGui::DragFloat3("Position", (float*)&camera->pos,1.f,-800.f,800.f,"%.2f",1.f);
	//	ImGui::DragFloat("fov", &camera->fov, 1.0f, 10.f, 120.f, "%f", 1.f);
	//	ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//	//ImGui::Text("fps: %f", fps());
	//	ImGui::End();
	//}
}



//void UpdateInputState()
//{
//	ImGuiIO& io = ImGui::GetIO();
//
//	INPUT_STATE.mouse_delt.x = io.MouseDelta.x;
//	INPUT_STATE.mouse_delt.y = io.MouseDelta.y;
//	INPUT_STATE.mouseDown = ImGui::IsMouseDown(1);
//
//}




//int main(int, char**)
//{
//	Framebuffer framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
//	Model* model = new Model("E:/VS2017/MyTinyRender/tinyrender/obj/african_head/african_head.obj");
//	PhoneShader shader(model,camera);
//
//	shader.viewPort(framebuffer.getWidth(), framebuffer.getHeight(), 256);
//	initialize_wnd();
//	init_imgui();
//	InitTexture(g_pd3dDevice, output_source_view , framebuffer);
//
//    // Main loop
//    MSG msg;
//    ZeroMemory(&msg, sizeof(msg));
//
//	while (msg.message != WM_QUIT)
//	{
//		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
//		{
//			::TranslateMessage(&msg);
//			::DispatchMessage(&msg);
//			continue;
//		}
//
//		// Start the Dear ImGui frame
//		ImGui_ImplDX11_NewFrame();
//		ImGui_ImplWin32_NewFrame();
//		ImGui::NewFrame();
//		{
//			ImGui::Begin("RenderWindow");
//			{
//				for (int iFace = 0; iFace < model->nfaces(); iFace++)
//				{
//					vec3 screenPos[3];
//					for (int j = 0; j < 3; j++)
//					{
//						screenPos[j] = shader.vertex(iFace, j);
//					}
//					//screenPos = shader.vertex(iFace);
//					triangle(screenPos, &shader, framebuffer);
//				}
//
//				D3D11_MAPPED_SUBRESOURCE subData;
//				ZeroMemory(&subData, sizeof(subData));
//				g_pd3dDeviceContext->Map(output_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
//				memcpy(subData.pData, framebuffer.getColorbufferAddr(), sizeof(uint32_t) * framebuffer.getWidth() * framebuffer.getHeight());
//				g_pd3dDeviceContext->Unmap(output_texture, 0);
//			}
//			ImGui::Image(output_source_view, ImVec2(framebuffer.getWidth(), framebuffer.getHeight()), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 2.5f));
//			framebuffer.ClearColorbuffer(TGAColor(255, 255, 255, 255));
//			framebuffer.ClearDepthbuffer(255);
//			ImGui::End();
//		}
//		TR::RenderUI();
//		UpdateInputState();
//		camera->Update();
//		// Rendering
//		ImGui::Render();
//		RenderD3D();
//		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//		g_pSwapChain->Present(1, 0); // Present with vsync
//
//		//REFRESH
//		TR::tri_num = 0;
//	}
//
//	// Cleanup
//	ImGui_ImplDX11_Shutdown();
//	ImGui_ImplWin32_Shutdown();
//	ImGui::DestroyContext();
//
//	CleanupDeviceD3D();
//	::DestroyWindow(hwnd);
//	delete model;
//	delete camera;
//	return 0;
//}

int main()
{
	test_enter_mainloop(tick_function/*, m_scene,framebuffer*/);
	
	return 0;
}