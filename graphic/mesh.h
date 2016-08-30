#ifndef __GK_GRAPHIC_MESH__
#define __GK_GRAPHIC_MESH__
#include<vector>
#include<stdint.h>
#include<d3d11.h>
#include"render.h"
#include"d3dx11effect.h"
#include<assimp\Importer.hpp>
#include<assimp\scene.h>
#include<assimp\postprocess.h>
#include<assimp\texture.h>
namespace gk{
	namespace graphic{
		struct vertex{
			float posx,posy,posz;
			float norx,nory,norz;
			float texu,texv;
		};
		struct texture{
			std::string m_file;
		};
		struct submesh{
			std::vector<vertex>		   m_vertex;
			std::vector<uint32_t>		m_index;
			ID3D11Buffer		  *m_vertexbuff;
			ID3D11Buffer		   *m_indexbuff;
			ID3D11ShaderResourceView     *m_tex;
			UINT32			         m_matindex;
			std::wstring			 effectname;
			ID3DX11Effect             *m_effect;
		};
		class mesh{
			std::string			  meshname;
			std::vector<submesh*>m_submesh;
			std::vector<aiMaterial*> m_mat;
			render          	 *m_render;
		public:
			bool load(const char*);
			bool draw();
			bool add();
			bool setEffect(submesh *,std::wstring);
		};
	}
}
#endif