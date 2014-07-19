/** @file
 * �s��N���X�̒�`
 * @date 2009/05/29
 * @author ����
 **/
#pragma once

#include "../system/Window.h"
#include "Vector.h"

namespace sugino{
namespace math{

	class Matrix4x4;
	//�ȗ�����������	
	typedef Matrix4x4 Matrix;	
	
	/** 3 �~ 3 �s�� */
	class Matrix3x3
	{
	public:
		union{
			/** �s�� */
			float mat[3][3];
			struct{
				float m00,m01,m02;
				float m10,m11,m12;
				float m20,m21,m22;
				};
			};

			Matrix3x3(){};
			/** ������
			 * @param [in] float x00 :�s��ւ̑���l
			 * @param [in] float x01 :�s��ւ̑���l
			 * @param [in] float x02 :�s��ւ̑���l
			 * @param [in] float x10 :�s��ւ̑���l
			 * @param [in] float x11 :�s��ւ̑���l
			 * @param [in] float x12 :�s��ւ̑���l
			 * @param [in] float x20 :�s��ւ̑���l
			 * @param [in] float x21 :�s��ւ̑���l
			 * @param [in] float x22 :�s��ւ̑���l			
			 */
			Matrix3x3( float x00,float x01,float x02,
					   float x10,float x11,float x12,
					   float x20,float x21,float x22 );
			
			/** ��]�E�X�P�[����\���x�x�N�g���ƈʒu�x�N�g���̍s��̏������@�����ړ���]�̏�����
			 * @param [out] Vector2 &up_axis : �X�P�[���x�x�N�g�� (x,y)
			 * @param [out] Vector2 &pos	 : �ʒu�x�N�g��(x,y)
			 * @return *this				 : ���������ꂽ�s��	 
			 **/
			Matrix3x3 &Initialize( Vector2 &up_axis,Vector2& pos ) 
			{
				m00 = up_axis.y, m01=-up_axis.x, m02 = 0;
				m10 = up_axis.x, m11= up_axis.y, m12 = 0;
				m20 = pos.x,	 m21= pos.y ,	 m22 = 1;

				return *this;
			};
			
			/** ��]�E�X�P�[����\���x�x�N�g���ƃJ�����ʒu�x�N�g���̍s��̏�����
			 * @param [out] Vector2 &ay		 : ��]�t�s�� (x,y)
			 * @param [out] Vector2 &pos	 : �t�ʒu�x�N�g��(x,y)
			 * @return *this				 : ���������ꂽ�s��
			 **/
			Matrix3x3 &View( Vector2 &ay,Vector2& pos ) 
			{
				const Vector2 &ax = ay.RotateInvert90();
				m00 = ax.x, m01= ay.x , m02 = 0;
				m10 = ax.y, m11= ay.y , m12 = 0;
				
				m20 = -( pos.x*m00 + pos.y*m10 );		//���]
				m21 = -( pos.x*m01 + pos.y*m11 );
				m22 = 1;

				return *this;
			};

			/** �s��|�����킹(*) */
			//�s�񓯎m�̊|���Z�͓��e�I�ɂ͕ϊ��������鎖(�����Z)�ɂȂ�
			Matrix3x3 operator * ( const Matrix3x3 &m ) const;
			
			/** �s��|�����킹(*=) */
			Matrix3x3 &operator *= ( const Matrix3x3 &m ){ return *this=*this *m; }

			/** �e�����o�����Z */
			Matrix3x3 operator + ( const Matrix3x3 &m ) const;
			
			/** �e�����o�����Z */
			Matrix3x3 operator - ( const Matrix3x3 &m ) const;
			
			/** �����{ */
			Matrix3x3 operator * ( const float f ) const;

			/** �P�ʍs�� */
			Matrix3x3 &Identity() { return *this=unit_matrix; }

			/** ��]�s��
			 * @param[in]	float r : ���W�A���l
			 **/
			Matrix3x3 &Rotate( float r );

