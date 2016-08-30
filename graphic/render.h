#ifndef __GK_GRAPHIC_RENDER__
#define __GK_GRAPHIC_RENDER__
#include<Windows.h>
#include<d3d11.h>
#include<dxgi1_3.h>
#include <d3dcompiler.h>
#include<list>
#include"base/macros.h"
#include"base/singleton.h"
#include"resource.h"
#include"base/gkdebug.h"
#include"mesh.h"
namespace gk{
	namespace graphic{
		class render
		{
			singleton(render);
			GET_SET(HWND, window);
			GET_SET(UINT32, width);
			GET_SET(UINT32, height);
			GET_SET(ID3D11Device *, device);
			GET_SET(IDXGISwapChain1 *, swapchain);
			GET_SET(ID3D11DeviceContext *, context);
			GET_SET(ID3D11Texture2D *, rendertarget);
			GET_SET(ID3D11Texture2D *, depthstencil);
			GET_SET(ID3D11DepthStencilView*, dsview);
			GET_SET(ID3D11RenderTargetView *, rtview);
			GET_SET(ID3D11VertexShader*, vs);
			GET_SET(ID3D11PixelShader*, ps);
			GET_SET(ID3D11InputLayout*, inputlayout);
			GET_SET(DXGI_MODE_DESC *, mode);
			GET_SET_STATIC(bool, windowed);
			std::list<gk::graphic::mesh *> renderlist;
		public:
			static LRESULT CALLBACK gkProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			bool createWindow(UINT x = 800, UINT y = 600);
			bool initialDevice();
			//bool setShader(const WCHAR*);
			bool renderAll();
			void addObject(gk::graphic::mesh *);
		};
	}
}
#endif