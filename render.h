#ifndef __GK_GRAPHIC_RENDER__
#define __GK_GRAPHIC_RENDER__
#include<Windows.h>
#include<d3d11.h>
#include<dxgi1_3.h>
#include"macros.h"
#include"singleton.h"
#include"resource.h"
class render
{
	singleton(render);
	GET_SET(HWND, window);
	GET_SET(ID3D11Device *,device);
	GET_SET(IDXGISwapChain1 *,swapchain);
	GET_SET(ID3D11DeviceContext *, context);
	GET_SET(DXGI_MODE_DESC *, mode);
	GET_SET_STATIC(bool, fullscreen);
public:
	static LRESULT CALLBACK gkproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	bool createwindow(UINT x = 800, UINT y = 600);
	bool initialdevice();
};

#endif