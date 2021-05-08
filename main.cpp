#include <vector>
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.h"
#include "glFunction.h"
#include "Device.h"
#include <d3d11.h>
#include "base/Scene.h"

using namespace glm;
using namespace std;
static string PROJECT_DIECTORY = "E:/VS2017/MyTinyRender/tinyrender/";
HWND hwnd;
Camera* camera = new Camera(vec3(0.f,0.f,2.f), 1.f, 10.f, 70.f, (float)WINDOW_WIDTH/WINDOW_HEIGHT , glm::vec3(0.f,1.f,0.f));
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
namespace TR {
	int tri_num = 0;

	void RenderUI() 
	{
		ImGui::Begin("Inspecter");
		ImGui::Text("mouse down: %s", INPUT_STATE.mouseDown ? "TRUE" : "FALSE");
		ImGui::Text("Face number: %d", TR::tri_num);
		ImGui::Text("Camera");
		ImGui::DragFloat3("Position", (float*)&camera->pos,1.f,-800.f,800.f,"%.2f",1.f);
		ImGui::DragFloat("fov", &camera->fov, 1.0f, 10.f, 120.f, "%f", 1.f);
		ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::Text("fps: %f", fps());
		ImGui::End();
	}
}

void triangle(vec3 vertexs[], IShader* shader, Framebuffer& frame)
{
	TR::tri_num++;
	//找到包围三角形的包围盒
	glm::vec2 bboxmin(frame.getWidth()-1 , frame.getHeight() - 1);
	glm::vec2 bboxmax(0, 0);
	glm::vec2 screenBorder(frame.getWidth() - 1, frame.getHeight() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bboxmin[j] = (int)(std::max)(0.0f, (std::min)(bboxmin[j], vertexs[i][j]));
			bboxmax[j] =  (int)(std::min)(screenBorder[j],(std::max)(bboxmax[j], vertexs[i][j]) );
		}
	}
	//遍历包围盒中的点
	glm::vec3 curSV;
	for (curSV.x = bboxmin.x; curSV.x <= bboxmax.x; curSV.x++)
	{
		for (curSV.y = bboxmin.y; curSV.y <= bboxmax.y; curSV.y++)
		{
			glm::vec3 bc_p = barycentric(vertexs, curSV);  //curSV的重心坐标
			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
			curSV.z = 0;
			for (int i = 0; i < 3; i++)
			{
				curSV.z += vertexs[i].z * bc_p[i];  //计算深度值
			}
			if (curSV.z < frame.getDepth(curSV.x,curSV.y) )   //z值小的靠近相机，故显示
			{
				TGAColor color = TGAColor(0,0,0,0);
				bool isDraw = shader->fragment(bc_p, color);
				frame.setDepth(curSV.x, curSV.y, curSV.z);
				frame.setColor(curSV.x, curSV.y, color);
			}
		}
	}
}

void UpdateInputState()
{
	ImGuiIO& io = ImGui::GetIO();

	INPUT_STATE.mouse_delt.x = io.MouseDelta.x;
	INPUT_STATE.mouse_delt.y = io.MouseDelta.y;
	INPUT_STATE.mouseDown = ImGui::IsMouseDown(1);

}


void RenderModel(Model* model,Framebuffer* frame)
{

}
//初始化窗口
void initialize_wnd() noexcept
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX12 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 960, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd/*,framebuffer*/))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return ;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);

	::UnregisterClass(wc.lpszClassName, wc.hInstance);
}

void init_imgui() noexcept
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls


	ImGui::StyleColorsDark();

	//为GUI的渲染提供窗口(WIN)和平台（D3D）
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}

bool InitTexture(ID3D11Device*  md3dDevice, ID3D11ShaderResourceView* mTexSrv ,Framebuffer& frame)
{
	// 创建纹理数组
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = WINDOW_WIDTH;
	texArrayDesc.Height = WINDOW_HEIGHT;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 1;
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;      // 不使用多重采样
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DYNAMIC;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texArrayDesc.MiscFlags = 0; // 指定需要生成mipmap

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = frame.getColorbufferAddr();
	sd.SysMemPitch = frame.getHeight() * sizeof(uint32_t);
	sd.SysMemSlicePitch = frame.getHeight() * frame.getWidth() * sizeof(uint32_t);

	md3dDevice->CreateTexture2D(&texArrayDesc, &sd, &g_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	md3dDevice->CreateShaderResourceView(g_texture, &srvDesc, &g_texsrv);

	return true;
}

void RenderD3D()
{
	
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };	
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
}


//int main(int, char**)
//{
//	Framebuffer framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
//	Model* model = new Model("E:/VS2017/MyTinyRender/tinyrender/obj/african_head/african_head.obj");
//	PhoneShader shader(model,camera);
//
//	shader.viewPort(framebuffer.getWidth(), framebuffer.getHeight(), 256);
//	initialize_wnd();
//	init_imgui();
//	InitTexture(g_pd3dDevice, g_texsrv , framebuffer);
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
//				g_pd3dDeviceContext->Map(g_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
//				memcpy(subData.pData, framebuffer.getColorbufferAddr(), sizeof(uint32_t) * framebuffer.getWidth() * framebuffer.getHeight());
//				g_pd3dDeviceContext->Unmap(g_texture, 0);
//			}
//			ImGui::Image(g_texsrv, ImVec2(framebuffer.getWidth(), framebuffer.getHeight()), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 2.5f));
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
	std::vector<int> v1;
	int a=1, b=1, c = 1;
	v1.push_back(a); v1.push_back(b); v1.push_back(c);
	std::vector<int> v2;
	v2.swap(v1);
	mat4_t translation = mat4_translate(4.785f, -105.275f, -23.067f);
	mat4_t rotation = mat4_rotate_y(TO_RADIANS(90));
	mat4_t scale = mat4_scale(0.004f, 0.004f, 0.004f);
	mat4_t root = mat4_mul_mat4(scale, mat4_mul_mat4(rotation, translation));
	scene_from_file("E:/VS2017/MyTinyRender/tinyrender/assets/nier2b/nier2b.scn",root);
	return 0;
}