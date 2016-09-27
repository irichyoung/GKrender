#include"camera.h"
#include"render.h"
using namespace gk::graphic;
DWORD modelcamera::m_time = 0;
DirectX::XMMATRIX camera::getViewMatrix(){
	if (!view){
		DirectX::XMStoreFloat4x4(&m_view, DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&m_pos), DirectX::XMLoadFloat3(&m_focus), DirectX::XMLoadFloat3(&m_up)));
		view = true;
	}
	return DirectX::XMLoadFloat4x4(&m_view);
}
DirectX::XMMATRIX camera::getProjMatrix(){

	if (!proj){
		auto x = gk::graphic::render::getinstance()->getwidth();
		auto y = gk::graphic::render::getinstance()->getheight();
		DirectX::XMStoreFloat4x4(&m_proj, DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, (float)x / (float)y, (float)0.1, 10000));
		proj = true;
	}
	return DirectX::XMLoadFloat4x4(&m_proj);
}
void camera::setFocus(float x, float y, float z){
	this->m_focus = DirectX::XMFLOAT3(x, y, z);
	view = false;
}
void camera::setPos(float x, float y, float z){
	m_pos = DirectX::XMFLOAT3(x, y, z);
	view = false;
}
void camera::setUp(float x, float y, float z){
	m_up = DirectX::XMFLOAT3(x, y, z);
	view = false;
}
DirectX::XMMATRIX modelcamera::getViewMatrix(){
	DWORD timecurr = GetTickCount();
	if (m_time == 0){
		m_time = timecurr;
	}
	const DirectX::XMFLOAT3 axis(0, 1, 0);
	float t = (timecurr - m_time) / 1000.0f * getm_speed();
	GKDebug("%f\n", t);
	auto tem = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&m_pos), DirectX::XMMatrixTranslation(getm_dis(), 0.0f, 0.0f));
	auto npos = DirectX::XMVector3Rotate(tem, DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat3(&axis), t));
	DirectX::XMStoreFloat4x4(&m_view, DirectX::XMMatrixLookAtLH(npos, DirectX::XMLoadFloat3(&m_focus), DirectX::XMLoadFloat3(&m_up)));
	return DirectX::XMLoadFloat4x4(&m_view);
}