			/** �X�P�[�����O�s��
			 * @param [in] float sx : �X�N���[�����W(0�`1.0)
			 * @param [in] float sy : �X�N���[�����W(0�`1.0)
			 **/ 
			Matrix3x3 &Scale( float sx,float sy );
			/** �X�P�[�����O�x�N�g���s��
			 * @param[out] Vector2 &s : �X�N���[���x�N�g�����W(0�`1.0)
			 * return Scale( s.x,s.y ): �X�N���[�����W
			 **/
			Matrix3x3 &Scale( const Vector2 &s ){ return Scale( s.x,s.y ); }

			/** �X�N���[���ϊ��s��
			 * @param [in] int w	:��ʕ�
			 * @param [in] int h	:��ʍ���
			 **/
			Matrix3x3 &Screen( int w,int h );

			/** ���s�ړ�
			 * @param[in] float x : ���s�ړ����W�� x (0.0�`1.0)
			 * @param[in] float y : ���s�ړ����W�� y (0.0�`1.0)
			 **/
			Matrix3x3 &Translate( float x,float y );
			
			/** ���s�ړ�
			 * @param[in] const Vector2 &v : �x�N�g�����W( x, y )
			 * @return ���s�ړ���̃x�N�g�����W( v.x,v.y )
			 **/
			Matrix3x3 &Translate( const Vector2 &v ){ return Translate( v.x,v.y ); }
			
			/** �]�u(�Ίp�ɓ���ւ�)�s��ɂ��� */
			Matrix3x3 &Transpose();
			
			/** �t�s��ɂ��� */
			Matrix3x3 &Inverse();

			/** �x�N�g��*�s��
			 * ���_(0,0)����̌��_�ύX
			 * @param [in] const Vector2 &v	: �x�N�g��(x,y)
			 * return �x�N�g���̃X�N���[���ϊ����W
			 **/
			inline Vector2 TransformVector( const Vector2 &v ) const
			{
				Vector2 v2;
				v2.x = v.x*m00 + v.y*m10 + m20;
				v2.y = v.x*m01 + v.y*m11 + m21;
	
				return v2;
			};

			/** �x�N�g��*��]�s��( 2*2 )
			 * @param [in] const Vector2 &v
			 * return �x�N�g���̉�]�ϊ��X�N���[�����W
			 **/
			inline Vector2 TransformRotation( const Vector2 &v )
			{
				Vector2 v2;
				v2.x = v.x*m00 + v.y*m10;
				v2.y = v.x*m01 + v.y*m11;
	
				return v2;
			};

			/**  �P�ʍs�� */
			static const Matrix3x3 unit_matrix;
	};

	/** 4 * 4 �s�� */
	class Matrix4x4
	{
	public:
		union{
			/** �s�� */
			float mat[4][4];
			struct{
				float m00,m01,m02,m03;
				float m10,m11,m12,m13;
				float m20,m21,m22,m23;
				float m30,m31,m32,m33;
				};
			};


			Matrix4x4(){};
			/** ������
			 * @param [in] float x00 :�s��ւ̑���l
			 * @param [in] float x01 :�s��ւ̑���l
			 * @param [in] float x02 :�s��ւ̑���l
			 * @param [in] float x03 :�s��ւ̑���l
			 * @param [in] float x10 :�s��ւ̑���l
			 * @param [in] float x11 :�s��ւ̑���l
			 * @param [in] float x12 :�s��ւ̑���l
			 * @param [in] float x13 :�s��ւ̑���l
			 * @param [in] float x20 :�s��ւ̑���l
			 * @param [in] float x21 :�s��ւ̑���l
			 * @param [in] float x22 :�s��ւ̑���l
			 * @param [in] float x23 :�s��ւ̑���l
			 * @param [in] float x30 :�s��ւ̑���l
			 * @param [in] float x31 :�s��ւ̑���l
			 * @param [in] float x32 :�s��ւ̑���l
			 * @param [in] float x33 :�s��ւ̑���l
			 */
			Matrix4x4( float x00,float x01,float x02,float x03,
					   float x10,float x11,float x12,float x13,
					   float x20,float x21,float x22,float x23,
					   float x30,float x31,float x32,float x33);
			
