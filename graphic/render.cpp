#include"render.h"
bool render::fullscreen;
LRESULT CALLBACK render::gkproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		fullscreen = !fullscreen;
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
bool render::createwindow(UINT x, UINT y)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = gkproc;
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

	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	render::getinstance()->setwindow(CreateWindow(L"gkwindow", L"Direct3D 11 Tutorial 7", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
		NULL));
	if (!render::getinstance()->getwindow())
		return false;

	ShowWindow(window, 10);
	return true;
}
bool render::initialdevice()
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
		else GKGetError();
	}
	IDXGIFactory2 *fac;
	if (S_OK != CreateDXGIFactory2(NULL, __uuidof(IDXGIFactory2), (void **)&fac))
		GKGetError();
	for (int i = 0;; i++){
		IDXGIAdapter1 *adp;
		fac->EnumAdapters1(i, &adp);
		if (adp == NULL)break;
		DXGI_ADAPTER_DESC1 des;
		adp->GetDesc1(&des);
		IDXGIOutput* pOutput = NULL;
		HRESULT hr;

		hr = adp->EnumOutputs(0, &pOutput);
		if (!pOutput){
			continue;
		}
		UINT numModes = 0;
		DXGI_MODE_DESC* displayModes = NULL;
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

		hr = pOutput->GetDisplayModeList(format, 0, &numModes, NULL);
		mode = new DXGI_MODE_DESC[numModes];
		hr = pOutput->GetDisplayModeList(format, 0, &numModes, displayModes);
		if (pOutput)
			pOutput->Release();
		if (adp)
			adp->Release();
	}
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
	swapChainDesc.Width = static_cast<UINT>(0); // Match the size of the window.
	swapChainDesc.Height = static_cast<UINT>(0);
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // This is the most common swap chain format.
	swapChainDesc.Stereo = false;
	swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2; // Use double-buffering to minimize latency.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // All Windows Store apps must use this SwapEffect.
	swapChainDesc.Flags = 0;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
	fullScreenDesc.RefreshRate = mode[0].RefreshRate;
	fullScreenDesc.Scaling = mode[0].Scaling;
	fullScreenDesc.ScanlineOrdering = mode[0].ScanlineOrdering;
	fullScreenDesc.Windowed = fullscreen;
	if (S_OK != fac->CreateSwapChainForHwnd(device, window, &swapChainDesc, &fullScreenDesc, NULL, &swapchain))
		GKGetError();
	return true;
}