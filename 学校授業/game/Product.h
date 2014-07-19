/** @file
 * ���o�N���X�̒�`
 * @date 2009/08/27
 * @author ����
 **/

#pragma once
#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

namespace sugino{
namespace game{
	/** �|�����C���N���X(�R�c��Ԃ̃��C��) */
	class Poly{

	public:
		int PolyLine( graphics::IRender *render,math::Vector3 &s,math::Vector3 &e,float w,math::Vector3 &cpos, math::Matrix4x4 &vps );

		void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,
			math::Vector2 &t1,math::Vector2 &t2,math::Vector2 &t3,math::Vector2 &t4,math::Matrix4x4 &vps );
	};

	/** �|�����X�g�N���X */
	class PolyList{
	private:
		struct PolyPoint{
			math::Vector3 v1,v2;		//�_
			math::Color		c;			//�F
		};
		std::list< PolyPoint >	plist;	//v1,v2 ��_�̃��X�g
	public:
		/** ���X�g�ɓ_��ǉ� */
		void addPoint( math::Vector3 &p1, math::Vector3 &p2, math::Color &col );

		void Move();

		void Draw( graphics::IRender *render,math::Matrix4x4 &vps);
		

		static void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,
			math::Vector2 &t1,math::Vector2 &t2,math::Vector2 &t3,math::Vector2 &t4,math::Matrix4x4 &vps );

		static void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,math::Matrix4x4 &vps );

	};
}}