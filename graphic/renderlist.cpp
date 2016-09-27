#include"renderlist.h"
using namespace gk::graphic;
void renderlist::addObject(mesh *m){
	m_list.push_back(m);
}
bool renderlist::renderAll(){
	auto render = render::getinstance();
	float color[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
	m_render->getcontext()->ClearRenderTargetView(m_render->getrtview(), color);
	m_render->getcontext()->ClearDepthStencilView(m_render->getdsview(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	for (auto &x: m_list){
		if (!x->draw())
			GKDebug("Wrong draw");
	}
	FUN_ERR(render->getswapchain()->Present(0, 0));
	return true;
}