			/** �N�H�[�^�j�I������s�񏉊���(�S����) */
			//�R���X�g���N�^
			Matrix4x4( Quaternion &q )
			{
				const float &x = q.v.x,&y = q.v.y,&z = q.v.z;
				const float &w = q.w;
				
				float x2 = x+x , y2 =y+y  , z2 =z+z;

				float xx = x*x2, yy =y*y2 , zz =z*z2;
				float xy = x*y2, xz =x*z2 , wx =w*x2;
				float yz = y*z2, wy =w*y2 , wz =w*z2;

				m00=1-(yy+zz);	m01=xy+wz;		m02=xz-wy;		m03=0;
				m10=xy-wz;		m11=1-(xx+zz);	m12=yz+wx;		m13=0;
				m20=xz+wy;		m21=yz-wx;		m22=1-(xx+yy);	m23=0;
				m30=0;			m31=0;			m32=0;			m33=1;
			}


			/** ��]�E�X�P�[����\���ʒu�x�N�g���̍s��̏������@�����ړ���]�̏�����
			 * @param [in] Vector3 &vec	 : ��]�x�N�g��
			 * @param [in] Vector3 &up	 : "��"�̏�����x�N�g��
			 * @param [in] Vector3 &pos	 : �ʒu�x�N�g��(x,y)
			 * @return *this			 : ���������ꂽ�s��	 
			 **/
			
			Matrix4x4 &Initialize( Vector3 &rot_vec,Vector3 &up,Vector3& pos );

			/** �J�����s��
			 * @param [in] Vector3 &vec		 : Z�����x�N�g��
			 * @param [in] Vector3 &up		 : ������x�N�g��
			 * @param [in] Vector3 &pos		 : �t�ʒu�x�N�g��(x,y,z)
			 * @return *this				 : ���������ꂽ�s��
			 **/
			Matrix4x4 &View( Vector3 &zvec,Vector3 &up,Vector3& pos ) ;
			
			/**
			 * @param [in] Vector3 &pos		 : �J�����ꏊ���W
			 * @param [in] Vector3 &at		 : ���Ă��i�Ώہj
			 * @param [in] Vector3 &up		 : ���̏�����x�N�g��
			 * @return *this				 : �J�����s�񂪏��������ꂽ�s��				
			 **/
			Matrix4x4 LookAt( Vector3 &pos,Vector3 &at,Vector3 &up )
			{
				Vector3 v;
				v = at - pos;								//����Ώې� - �J�����̈ʒu
				v.Normalize();
				return View( v,up,pos );
			}

			/** �s��|�����킹(*) */
			//�s�񓯎m�̊|���Z�͓��e�I�ɂ͕ϊ��������鎖(�����Z)�ɂȂ�
			Matrix4x4 operator * ( const Matrix4x4 &m ) const;
			
			/** �s��|�����킹(*=) */
			Matrix4x4 &operator *= ( const Matrix4x4 &m ){ return *this=*this *m; }

			/** �e�����o�����Z */
			Matrix4x4 operator + ( const Matrix4x4 &m ) const;
			
			/** �e�����o�����Z */
			Matrix4x4 operator - ( const Matrix4x4 &m ) const;
			
			/** �����{ */
			Matrix4x4 operator * ( const float f ) const;

			/** �P�ʍs�� */
			Matrix4x4 &Identity() { return *this=unit_matrix; }

			/** Z����]�s��
			 * @param[in]	float r : ���W�A���l
			 * @note �x���w��� X/180 * PI
			 **/
			Matrix4x4 &RotateZ( float r );

			/** X����]�s��
			 * @param[in]	float r : ���W�A���l
			 * @note �x���w��� X/180 * PI
			 **/
			Matrix4x4 &RotateX( float r );

			/** Y����]�s��
			 * @param[in]	float r : ���W�A���l
			 * @note �x���w��� X/180 * PI
			 **/
			Matrix4x4 &RotateY( float r );
			
			/** �C��"�_"�ŉ�]
			 * @param [in]	&rot	:��]�s��
			 * @param [in]	&pos	:�C�Ӎ��W
			 **/
			Matrix4x4 &RotatePos( Matrix4x4 &rot,Vector3 &pos );

