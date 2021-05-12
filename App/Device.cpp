//#include "Device.h"
//bool create_d3d_device(HWND hWnd, ID3D11DeviceContext*     g_pd3dDeviceContext)
//{
//	// Setup swap chain
//	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
//	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
//	swap_chain_desc.BufferCount = 2;
//	swap_chain_desc.BufferDesc.Width = 800;
//	swap_chain_desc.BufferDesc.Height = 800;
//	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
//	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
//	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
//	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swap_chain_desc.OutputWindow = hWnd;
//	swap_chain_desc.SampleDesc.Count = 1;
//	swap_chain_desc.SampleDesc.Quality = 0;
//	swap_chain_desc.Windowed = TRUE;
//	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//
//	UINT createDeviceFlags = 0;
//	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//	D3D_FEATURE_LEVEL featureLevel;
//	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
//	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2,
//						D3D11_SDK_VERSION, &swap_chain_desc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
//		return false;
//
//	create_render_target();
//	return true;
//}
//
//void cleanup_d3d_device(ID3D11DeviceContext*     g_pd3dDeviceContext)
//{
//	cleanup_render_target();
//	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
//	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
//	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
//}
//
//void create_render_target(/*Framebuffer& frame*/)
//{
//	ID3D11Texture2D* pBackBuffer;
//	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
//	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
//	pBackBuffer->Release();
//
//}
//
//void cleanup_render_target()
//{
//	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
//}
//
//// Win32 message handler
//LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
//		return true;
//
//	switch (msg)
//	{
//	case WM_SIZE:
//		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
//		{
//			cleanup_render_target();
//			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
//			create_render_target();
//		}
//		return 0;
//	case WM_SYSCOMMAND:
//		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
//			return 0;
//		break;
//	case WM_DESTROY:
//		::PostQuitMessage(0);
//		return 0;
//	}
//	return ::DefWindowProc(hWnd, msg, wParam, lParam);
//}
//void initialize_wnd(ID3D11DeviceContext*     g_pd3dDeviceContext)
//{
//	// Create application window
//	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
//	::RegisterClassEx(&wc);
//	hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX12 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 960, NULL, NULL, wc.hInstance, NULL);
//
//	// Initialize Direct3D
//	if (!create_d3d_device(hwnd, g_pd3dDeviceContext))
//	{
//		cleanup_d3d_device(g_pd3dDeviceContext);
//		::UnregisterClass(wc.lpszClassName, wc.hInstance);
//		return;
//	}
//
//	// Show the window
//	::ShowWindow(hwnd, SW_SHOWDEFAULT);
//	::UpdateWindow(hwnd);
//}
//void init_imgui(ID3D11DeviceContext*     g_pd3dDeviceContext)
//{
//	// Setup Dear ImGui context
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//
//
//	ImGui::StyleColorsDark();
//
//	//为GUI的渲染提供窗口(WIN)和平台（D3D）
//	ImGui_ImplWin32_Init(hwnd);
//	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
//
//}
//
//bool init_render_texture(framebuffer_t* frame)
//{
//	// 创建纹理数组
//	D3D11_TEXTURE2D_DESC texArrayDesc;
//	texArrayDesc.Width = WINDOW_WIDTH;
//	texArrayDesc.Height = WINDOW_HEIGHT;
//	texArrayDesc.MipLevels = 1;
//	texArrayDesc.ArraySize = 1;
//	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texArrayDesc.SampleDesc.Count = 1;      // 不使用多重采样
//	texArrayDesc.SampleDesc.Quality = 0;
//	texArrayDesc.Usage = D3D11_USAGE_DYNAMIC;
//	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	texArrayDesc.MiscFlags = 0; // 指定需要生成mipmap
//
//	D3D11_SUBRESOURCE_DATA sd;
//	sd.pSysMem = frame->color_buffer;
//	sd.SysMemPitch = WINDOW_HEIGHT * sizeof(uint32_t);
//	sd.SysMemSlicePitch = WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t);
//
//	g_pd3dDevice->CreateTexture2D(&texArrayDesc, &sd, &output_texture);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = 1;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	g_pd3dDevice->CreateShaderResourceView(output_texture, &srvDesc, &output_source_view);
//	return true;
//}
//void render_d3d(ID3D11DeviceContext*     g_pd3dDeviceContext)
//{
//	ImGui::Render();
//	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
//	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
//	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
//	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
//	g_pSwapChain->Present(1, 0); // Present with vsync
//}
//
////bool create_d3d_device(HWND hWnd/*,Framebuffer& framebuffer*/)
////{
////	// Setup swap chain
////	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
////	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
////	swap_chain_desc.BufferCount = 2;
////	swap_chain_desc.BufferDesc.Width = 800;
////	swap_chain_desc.BufferDesc.Height = 800;
////	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
////	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
////	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
////	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
////	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
////	swap_chain_desc.OutputWindow = hWnd;
////	swap_chain_desc.SampleDesc.Count = 1;
////	swap_chain_desc.SampleDesc.Quality = 0;
////	swap_chain_desc.Windowed = TRUE;
////	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
////
////	UINT createDeviceFlags = 0;
////	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
////	D3D_FEATURE_LEVEL featureLevel;
////	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
////	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2,
////		D3D11_SDK_VERSION, &swap_chain_desc, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
////		return false;
////
////	create_render_target();
////	return true;
////}
////
////void cleanup_d3d_device()
////{
////	cleanup_render_target();
////	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
////	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
////	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
////}
////
////void create_render_target(/*Framebuffer& frame*/)
////{
////	ID3D11Texture2D* pBackBuffer;
////	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
////	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
////	pBackBuffer->Release();
////
////}
////
////void cleanup_render_target()
////{
////	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
////}
////
////// Win32 message handler
////LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
////{
////	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
////		return true;
////
////	switch (msg)
////	{
////	case WM_SIZE:
////		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
////		{
////			cleanup_render_target();
////			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
////			create_render_target();
////		}
////		return 0;
////	case WM_SYSCOMMAND:
////		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
////			return 0;
////		break;
////	case WM_DESTROY:
////		::PostQuitMessage(0);
////		return 0;
////	}
////	return ::DefWindowProc(hWnd, msg, wParam, lParam);
////}
////void initialize_wnd(HWND hwnd, )
////{
////	// Create application window
////	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
////	::RegisterClassEx(&wc);
////	hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX12 Example"), WS_OVERLAPPEDWINDOW, 100, 100, 1280, 960, NULL, NULL, wc.hInstance, NULL);
////
////	// Initialize Direct3D
////	if (!create_d3d_device(hwnd/*,framebuffer*/))
////	{
////		cleanup_d3d_device();
////		::UnregisterClass(wc.lpszClassName, wc.hInstance);
////		return;
////	}
////
////	// Show the window
////	::ShowWindow(hwnd, SW_SHOWDEFAULT);
////	::UpdateWindow(hwnd);
////}
////void init_imgui(HWND hwnd, ID3D11Device*  g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext)
////{
////	// Setup Dear ImGui context
////	IMGUI_CHECKVERSION();
////	ImGui::CreateContext();
////	ImGuiIO& io = ImGui::GetIO(); (void)io;
////	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
////
////
////	ImGui::StyleColorsDark();
////
////	//为GUI的渲染提供窗口(WIN)和平台（D3D）
////	ImGui_ImplWin32_Init(hwnd);
////	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
////
////}
////
////bool init_render_texture(framebuffer_t* frame)
////{
////	// 创建纹理数组
////	D3D11_TEXTURE2D_DESC texArrayDesc;
////	texArrayDesc.Width = WINDOW_WIDTH;
////	texArrayDesc.Height = WINDOW_HEIGHT;
////	texArrayDesc.MipLevels = 1;
////	texArrayDesc.ArraySize = 1;
////	texArrayDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
////	texArrayDesc.SampleDesc.Count = 1;      // 不使用多重采样
////	texArrayDesc.SampleDesc.Quality = 0;
////	texArrayDesc.Usage = D3D11_USAGE_DYNAMIC;
////	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
////	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
////	texArrayDesc.MiscFlags = 0; // 指定需要生成mipmap
////
////	D3D11_SUBRESOURCE_DATA sd;
////	sd.pSysMem = frame->color_buffer;
////	sd.SysMemPitch = WINDOW_HEIGHT * sizeof(uint32_t);
////	sd.SysMemSlicePitch = WINDOW_HEIGHT * WINDOW_WIDTH * sizeof(uint32_t);
////
////	g_pd3dDevice->CreateTexture2D(&texArrayDesc, &sd, &output_texture);
////
////	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
////	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
////	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
////	srvDesc.Texture2D.MipLevels = 1;
////	srvDesc.Texture2D.MostDetailedMip = 0;
////	g_pd3dDevice->CreateShaderResourceView(output_texture, &srvDesc, &output_source_view);
////
////	return true;
////}
////void render_d3d()
////{
////	ImGui::Render();
////	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
////	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
////	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
////	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
////	g_pSwapChain->Present(1, 0); // Present with vsync
////}
