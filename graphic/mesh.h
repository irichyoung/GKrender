#ifndef __GK_GRAPHIC_MESH__
#define __GK_GRAPHIC_MESH__
#include<vector>
#include<stdint.h>
#include<d3d11.h>
#include<DirectXMath.h>
#include"render.h"
#include"cgprogram.h"
#include"..\base\gkdebug.h"
#include"..\base\macros.h"
#include"..\assimp\Importer.hpp"
#include"..\assimp\scene.h"
#include"..\assimp\postprocess.h"
#include"..\assimp\texture.h"
namespace gk{
	namespace graphic{
		enum class PRIMITIVE{
			GK_PRIMITIVE_POINT,
			GK_PRIMITIVE_LINE,
			GK_PRIMITIVE_TRIANGLE
		};
		struct vertex{
			float posx,posy,posz;
			float norx,nory,norz;
			float diffuser, diffuseg, diffuseb, diffusea;
			float texu,texv;
		};
		struct texture{
			std::string m_file;
		};
		struct submesh;
		class mesh{
			GET_SET(float, m_xpos); 
			GET_SET(float, m_ypos);
			GET_SET(float, m_zpos);
			std::string			  meshname;
			std::vector<submesh*>m_submesh;
			render          	 *m_render;
		public:
			mesh(submesh&s){
				m_submesh.push_back(&s);
			}
			mesh(){};
			bool load(const char*,float x,float y,float z);
			bool loadTexture(const std::wstring&, submesh *);
			bool draw();
		};
		struct submesh{
			float diffuser, diffuseg, diffuseb, diffusea;
			PRIMITIVE               m_primitive;
			std::vector<vertex>		   m_vertex;
			std::vector<UINT>		    m_index;
			ID3D11Buffer		  *m_vertexbuff;
			ID3D11Buffer		   *m_indexbuff;
			mesh                        *m_mesh;
			cgprogram                *m_program;
			ID3D11ShaderResourceView *m_srv;
			ID3D11Resource           *m_tex;
			bool					 m_hastex;
			submesh():m_hastex(false){};
		};
	}
}
#endif