			/** �C��"��"�ŉ�]
			 * @param [in] Vector &vz	:�C�ӎ� (0,1,0)�Ȃ�Y�� (1,0,0)�Ȃ�X��
			 * @param [in] float	r	:��]�x��
			 * vz��1�ȏ�Ȃ�Normalize()���邱�ƁI
			 **/
			Matrix4x4 &RotateAxis( Vector3 &vz, float r );
			
			/** �X�P�[�����O�s��
			 * @param [in] float sx : �X�N���[�����W(0�`1.0)
			 * @param [in] float sy : �X�N���[�����W(0�`1.0)
	   		 * @param [in] float sz : �X�N���[�����W(0�`1.0)
			 **/ 
			Matrix4x4 &Scale( float sx,float sy,float sz );
			
			/** �X�P�[�����O�x�N�g���s��
			 * @param [in] Vector3 &s		: �X�N���[���x�N�g�����W(0�`1.0)
			 * return Scale( s.x,s.y,s.z )	: �X�N���[�����W
			 **/
			Matrix4x4 &Scale( const Vector3 &s ){ return Scale( s.x,s.y,s.z ); }

			/** �X�N���[���ϊ��s��(���\�������ق�)
			 * @param [in]	int x	:�ʂ��ꏊ
			 * @param [in]	int y	:�ʂ��ꏊ
			 * @param [in]	int w	:��ʕ�
			 * @param [in]	int h	:��ʍ���
			 **/
			Matrix4x4 &Screen( int x,int y,int w,int h );
			
			/** �X�N���[���ϊ��s��( Rect�œn�� )
			 * @param [in] const Rect &r	:�͈͎w��
			 **/
			Matrix4x4 &Screen( const system::Rect &r  ){ return Screen(r.left,r.top,r.right,r.bottom); }
			
			/** �X�N���[���ϊ��s��
			 * @param [in] int w	:��ʕ�
			 * @param [in] int h	:��ʍ���
			 **/
			Matrix4x4 &Screen( int w,int h ) { return Screen( 0,0,w,h); }
			
			/** ���s�ړ�
			 * @param[in] float x : ���s�ړ����W�� x (0.0�`1.0)
			 * @param[in] float y : ���s�ړ����W�� y (0.0�`1.0)
			 * @param[in] float z : ���s�ړ����W�� z (0.0�`1.0)
			 **/
			Matrix4x4 &Translate( float x,float y,float z );
			
			/** ���s�ړ�
			 * @param[in] const Vector3 &v : �x�N�g�����W( x,y,z )
			 * @return ���s�ړ���̃x�N�g�����W( v.x,v.y,v.z )
			 **/
			Matrix4x4 &Translate( const Vector3 &v ){ return Translate( v.x,v.y,v.z ); }
			
			/** �]�u(�Ίp�ɓ���ւ�)�s��ɂ��� */
			Matrix4x4 &Transpose();
			
			/** �t�s��ɂ��� */
			static Matrix4x4 Inverse( Matrix4x4 &m );
			
			/** �ŗL�l�����߂� */
			float Determinant(){
				return(	mat[0][0] *Cofactor(0,0) + mat[0][1]*Cofactor(0,1)+
						mat[0][2] *Cofactor(0,2) + mat[0][3]*Cofactor(0,3));
			}

			/** �]���q�����߂�
			 * @attention �z��̎Q�ƂƓ����ɂȂ�悤��(y,x)�̏��Ԃɂ���
			 **/
			float Cofactor( int y,int x );
			
			/** �x�N�g��*�s��
			 * ���_(0,0)����̌��_�ύX
			 * @param [in] const Vector3 &v	: �x�N�g��(x,y,z)
			 * return �x�N�g���̃X�N���[���ϊ����W
			 **/
			inline Vector3 TransformVector( const Vector3 &v ) const
			{
				Vector3 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20 + m30;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21 + m31;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22 + m32;
	
				return v2;
			};
			
			/** 4�����x�N�g�������ʂƂ��ē���
			 *	@return	Vector4	:w���܂ރx�N�g��
			 **/
			inline Vector4 Transform( Vector3 &v ) const
			{
				Vector4 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20 + m30;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21 + m31;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22 + m32;
				v2.w = v.x*m03 + v.y*m13 + v.z*m23 + m33;

				return v2;
			};
			
