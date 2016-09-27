#ifndef __GK_GRAPHIC_CAMERA__
#define __GK_GRAPHIC_CAMERA__
#include<DirectXMath.h>
#include<base\macros.h>
#include<Windows.h>
namespace gk{
	namespace graphic{
		class camera
		{
		protected:
			DirectX::XMFLOAT3 m_pos;
			DirectX::XMFLOAT3 m_up;
			DirectX::XMFLOAT3 m_focus;
			DirectX::XMFLOAT4X4 m_view;
			DirectX::XMFLOAT4X4 m_proj;
			GET_SET(float, fov);
			bool view;
			bool proj;
		public:
			camera() :view(false), proj(false),fov(DirectX::XM_PIDIV2){};
			virtual DirectX::XMMATRIX getViewMatrix();
			DirectX::XMMATRIX getProjMatrix();
			void setPos(float, float, float);
			float getx(){ return m_pos.x; };
			float gety(){ return m_pos.y; };
			float getz(){ return m_pos.z; };
			void  setx(float x){
				m_pos.x = x;
				view = false;
			}
			void sety(float y){
				m_pos.y = y;
				view = false;
			}
			void setz(float z){
				m_pos.z = z;
				view = false;
			}
			void setUp(float, float, float);
			void setFocus(float, float, float);
		};
		class modelcamera:public camera
		{
			GET_SET(float,m_dis);
			GET_SET(float,m_speed);
			GET_SET(bool,spin);
			static DWORD m_time¡£;
		public:
			modelcamera(float d, float s,float x,float y,float z) :m_dis(d), m_speed(s){
				m_pos.x = x;
				m_pos.y = y;
				m_pos.z = z;
			};
			virtual DirectX::XMMATRIX getViewMatrix()override;
		};
	}
}
#endif