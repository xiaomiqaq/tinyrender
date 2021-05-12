#include "Application.h"
bool create_d3d_device(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc.Width = 800;
	swap_chain_desc.BufferDesc.Height = 800;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = hWnd;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2,
		D3D11_SDK_VERSION, &swap_chain_desc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	create_render_target();
	return true;
}

void cleanup_d3d_device()
{
	cleanup_render_target();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void create_render_target(/*Framebuffer& frame*/)
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();

}

void cleanup_render_target()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			cleanup_render_target();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			create_render_target();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
void initialize_wnd()
{
	// Create application window
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX12 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 960, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!create_d3d_device(hwnd))
	{
		cleanup_d3d_device();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return;
	}

	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);
}
void init_imgui()
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

bool init_render_texture(framebuffer_t* frame)
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
	sd.pSysMem = frame->color_buffer;
	sd.SysMemPitch = WINDOW_HEIGHT * sizeof(uint32_t);
	sd.SysMemSlicePitch = WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t);

	g_pd3dDevice->CreateTexture2D(&texArrayDesc, &sd, &output_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_pd3dDevice->CreateShaderResourceView(output_texture, &srvDesc, &output_source_view);
	return true;
}
void render_d3d()
{
	ImGui::Render();
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0); // Present with vsync
}

static vec3_t get_light_dir()
{
	float theta = LIGHT_THETA;
	float phi = LIGHT_PHI;
	float x = (float)sin(phi) * (float)sin(theta);
	float y = (float)cos(phi);
	float z = (float)sin(phi) * (float)cos(theta);
	return vec3_new(-x, -y, -z);
}
void init_device()
{
	initialize_wnd();
	init_imgui();
	init_render_texture(framebuffer);
}
void cleanup_device()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	::DestroyWindow(hwnd);
}
void init_resource()
{
	mat4_t translation = mat4_translate(4.785f, -105.275f, -23.067f);
	mat4_t rotation = mat4_rotate_y(TO_RADIANS(90));
	mat4_t scale = mat4_scale(0.004f, 0.004f, 0.004f);
	mat4_t root = mat4_mul_mat4(scale, mat4_mul_mat4(rotation, translation));
	m_scene = Scene::scene_from_file("E:/VS2017/MyTinyRender/tinyrender/assets/nier2b/nier2b.scn", root);
	framebuffer = framebuffer_create(WINDOW_WIDTH, WINDOW_HEIGHT);
}
void tick_function(context_t *context, void *userdata)
{
	Scene *scene = (Scene*)userdata;
	perframe_t perframe = build_perframe(scene, context);
	test_draw_scene(scene, context->framebuffer, &perframe);
}

void test_enter_mainloop(tickfunc_t *tickfunc/*, void *userdata, framebuffer_t *framebuffer*/)
{
	init_resource();
	init_device();
	
	camera_t *camera;
	context_t context;
	float aspect;
	float prev_time;
	float print_time;
	int num_frames;


	aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
	camera = camera_create(CAMERA_POSITION, CAMERA_TARGET, aspect);

	/*memset(&record, 0, sizeof(record_t));
	record.light_theta = LIGHT_THETA;
	record.light_phi = LIGHT_PHI;

	memset(&callbacks, 0, sizeof(callbacks_t));
	callbacks.button_callback = button_callback;
	callbacks.scroll_callback = scroll_callback;*/

	memset(&context, 0, sizeof(context_t));
	context.framebuffer = framebuffer;
	context.camera = camera;
	
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
		context.light_dir = get_light_dir();
		tickfunc(&context, m_scene);
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Text("average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Begin("RenderWindow");
		{

			D3D11_MAPPED_SUBRESOURCE subData;
			ZeroMemory(&subData, sizeof(subData));
			g_pd3dDeviceContext->Map(output_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
			memcpy(subData.pData, framebuffer->color_buffer, sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
			g_pd3dDeviceContext->Unmap(output_texture, 0);
		}
		ImGui::Image(output_source_view, ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImVec2(1, 1), ImVec2(0, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 2.5f));
		ImGui::End();
		render_d3d();
	}
	//framebuffer_release(framebuffer);
	camera_release(camera);
};