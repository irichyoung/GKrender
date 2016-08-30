#include"render.h"
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
	fullScreenDesc.Windowed = windowed;
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
	FUN_ERR(device->CreateDepthStencilView(depthstencil, NULL, &dsview));
	context->OMSetRenderTargets(1, &rtview, dsview);

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1, &vp);

	return true;
}
//bool render::setShader(const WCHAR*filename){
//	//ID3DBlob* pVSBlob = NULL;
//	//FUN_ERR(CompileShaderFromFile(const_cast<WCHAR*>(filename), "VS", "vs_4_0", &pVSBlob));
//
//	// Create the vertex shader
//	//FUN_ERR(device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vs));
//
//	// Define the input layout
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//	{
//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOOR",0, DXGI_FORMAT_R32G32_UINT,0,24,D3D11_INPUT_PER_VERTEX_DATA,0}
//	};
//	UINT numElements = ARRAYSIZE(layout);
//
//	// Create the input layout
//	FUN_ERR(device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
//		pVSBlob->GetBufferSize(), &inputlayout));
//	pVSBlob->Release();
//
//	// Set the input layout
//	context->IASetInputLayout(inputlayout);
//
//	// Compile the pixel shader
//	ID3DBlob* pPSBlob = NULL;
//	FUN_ERR(CompileShaderFromFile(const_cast<WCHAR*>(filename), "PS", "ps_4_0", &pPSBlob));
//
//	// Create the pixel shader
//	FUN_ERR(device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &ps));
//	pPSBlob->Release();
//}

void render::addObject(mesh *m){
	renderlist.push_back(m);
}
bool render::renderAll(){
	for (auto x : renderlist){
		x->draw();
	}
	swapchain->Present(0, 0);
}