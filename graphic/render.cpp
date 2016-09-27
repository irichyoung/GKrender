#include"render.h"
#include<Windows.h>
using namespace gk::graphic;
bool render::windowed = true;

LRESULT CALLBACK render::gkProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYUP:
	{
		if (wParam == 'W'){
			auto x = gk::graphic::render::getinstance()->getcurrentcamera();
			x->setPos(x->getx(), x->gety(), x->getz() + 1);
			GKDebug("z + 1");
		}
		if (wParam == 'S'){
			auto x = gk::graphic::render::getinstance()->getcurrentcamera();
			x->setPos(x->getx(), x->gety(), x->getz() - 1);
			GKDebug("z - 1");
		}
		windowed = !windowed;
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
bool render::createWindow(UINT x, UINT y)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wcex;
	setwidth(x);
	setheight(y);
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = gkProc;
	wcex.cbClsExtra = 0;	
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"gkwindow";
	wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, getwidth(), getheight() };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	render::getinstance()->setwindow(CreateWindow(L"gkwindow", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL));
	if (!render::getinstance()->getwindow())
		return false;

	ShowWindow(window, 10);
	return true;
}
bool render::initialDevice()
{
	D3D_DRIVER_TYPE dtype[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	D3D_FEATURE_LEVEL flevel[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	for (UINT drivernum = 0; drivernum < ARRAYSIZE(dtype); drivernum++){
		if (S_OK == D3D11CreateDevice(NULL, dtype[drivernum], NULL, NULL, flevel, ARRAYSIZE(flevel), D3D11_SDK_VERSION, &device, NULL, &context))
			break;
		else GKDebug("line:%d,fuction:%s,error:%s\n", __LINE__, __FUNCTION__, GKGetError(true).c_str());
	}
	IDXGIFactory2 *fac;
	FUN_ERR(CreateDXGIFactory2(NULL, __uuidof(IDXGIFactory2), (void **)&fac));
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	swapChainDesc.Width = static_cast<UINT>(0); // Match the size of the window.
	swapChainDesc.Height = static_cast<UINT>(0);
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // This is the most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // All Windows Store apps must use this SwapEffect.
	swapChainDesc.Flags = 0;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
	fullScreenDesc.RefreshRate.Denominator = 1;
	fullScreenDesc.RefreshRate.Numerator = 60;
	fullScreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	fullScreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullScreenDesc.Windowed = true;
	HRESULT hr = 0;
	FUN_ERR(fac->CreateSwapChainForHwnd(device, window, &swapChainDesc, &fullScreenDesc, NULL, &swapchain));
	FUN_ERR(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&rendertarget));
	FUN_ERR(device->CreateRenderTargetView(rendertarget, NULL, &rtview));

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = getwidth();
	descDepth.Height = getheight();
	if (getwidth() < 0 || getheight() < 0){
		GKDebug("Please set windows area with createwindow()");
		return false;
	}
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	FUN_ERR(device->CreateTexture2D(&descDepth, NULL, &depthstencil));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;


	FUN_ERR(device->CreateDepthStencilView(depthstencil, &descDSV, &dsview));
	context->OMSetRenderTargets(1, &rtview, dsview);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(percbuff);
	bd.CPUAccessFlags = 0;
	bd.Usage = D3D11_USAGE_DEFAULT;
	FUN_ERR(device->CreateBuffer(&bd, NULL, &m_percbuff));
	bd.ByteWidth = sizeof(inscbuff);
	FUN_ERR(device->CreateBuffer(&bd, NULL, &m_inscbuff));

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	device->CreateRasterizerState(&rasterDesc, &m_raster);

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	FUN_ERR(device->CreateSamplerState(&sampDesc, &m_sampler));

	return true;


}