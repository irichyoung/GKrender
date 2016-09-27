#ifndef __GK_GRAPHIC_CGPROGRAM__
#define __GK_GRAPHIC_CGPROGRAM__
#include<d3d11.h>
#include<string>
#include<DirectXMath.h>
#include"render.h"
#include"..\base\gkdebug.h"
#include"..\base\macros.h"
namespace gk{
	namespace graphic{

		class cgprogram
		{
			GET_SET(ID3D11VertexShader *,m_vs);
			GET_SET(ID3D11GeometryShader *,m_gs);
			GET_SET(ID3D11PixelShader *,m_ps);
			GET_SET(ID3DBlob   *,m_vb);
			GET_SET(ID3DBlob   *,m_gb);
			GET_SET(ID3DBlob   *,m_pb);
			GETP_SET(std::wstring,m_filename);
		public:
			bool load(const wchar_t *);
		};
	}
}
#endif