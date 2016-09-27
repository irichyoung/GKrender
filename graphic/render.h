#ifndef __GK_GRAPHIC_RENDER__
#define __GK_GRAPHIC_RENDER__
#include<Windows.h>
#include<d3d11.h>
#include<dxgi1_3.h>
#include <d3dcompiler.h>
#include<list>
#include"..\base\macros.h"
#include"..\base\singleton.h"
#include"..\resource.h"
#include"..\base\gkdebug.h"
#include"camera.h"
namespace gk{
	namespace graphic{
		struct percbuff
		{
			DirectX::XMMATRIX m_world;
			DirectX::XMMATRIX m_view;
			DirectX::XMMATRIX m_proj;
		};
		struct inscbuff
		{
			DirectX::XMFLOAT4 m_diffuse;
		};
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
			GET_SET(ID3D11InputLayout*, inputlayout);
			GET_SET(DXGI_MODE_DESC *, mode);
			GET_SET_STATIC(bool, windowed);
			GET_SET(camera *, currentcamera);
			GET_SET(ID3D11Buffer*, m_percbuff);
			GET_SET(ID3D11Buffer*, m_inscbuff);
			GETP_SET(ID3D11SamplerState*, m_sampler);
			GET_SET(ID3D11RasterizerState*, m_raster);
		public:
			static LRESULT CALLBACK gkProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
			bool createWindow(UINT x = 800, UINT y = 600);
			bool initialDevice();
		};
	}
}
#endif