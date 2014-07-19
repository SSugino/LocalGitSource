/** @file
 * �o�E���e�B���O(�Փ�)�N���X�̒�`
 * @date 2009/06/05
 * @author ����
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Float.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

namespace sugino{
namespace game{
	/** �o�E���f�B���O�I�u�W�F�N�g�̒�` */

	/** �o�E���f�B���O�T�[�N�� */
	struct BoundingCircle2D
	{
		/** ���S�_ */
		math::Vector2 p;
		/** ���a */
		float	s;
		BoundingCircle2D(){}
		/** ������
		 * @param[in] Vector2	&v	:�ʒu�x�N�g���𒆐S�_�����ēn��
		 * @param[in] float		_s	:���a
		 **/
		BoundingCircle2D( math::Vector2 &v,float _s ) : p(v),s(_s) {}
	};
	
	/** �o�E���f�B���O���C�� */
	struct BoundingLine2D
	{
		/** �n�_�E�I�_ */
		math::Vector2 s,e;

		BoundingLine2D(){};
		/** ������
		 * @param[in] Vector2	&st	:�n�_�x�N�g��
		 * @param[in] Vector2	&ed	:�I�_�x�N�g��
		 **/
		BoundingLine2D( math::Vector2 &st,math::Vector2 &ed ){ s=st; e=ed ;}
	};

	/** �o�E���f�B���O�{�b�N�X( Axis_Aligh_Bounding_Box ) AABB */
	struct BoundingBox2D
	{
		/** ���S�_ */
		math::Vector2 p;
		/** �傫�� */
		math::Vector2 s;

		BoundingBox2D(){}
		/** ������
		 * @param[in] Vector2	&center	:���S�x�N�g��
		 * @param[in] Vector2	&size	:�T�C�Y�x�N�g��
		 **/
		BoundingBox2D( math::Vector2 &center,math::Vector2 &size ){ p=center, s=size; }
	};
	
	/** �o�E���f�B���O��]�{�b�N�X( Oriented_Bounding_Box ) OBB */
	struct BoundingOBox2D
	{
		/** ���S�_ */
		math::Vector2 p;
		/** �傫�� */
		math::Vector2 s;
		/** ��]�� Y���ɂȂ� */
		math::Vector2 r;
		
		BoundingOBox2D(){}
		/** ������
		 * @param[in] Vector2	&center	:���S�x�N�g��
		 * @param[in] float		rot		:Y��(�P�ʃx�N�g��)
		 * @param[in] Vector2	&size	:�T�C�Y�̃x�N�g��
		 **/
		BoundingOBox2D( math::Vector2 &center,float rot,math::Vector2 &size )
		{ 
			p = center;
			r = math::Vector2( rot ).Rotate90();
			s = size;
		}

		/** ������
		 * @param[in] Vector2	&center	:���S�x�N�g��
		 * @param[in] Vector2	rot		:Y���x�N�g�� ( [0,1]�Ƃ��œn��)	(�P�ʃx�N�g��)
		 * @param[in] Vector2	&size	:�T�C�Y�̃x�N�g��
		 **/
		BoundingOBox2D( math::Vector2 &center,math::Vector2 rot,math::Vector2 &size )
		{
			p = center;
			s = size;
			r = rot;
		}
	};
	
	/** 2D�̐ڐG����N���X�̒�` */
	class Collision2D
	{
	public:
		/** �ڐG���� �~�Ɖ~
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingCircle2D &b );
		/** �ڐG���� �l�p�Ǝl�p
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox2D &a,BoundingBox2D &b );
		/** �ڐG���� ���Ɛ�
		 * return true : Hit
		 **/
		static bool Collision( BoundingLine2D &a,BoundingLine2D &b );
		/** �ڐG���� �~�Ɛ�
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingLine2D &b );
		/** �ڐG���� �~�Ǝl�p (AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingBox2D &b );
		/** �ڐG���� �~�Ɖ�]�� (OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingOBox2D &b );
		/** �ڐG���� ��]���Ɖ�]�� (OBB*OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox2D &a,BoundingOBox2D &b );
		/** �ڐG���� ��]���Ǝl�p(OBB*AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox2D &a,BoundingBox2D &b );
		
	};

	/** ����(�����Ԃ�)����N���X�̒�` */
	class Intersect2D
	{
	public:
		/** �������� �~�Ɖ~
		 * return true : Hit
		 * @param[out] :
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingCircle2D &b,math::Vector2 *ret );
		/** �������� �l�p�Ǝl�p
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox2D &a,BoundingBox2D &b,math::Vector2 *ret );
		/** �������� �~�Ɛ�
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingLine2D &b,math::Vector2 *ret );
		/** �������� �~�Ǝl�p(AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingBox2D &b,math::Vector2 *ret );
		/** �������� �~�Ɖ�]��(OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingOBox2D &b,math::Vector2 *ret );
		/** �������� ��]���Ɖ�]��(OBB*OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox2D &a,BoundingOBox2D &b,math::Vector2 *ret );
		/** �������� ��]���Ǝl�p(OBB*AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox2D &a,BoundingBox2D &b,math::Vector2 *ret );

	};

	//--------------------------------------------------------------------------------------
	/** ���̂�3D�����蔻�� */
	class BoundingSphere3D
	{
	public:
		math::Vector3	p;	//���S�_
		float			s;	//�傫��
		BoundingSphere3D(){}
		BoundingSphere3D( math::Vector3 &center,float size ){ p=center; s=size; }
	};

	/** ����3D�����蔻�� */
	class BoundingBox3D
	{
	public:
		math::Vector3 p;	//���S�_
		math::Vector3 s;	//�傫��
		BoundingBox3D(){}
		BoundingBox3D( math::Vector3 &center,math::Vector3 &size ){ p=center;s=size; }
	};

	/** ��]���̂R�c�����蔻�� */
	class BoundingOBox3D
	{
	public:
		math::Vector3 p,s;	//���S�_�A�傫��
		math::Vector3 ax,ay,az;	//�e��
		BoundingOBox3D(){}
		BoundingOBox3D( math::Vector3 &c,math::Vector3 &size,math::Matrix4x4 &rot ){
			p = c;s = size;
			ax.x = rot.m00;ax.y = rot.m01; ax.z = rot.m02;
			ay.x = rot.m10;ay.y = rot.m11; ay.z = rot.m12;
			az.x = rot.m20;az.y = rot.m21; az.z = rot.m22;
		}
	};

	/** ���i�J�v�Z���^)�̂R�c�����蔻�� */
	class BoundingLine3D
	{
	public:
		math::Vector3 s,e;	//�n�_�@�I�_
		float		width;	//��

		BoundingLine3D(){}
		BoundingLine3D( math::Vector3 &st,math::Vector3 &ed,float w )
		{
			s = st;e = ed;width = w;
		}
	};
	
	//----------------------------------------------------------------------------

	/** 3D�̐ڐG����N���X�̒�` */
	class Collision3D
	{
	public:
		/** �ڐG���� ���Ƌ�
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingSphere3D &b );
		/** �ڐG���� �l�p�Ǝl�p
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox3D &a,BoundingBox3D &b );
		/** �ڐG���� ���Ɛ�
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingLine3D &b );

		/** �ڐG���� ���ƃ��C�i����)
		 * @param [in] BoundingSphere &a,	����p����
		 * @param [in] Vector3		  &pos,	�����J�n�ʒu
		 * @param [in] Vector3		  &ray,	����
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,math::Vector3 &pos,math::Vector3 &ray );
		
		/** �ڐG���� ���Ǝl�p (AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingBox3D &b );
		
		/** �ڐG���� ���Ɖ�]�� (OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingOBox3D &b );
		
		/** �ڐG���� ��]���Ɖ�]�� (OBB*OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox3D &a,BoundingOBox3D &b );
		
		/** �ڐG���� �l�p�ƃ��C(AABB*Ray)
		 * @param [out]	Vevtor3	&ray	:���C�̕���
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox3D &a,math::Vector3 &pos,math::Vector3 &ray );

		/** �ڐG���� ��]�l�p�ƃ��C(OBB*Ray)
		 * @param [out]	Vector3	&pos1	:�����̍��W
		 * @param [out] Vector3	&ray1	:���C�̕���
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox3D &a,math::Vector3 &pos1,math::Vector3 &ray1 );
	
		/** ���肷�鎲�Ǝ���̋������w�肵�āA2��OBB���Փ˂��Ă��邩���ׂ�
		 * @param [in] Vector3	&axis		:�C�ӂ̎�
		 * @param [in] Vector3	&dist		:����
		 * @param [in] BoundingOBox3D &a	:�n�a�a�P
		 * @param [in] BoundingOBox3D &b	:�n�a�a�Q
		 * @param [out] float *length		:�␳��
		 * @param [out]	float *sign			:�␳����
		 * @return length					:�␳��
		 **/
		static bool AxisLength( math::Vector3 &axis, math::Vector3 &dist, BoundingOBox3D &a, BoundingOBox3D &b,float *length,float *sign );

	};

	/** ����(�����Ԃ�)����N���X�̒�` */
	class Intersect3D
	{
	public:
		/** �������� ���Ƌ�
		 * @param [in] Vector3	*ret	:�␳��	
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingSphere3D &b,math::Vector3 *ret );
		
		/** �������� �l�p�Ǝl�p
		 * @param [in] Vector3	*ret	:�␳��
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox3D &a,BoundingBox3D &b,math::Vector3 *ret );
		
		/** �������� ���Ɛ�
		 * @param [in] Vector3	*ret	:�␳��
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingLine3D &b,math::Vector3 *ret );
		
		/** �������� ���Ɛ�
		 * @param [in]	Vector3	*ret			:�␳��
		 * @param [in]	Vector3	*cross_pos		:��_
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingLine3D &b,math::Vector3 *ret,math::Vector3 *cross_pos );

		/** �������� ���ƃ��C(����)
		 * @param [in] BoundingSphere3D		&a			:����Ώۂ̋�
		 * @param [in] Vector3				&pos		:�����J�n�����W
		 * @param [in] Vector3				&ray		:����
		 * @param [in] Vector3				*cross_pos	:��_	���␳���W�łȂ�
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,math::Vector3 &pos,math::Vector3 &ray,math::Vector3 *cross_pos );
		
		/** �������� �~�Ɣ�(AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingBox3D &b,math::Vector3 *ret );
		
		/** �������� ���Ɖ�]��(OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingOBox3D &b,math::Vector3 *ret );
		/** �������� ��]���Ɖ�]��(OBB*OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox3D &a,BoundingOBox3D &b,math::Vector3 *ret );
		
		/** �������� �l�p�ƃ��C
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox3D &a,math::Vector3 &pos,math::Vector3 &ray,math::Vector3 *ret );
		
		/** �������� ��]�l�p�ƃ��C
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox3D &a,math::Vector3 &pos1,math::Vector3 &ray1,math::Vector3 *ret );

	};

	/** ������J�����O�i��ʓ��ɃI�u�W�F�N�g�����邩�H/�f�o�b�O�ł͈̔͊m�F�p�j */
	class Frustum{
	public:
	
		math::Vector3	p[8];		//near��far��4�_�~2
		math::Plane pnear,pfar;		//��O�A��
		math::Plane left,right;		//���A�E
		math::Plane top,bottom;		//��A��
		
		//Frustum( math::Vector3 &point,
		
		/** ������̑�`�쐬
		 * @param [in] Matrix4x4	&view		:�J�����̌��Ă�s��
		 * @param [in] float		wid			:��
		 * @param [in] float		hei			:����
		 * @param [in] float	near_len		:��O�̒���
		 * @param [in] float	far_len			:���̒���
		 **/
		void Create( math::Matrix4x4 &view,float wid,float hei,float near_len,float far_len );	//������

		void Draw( graphics::IRender *r,math::Matrix4x4 &vps,math::Vector3 &dir,math::Color &col );
		
		/** �t���X�^���̐ڐG���� */
		bool Collision( BoundingSphere3D &sp );
		

		bool Collision( BoundingOBox3D &obox );

	};
}}

