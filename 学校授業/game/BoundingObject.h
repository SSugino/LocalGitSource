/** @file
 * �e�o�E���e�B���O(�Փ�)�I�u�W�F�N�g�̕\���N���X�̒�`
 * @date 2009/06/05
 * @author ����
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../math/Matrix.h"
#include "../math/Color.h"
#include "Collision.h"
#include "Object3D.h"

namespace sugino{
/** �Q�[���̗̈� */
namespace game{

	class Object3D;//����`

	/** �o�E���f�B���O�I�u�W�F�N�g���\���N���X */
	class BoundingObject{
	private:
		static Object3D box,sphere;

	public:
		/** �~�`��
		 * @param [out]		IRender			*r	:�\�������������_�����O 
		 * @param [in] BoundingCircle2D		&b	:�Փ˔��茳
		 * @param [in]		Matrix3x3		&m	:�s��
		 * @param [in]		DWORD			col	:�F
		 **/
		static	void Draw( graphics::IRender *render,BoundingCircle2D &bobj,math::Matrix3x3 &m,DWORD col );
		/**	���`��
		 * @param [out]		IRender			*r	:�\�������������_�����O 
		 * @param [in] BoundingCircle2D		&b	:�Փ˔��茳
		 * @param [in]		Matrix3x3		&m	:�s��
		 * @param [in]		DWORD			col	:�F
		 **/
		static	void Draw( graphics::IRender *render,BoundingLine2D &bobj,math::Matrix3x3 &m,DWORD col );
		/** AABB�`��
		 * @param [out]		IRender			*r	:�\�������������_�����O 
		 * @param [in] BoundingCircle2D		&b	:�Փ˔��茳
		 * @param [in]		Matrix3x3		&m	:�s��
		 * @param [in]		DWORD			col	:�F
		 **/
		static	void Draw( graphics::IRender *render,BoundingBox2D &bobj,math::Matrix3x3 &m,DWORD col );
		/** OBB�`��
		 * @param [out]		IRender			*r	:�\�������������_�����O 
		 * @param [in] BoundingCircle2D		&b	:�Փ˔��茳
		 * @param [in]		Matrix3x3		&m	:�s��
		 * @param [in]		DWORD			col	:�F
		 **/
		static	void Draw( graphics::IRender *render,BoundingOBox2D &bobj,math::Matrix3x3 &m,DWORD col );
	
		//----------------------------------------------------------------------------------
		
		static  void Draw( BoundingSphere3D &sp,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingBox3D &bbox,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingOBox3D &obox,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingLine3D &bl,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );

	};

}}