#include"cgprogram.h"
#include<d3dcompiler.h>
using namespace gk::graphic;
bool cgprogram::load(const wchar_t *name){
	FUN_ERR(D3DCompileFromFile(name, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &m_vb, NULL));
	FUN_ERR(D3DCompileFromFile(name, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_4_0", D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG, 0, &m_pb, NULL));
	auto r = render::getinstance();
	m_filename = std::wstring(name);
	FUN_ERR(r->getdevice()->CreateVertexShader(m_vb->GetBufferPointer(), m_vb->GetBufferSize(), NULL, &m_vs));
	FUN_ERR(r->getdevice()->CreatePixelShader(m_pb->GetBufferPointer(), m_pb->GetBufferSize(), NULL, &m_ps));
	return true;
}