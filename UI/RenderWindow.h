#include "imgui.h"
#include "../base/framebuffer.h"
class M_UI
{
public:
	virtual void draw()=0;

};
class UIRenderWindow:public M_UI
{
public:
	UIRenderWindow(framebuffer_t* framebuffer, ID3D11DeviceContext* context);

	void draw();

	
private:
	ImGuiWindowFlags window_flags = 0;
	bool open = true;
	framebuffer_t* framebuffer;
	ID3D11DeviceContext*     g_pd3dDeviceContext;
	D3D11_MAPPED_SUBRESOURCE subData;

};