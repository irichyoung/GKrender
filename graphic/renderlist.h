#ifndef __GK_GRAPHIC_RENDERLIST__
#define __GK_GRAPHIC_RENDERLIST__
#include<list>
#include"mesh.h"
namespace gk{
	namespace graphic{
		class renderlist
		{
			std::list<mesh*>   m_list;
			render          *m_render;
		public:
			renderlist(){
				m_render = render::getinstance();
			};
			~renderlist(){
				m_list.clear();
			};
			void addObject(mesh*);
			bool renderAll();
		};
	}
}
#endif