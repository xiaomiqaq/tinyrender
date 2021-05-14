#include "RenderWindow.h"
UIRenderWindow::UIRenderWindow(framebuffer_t* framebuffer, ID3D11DeviceContext* context)
{
	this->framebuffer = framebuffer;
	this->g_pd3dDeviceContext = context;
	window_flags |= ImGuiWindowFlags_NoMove;
}
void UIRenderWindow::draw()
{
	ImGui::Begin("RenderWindow", &open, window_flags);
	{
		ZeroMemory(&subData, sizeof(subData));
		g_pd3dDeviceContext->Map(output_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);
		memcpy(subData.pData, framebuffer->color_buffer, sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
		g_pd3dDeviceContext->Unmap(output_texture, 0);
	}
	ImGui::Image(output_source_view, ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImVec2(1, 1), ImVec2(0, 0), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 2.5f));
	ImGui::End();
}