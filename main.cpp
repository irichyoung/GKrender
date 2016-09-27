#include"graphic\render.h"
#include"graphic\mesh.h"
#include"graphic\renderlist.h"
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto render = gk::graphic::render::getinstance();
	render->createWindow();
	render->initialDevice();
	gk::graphic::mesh table;
	gk::graphic::mesh plane;
	table.load("D://gk.dae", 5, 0, 0);
	plane.load("F://model.dae", 0, 0, 0);
	gk::graphic::modelcamera cam(10,1,0,0,-10);
	render->setcurrentcamera(&cam);
	render->getcurrentcamera()->setFocus(0, -1, 0);
	render->getcurrentcamera()->setPos(0, 3, -5);
	render->getcurrentcamera()->setUp(0, 0, 0);
	gk::graphic::renderlist rlist;
	rlist.addObject(&table);
	rlist.addObject(&plane);
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
			rlist.renderAll();
		}
	}
	return (int)msg.wParam;
}