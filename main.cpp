#include <vector>
#include <algorithm>
#include "glm/gtc/type_ptr.hpp"
#include"Camera.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.h"
#include "glFunction.h"
#include "Device.h"

using namespace glm;
using namespace std;
//void triangle(vec3 vertexs[], PhoneShader shader, TGAImage& image)
//{
//	//�ҵ���Χ�����εİ�Χ��
//	glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
//	glm::vec2 bboxmax(0, 0);
//	glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			bboxmin[j] = (std::max)(0, (int)(std::min)(bboxmin[j], vertexs[i][j]));
//			bboxmax[j] = (std::min)((int)screenBorder[j], (int)(std::max)(bboxmax[j], vertexs[i][j]));
//		}
//	}
//	//������Χ���еĵ�
//	glm::vec3 pv;
//	for (pv.x = bboxmin.x; pv.x <= bboxmax.x; pv.x++)
//	{
//		for (pv.y = bboxmin.y; pv.y <= bboxmax.y; pv.y++)
//		{
//			glm::vec3 bc_p = barycentric(vertexs, pv);  //curSV����������
//			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
//			pv.z = 0;
//			for (int i = 0; i < 3; i++)
//			{
//				pv.z += vertexs[i].z * bc_p[i];  //�������ֵ
//			}
//			if (zbuffer[int(pv.x + pv.y * width)] < pv.z)
//			{
//				TGAColor color;
//				bool isDraw = shader.fragment(bc_p, color);
//				if (isDraw)
//				{
//					zbuffer[int(pv.x + pv.y * width)] = pv.z;
//					image.set(pv.x, pv.y, color);
//				}
//			}
//		}
//	}
//}
void triangle(vec3 vertexs[], IShader* shader, Framebuffer& frame)
{
	vec3 v1 = vertexs[0];
	vec3 v2 = vertexs[1];
	vec3 v3 = vertexs[2];
	//�ҵ���Χ�����εİ�Χ��
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
	//������Χ���еĵ�
	glm::vec3 curSV;
	for (curSV.x = bboxmin.x; curSV.x <= bboxmax.x; curSV.x++)
	{
		for (curSV.y = bboxmin.y; curSV.y <= bboxmax.y; curSV.y++)
		{
			glm::vec3 bc_p = barycentric(vertexs, curSV);  //curSV����������
			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
			curSV.z = 0;
			for (int i = 0; i < 3; i++)
			{
				curSV.z += vertexs[i].z * bc_p[i];  //�������ֵ
			}
			if (curSV.z < frame.getDepth(curSV.x,curSV.y) )   //zֵС�Ŀ������������ʾ
			{
				TGAColor color = TGAColor(0,0,0,0);
				bool isDraw = shader->fragment(bc_p, color);
				frame.setDepth(curSV.x, curSV.y, curSV.z);
				frame.setColor(curSV.x, curSV.y, color);
			}
		}
	}
}


//void triangle(glm::vec3 vertexs[], DepthShader shader, TGAImage& image)
//{
//	//�ҵ���Χ�����εİ�Χ��
//	glm::vec2 bboxmin(image.get_width() - 1, image.get_height() - 1);
//	glm::vec2 bboxmax(0, 0);
//	glm::vec2 screenBorder(image.get_width() - 1, image.get_height() - 1);
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 2; j++)
//		{
//			bboxmin[j] = (std::max)(0, (int)(std::min)(bboxmin[j], vertexs[i][j]));
//			bboxmax[j] = (std::min)((int)screenBorder[j], (int)(std::max)(bboxmax[j], vertexs[i][j]));
//		}
//	}
//	//������Χ���еĵ�
//	glm::vec3 pv;
//	for (pv.x = bboxmin.x; pv.x <= bboxmax.x; pv.x++)
//	{
//		for (pv.y = bboxmin.y; pv.y <= bboxmax.y; pv.y++)
//		{
//			glm::vec3 bc_p = barycentric(vertexs, pv);  //curSV����������
//			if (bc_p.x < 0 || bc_p.y < 0 || bc_p.z < 0) continue;
//			pv.z = 0;
//			for (int i = 0; i < 3; i++)
//			{
//				pv.z += vertexs[i].z * bc_p[i];  //�������ֵ
//			}
//			if (shadowBuffer[int(pv.x + pv.y * width)] < pv.z)
//			{
//				TGAColor color;
//				bool isDraw = shader.fragment(bc_p, color);
//				if (isDraw)
//				{
//					shadowBuffer[int(pv.x + pv.y * width)] = pv.z;
//					image.set(pv.x, pv.y, color);
//				}
//			}
//		}
//	}
//}
//
//int main(int argc, char** argv) {
//	model = new Model("../obj/african_head/african_head.obj");
//	vec3 eye(0, 2, 3);
//	vec3 target(0, 0, 0);
//
//
//	DepthShader shadowShader(model);
//	PhoneShader shader(model);
//	mat4 ShadowSpace = shadowShader.ShadowMapMat;
//
//
//	for (int iFace = 0; iFace < model->nfaces(); iFace++)
//	{
//		//����ShadowMap
//		vec3 shadowPos[3];
//		for (int j = 0; j < 3; j++)
//		{
//			shadowPos[j] = shadowShader.vertex(iFace, j);
//		}
//		triangle(shadowPos, shadowShader, shadowDepth);
//	}
//	shader.ShadowMat = shadowShader.ShadowMapMat;
//	for (int iFace = 0; iFace < model->nfaces(); iFace++)
//	{
//
//		vec3 screenPos[3];
//		for (int j = 0; j < 3; j++)
//		{
//			screenPos[j] = shader.vertex(iFace, j);
//		}
//
//		triangle(screenPos, shader, image);
//	}
//    delete model;
//	delete[] zbuffer ;
//
//}

