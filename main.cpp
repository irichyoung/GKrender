#include"graphic\render.h"
#include"graphic\mesh.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto render = gk::graphic::render::getinstance();
	render->createWindow();
	render->initialDevice();
	gk::graphic::mesh table;
	table.load("F:\model.dea");
	table.add();
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
		}
	}
	return (int)msg.wParam;
}