			/** �x�N�g��*��]�s��( 2*2 )
			 * @param [in] const Vector3 &v
			 * return �x�N�g���̉�]�ϊ��X�N���[�����W
			 **/
			inline Vector3 TransformRotation( Vector3 &v ) const
			{
				Vector3 v2;
				v2.x = v.x*m00 + v.y*m10 + v.z*m20;
				v2.y = v.x*m01 + v.y*m11 + v.z*m21;
				v2.z = v.x*m02 + v.y*m12 + v.z*m22;

				return v2;
			};

			/** �v���W�F�N�V�����s��i�������͈͂����߂�)
			 * @param [in] float wid		:��������
			 * @param [in] float hei		:����������
			 * @param [in] float zn			:near_plane	��O
			 * @param [in] float zf			:far_plane	���s��
			 * 2:2:1:���̓p�[�X������ 2:2:2:�� �͕���		���͂����ł�����
			 **/
			Matrix4x4 &Perspective( float wid, float hei,float zn, float zf );
			
			/** �V���h�E�s��(�C�ӂ̍����ŉe�̑傫����ω�������
			 * @param [in] float xs		:������X����
			 * @param [in] float zs		:������Y����
			 * @param [in] float height	:�Ώۂ̍���(���[�J�����_�܂ł̍���)
			 * @return �e�̍s��
			 **/
			Matrix4x4 &Shadow( float xs,float zs,float height ){
				m00 = 1,		m01= 0 , m02 = 0			,m03 = 0;
				m10 = xs,		m11= 0 , m12 = zs			,m13 = 0;
				m20 = 0,		m21= 0 , m22 = 1			,m23 = 0;
				m30 = -height*xs,	m31=0  , m32 = -height*zs	,m33 = 1;

				return *this;
			};

			/** �V���h�E�s��
			 * @param [in] float		xs		:������X����
			 * @param [in] float		zs		:������Y����
			 * @param [in] float		height	:�Ώۂ̍���(���[�J�����_�܂ł̍���)
			 * @param [in] Matrix4x4	&rot	:��]�s��
			 * @return �e�̍s��
			 **/
			Matrix4x4 &Matrix4x4::Shadow( float xs,float zs, float height,Matrix4x4 &rot )
			{
				float xx = -height*xs;
				float yy = height;
				float zz = -height*zs;

				float t1,t2,t3,t4,t5,t6;

				t1 = rot.m00+rot.m10*xs;
				t2 = rot.m20+rot.m10*zs;
				t3 = rot.m01+rot.m11*xs;
				t4 = rot.m21+rot.m11*zs;
				t5 = rot.m02+rot.m12*xs;
				t6 = rot.m22+rot.m12*zs;

				float x = xx*rot.m00+yy*rot.m10+zz*rot.m20;
				float y = xx*rot.m01+yy*rot.m11+zz*rot.m21;
				float z = xx*rot.m02+yy*rot.m12+zz*rot.m22;

				m00=t1*rot.m00+t2*rot.m20; m01=t1*rot.m01+t2*rot.m21; m02=t1*rot.m02+t2*rot.m22; m03=0;
				m10=t3*rot.m00+t4*rot.m20; m11=t3*rot.m01+t4*rot.m21; m12=t3*rot.m02+t4*rot.m22; m13=0,
				m20=t5*rot.m00+t6*rot.m20; m21=t5*rot.m01+t6*rot.m21; m22=t5*rot.m02+t6*rot.m22; m23=0,
				m30=x;					   m31=y;					  m32=z;					 m33=1;

				return *this;
			}

			/** ���ˍs��
			 * @paran [in]	Vector3	&az			:Z��
			 * @paran [in]	Vector3	&up			:�������
			 * @paran [in]	Vector3	&s			:�X�P�[��
			 **/
			Matrix4x4 &Matrix4x4::Reflect( const Vector3 &az,const Vector3 &up,const Vector3 &s );
			
			/**  �P�ʍs�� */
			static const Matrix4x4 unit_matrix;
	};
}}