HWND hwnd;

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void RenderModel(Model* model,Framebuffer* frame)
{

}
//��ʼ������
void initialize_wnd(Framebuffer& framebuffer) noexcept
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX12 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 800, 800, NULL, NULL, wc.hInstance, NULL);

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
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	//ΪGUI����Ⱦ�ṩ����(WIN)��ƽ̨��D3D��
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

}
bool InitTexture(ID3D11Device*  md3dDevice, ID3D11ShaderResourceView* mTexSrv ,Framebuffer& frame)
{
	// ������������
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = WINDOW_WIDTH;
	texArrayDesc.Height = WINDOW_HEIGHT;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = 1;
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;      // ��ʹ�ö��ز���
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DYNAMIC;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texArrayDesc.MiscFlags = 0; // ָ����Ҫ����mipmap

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
bool InitRenderTargetView(ID3D11Device*  md3dDevice, ID3D11RenderTargetView* mRTV, Framebuffer& frame)
{
	// ������������
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = WINDOW_WIDTH;
	texArrayDesc.Height = WINDOW_HEIGHT;
	texArrayDesc.MipLevels = 1;    //������Mipmap
	texArrayDesc.ArraySize = 1;
	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texArrayDesc.SampleDesc.Count = 1;      // ��ʹ�ö��ز���
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DYNAMIC;
	texArrayDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texArrayDesc.MiscFlags = 0; // ָ����Ҫ����mipmap

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = frame.getColorbufferAddr();
	sd.SysMemPitch = frame.getHeight() * sizeof(uint32_t);
	sd.SysMemSlicePitch = frame.getHeight() * frame.getWidth() * sizeof(uint32_t);


	md3dDevice->CreateTexture2D(&texArrayDesc, &sd, &g_texRTV);


	md3dDevice->CreateRenderTargetView(g_texRTV, nullptr, &mRTV);

	return true;
}
// Main code
int main(int, char**)
{
	Camera* camera = new Camera(1.f, 10.f, 90.f, 1.f);
	{
		vec3 eye(0, 0, 2);
		vec3 target(0, 0, 0);
		camera->LookAt2(eye, target, vec3(0.f, 1.f, 0.f));
		camera->Perspective();
	}
	Framebuffer framebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
	//InitRenderTargetView(g_pd3dDevice, g_rtv, framebuffer);
	Model* model = new Model("../obj/african_head/african_head.obj");

	PhoneShader shader(model,camera);
	shader.viewPort(framebuffer.getWidth(), framebuffer.getHeight(), 256);
	initialize_wnd(framebuffer);
	init_imgui();
	InitTexture(g_pd3dDevice, g_texsrv , framebuffer);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("�ƴ���");
			if (ImGui::Button("�����"))
			{
				vec3 tris[3] = { vec3(0,0,0),vec3(0,100,0),vec3(100,100,0) };
				for (int iFace = 0; iFace < model->nfaces(); iFace++)
				{

					vec3 screenPos[3];
					for (int j = 0; j < 3; j++)
					{
						screenPos[j] = shader.vertex(iFace, j);
					}

					triangle(screenPos, &shader, framebuffer);
				}

				D3D11_MAPPED_SUBRESOURCE subData;
				ZeroMemory(&subData, sizeof(subData));
				g_pd3dDeviceContext->Map(g_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
				memcpy(subData.pData, framebuffer.getColorbufferAddr(), sizeof(uint32_t) * framebuffer.getWidth() * framebuffer.getHeight());
				g_pd3dDeviceContext->Unmap(g_texture, 0);
			}
			ImGui::Image(g_texsrv, ImVec2(framebuffer.getWidth(), framebuffer.getHeight()), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 2.5f));
			ImGui::End();

		}


		// Rendering
		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	delete model;
	delete camera;
	return 0;
}

// Helper functions

