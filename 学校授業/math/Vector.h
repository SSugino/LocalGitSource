/** @file
 * �x�N�g���֐��N���X�̒�`
 * @date 2009/05/26
 * @author ����
 **/

#pragma once
#include "../system/Template.h"
#include "Float.h"
#include <math.h>

namespace sugino{
namespace math{
	class Vector3;
	typedef Vector3	Vector;	

	/** �Q�����x�N�g���N���X */
	class Vector2{
	public:
		float x;
		float y;
		Vector2(){}	//�f�t�H���g�R���X�g���N�^
		Vector2( float xx,float yy ) : x(xx),y(yy) {}
		explicit Vector2( float r ) : x( Math::cos(r) ),y( Math::sin(r) ) {}//�p�x�̃x�N�g��
		Vector2 &setVector( float _x,float _y ) { x=_x,y=_y; return *this; }
		
		//---- �l�����Z -----
		/** ���Z */
		inline Vector2 operator + ( const Vector2 &v ) const{ return Vector2( x+v.x,y+v.y ); }	
		/** ���Z */
		inline Vector2 operator - ( const Vector2 &v ) const{ return Vector2( x-v.x,y-v.y ); }
		/** ��Z */
		inline Vector2 operator * ( const float s ) const{ return Vector2( x*s,y*s ); }		
		/** ���Z */
		inline Vector2 operator / ( const float s ) const{ return *this * Float::inv(s); }

		//---- ������Z -----
		inline Vector2 &operator += ( const Vector2 &v ){ x+=v.x;y+=v.y; return *this; }
		inline Vector2 &operator -= ( const Vector2 &v ) { x-=v.x;y-=v.y;return *this; }
		inline Vector2 &operator *= ( const float s ) { x*=s; y*=s; return *this;}
		inline Vector2 &operator /= ( const float s ) { return *this *=Float::inv(s); }	//�t��������
		//����
		Vector2 operator + () const { return *this; }
		/** �������] */
		Vector2 operator - () const { return Vector2( -x,-y ) ; }				
		/** �����̂Q�� */
		float Length2() const { return x*x + y*y; }		
		/** �O�����̒藝�Œ��������߂�(����) */
		float Length()  const { return Math::sqrt( Length2() ); }	// x*x + y*y
		/** �������g�̐��K��(�P�ʃx�N�g���q����"1"�r�ɂ���) */
		Vector2 &Normalize(){ return *this /= Length(); }							
		
		/** �����Ŏ󂯎�����x�N�g���̐��K��(�P�ʃx�N�g��)����"1"�� */
		static Vector2 NormalVector( const Vector2 &v ){ return v / v.Length(); }

		/** ���όv�Z
		 * @return float cos����
		 **/
		static float Dot( const Vector2 &a,const Vector2 &b ){ return a.x*b.x + a.y*b.y; }
			
		/** �O�όv�Z
		 * @return float sin����
		 **/
		static float Cross( const Vector2 &a,const Vector2 &b ){ return a.x*b.y - a.y*b.x; }
		
		/** ��̃x�N�g���̂Ȃ��p�x�̎擾(A��B)
		 * A "����" B�Ɍ������Ẳ�]
		 * return ���W�A���l�p�x
		 **/
		static float Angle( const Vector2 &a,const Vector2 &b ){ return Math::atan2( Cross(a,b),Dot(a,b) ); }

		/** 90�x��]�����x�N�g������� */
		inline Vector2 Rotate90() const { return Vector2( -y,x ); }
										  
		/** -90�x��]�����x�N�g������� */
		inline Vector2 RotateInvert90() const {	return Vector2( y,-x ); }
		
		/** �p�x�w���]
		 * @param [in] float r:�p�x
		 **/
		inline Vector2 Rotate( float r ) const 
		{ 
			Vector2 rot(r);
			return Vector2( x*rot.x - y*rot.y ,
							x*rot.y + y*rot.x );
		}

		/** �t��]�̃x�N�g�� */
		inline Vector2 Invert() const {	return Vector2( x,-y ); }
		
		/** zero�x�N�g�� */
		static const Vector2 zero;						
	};
	/** �R�����x�N�g�� */
	class Vector3
	{
	public:	
		float x;	//��
		float y;	//����
		float z;	//����

		Vector3(){}	//�f�t�H���g�R���X�g���N�^
		Vector3( float xx,float yy,float zz ) : x(xx),y(yy),z(zz) {}
		Vector3 &setVector( float _x,float _y,float _z ) { x=_x,y=_y,z=_z; return *this; }
		Vector3 &setVector( const Vector3 &v ) { x=v.x,y=v.y,z=v.z; return *this; }
		//---- �l�����Z -----
		/** ���Z */
		inline Vector3 operator + ( const Vector3 &v ) const{ return Vector3( x+v.x,y+v.y,z+v.z ); }	
		/** ���Z */
		inline Vector3 operator - ( const Vector3 &v ) const{ return Vector3( x-v.x,y-v.y,z-v.z ); }
		/** ��Z */
		inline Vector3 operator * ( const float s ) const{ return Vector3( x*s,y*s,z*s ); }		
		inline Vector3 operator * ( const Vector3 &v ) const{ return Vector3( x*v.x,y*v.y,z*v.z ); }		
		/** ���Z */
		inline Vector3 operator / ( const float s ) const{ return *this * Float::inv(s); }
		inline Vector3 operator / ( const Vector3 &v ) const{ return Vector3( x*Float::inv(v.x),y*Float::inv(v.y),z*Float::inv(v.z) ); }
		//---- ������Z -----
		inline Vector3 &operator += ( const Vector3 &v ){ x+=v.x;y+=v.y;z+=v.z; return *this; }
		inline Vector3 &operator -= ( const Vector3 &v ) { x-=v.x;y-=v.y;z-=v.z; return *this; }
		inline Vector3 &operator *= ( const float s ) { x*=s; y*=s;z*=s; return *this;}
		inline Vector3 &operator *= ( const Vector3 &v ) { x*=v.x; y*=v.y;z*=v.z; return *this;}
		inline Vector3 &operator /= ( const float s ) { return *this *=Float::inv(s); }	//�t��������

		//������
		inline bool operator == ( const Vector3 &v ){
			float xl,yl,zl;
			int hit=0;
			const float margin = 0.015f;					//Vector�l��float�̌덷��
			xl = x - v.x;
			if( xl<0 ) xl = -xl;							//�}�C�i�X�Ȃ畄�����]
			if( xl < margin )hit++;							//���������߁A1�����̂��Ȃ�Ⴂ�l�Ȃ�C�R�[���֌W��
			yl = y - v.y;
			if( yl<0 ) yl = -yl;							
			if( yl < margin )hit++;		
			zl = z - v.z;
			if( zl<0 ) zl = -zl;
			if( zl < margin )hit++;
			
			if( hit==3 ) return true;						//x,y,z�Ƃ���1�����̃C�R�[���֌W��ԂȂ�
			return false;
		}
		
		inline bool operator == ( const float s ){
			float xl,yl,zl;
			int hit=0;
			const float margin = 0.015f;					//Vector�l��float�̌덷��
			xl = x - s;
			if( xl<0 ) xl = -xl;							//�}�C�i�X�Ȃ畄�����]
			if( xl < margin )hit++;
			yl = y - s;
			if( yl<0 ) yl = -yl;
			if( yl < margin )hit++;
			zl = z - s;
			if( zl<0 ) zl = -zl;
			if( zl < margin )hit++;
			
			if( hit==3 ) return true;
			return false;
		}

		//����
		/** + */
		Vector3 operator + () const { return *this; }
		/** - �������] */
		Vector3 operator - () const { return Vector3( -x,-y,-z ) ; }				
		/** �����̂Q�� */
		float Length2() const { return x*x + y*y + z*z; }		
		/** �O�����̒藝�Œ��������߂�(����) */
		float Length()  const { return Math::sqrt( Length2() ); }	// ��(x*x + y*y + z*z)
		/** �������g�̐��K��(�P�ʃx�N�g���q����"1"�r�ɂ���) */
		Vector3 &Normalize(){ return *this /= Length(); }							
		
		/** �����Ŏ󂯎�����x�N�g���̐��K��(�P�ʃx�N�g��)����"1"�� */
		static Vector3 NormalVector( const Vector3 &v ){ return v / v.Length(); }

		/** ���όv�Z
		 * @return float cos����
		 **/
		static float Dot( const Vector3 &a,const Vector3 &b ){ return a.x*b.x + a.y*b.y + a.z*b.z; }
		
		/**  �Q�̃x�N�g���̓���(�Q�l��)
		 * @param [in] Vector3	:���ς������x�N�g��
		 * @return	���ς����l
		 **/
		float dot3Vector( const Vector3 &v )const
		{
			return x * v.x + y * v.y + z * v.z;
		}
		/** �O�όv�Z
		 * @return Vector3	:�O�����x�N�g��
		 **/
		static Vector3 Cross( const Vector3 &a,const Vector3 &b )
		{
			Vector3 v;
			v.z = a.x*b.y - a.y*b.x;
			v.x = a.y*b.z - a.z*b.y;
			v.y = a.z*b.x - a.x*b.z;

			return v;
		}
		/** ��̃x�N�g���̊O��(�Q�l��)
		 * @param [in] Vector3 v	:�O�ς������x�N�g��
		 * @return �V�����x�N�g��
		 **/
		Vector3 cross3Vector( const Vector3 &v ) const
		{
			return Vector3( y * v.z - z * v.y,
							z * v.x - x * v.z,
							x * v.y - y * v.x );
		}
			
		/** �x�N�g���̔C�ӎ���]
		 * �������g��axis�����rot����]������
		 * @param [in] Vector3	axis	:�C�ӂ̎�
		 * @param [in] float	rot		:��]�p�x(���W�A���l)
		 **/
		Vector3 &Rotate( Vector3 &axis , float rot )
		{
			float len = Dot( *this,axis );
			Vector3 v = axis * len;
			Vector3 vx = *this - v;
			Vector3 vy = Cross( axis,vx );	//��]���t�Ȃ̂�

			*this = v + vx*Math::cos(rot) + vy*Math::sin(rot);
			return *this;
		}
		/** zero�x�N�g�� */
		static const Vector3 zero;						
	};

	/** 4�����x�N�g�� */
	class Vector4
	{
	public:
		float	x,y,z,w;		//TransForm���Z���ʑ���p
		
		Vector4(){}
		Vector4( float xx,float yy,float zz,float ww ) : x(xx),y(yy),z(zz),w(ww) {}
		Vector4 &setVector( float _x,float _y,float _z,float _w ) { x=_x,y=_y,z=_z,w=_w; return *this; }

	};
	
	/** 4���� */
	class Quaternion
	{
	public:
		Vector3	v;
		float	w;				//��]��
		//�f�t�H���g�R���X�g���N�^
		Quaternion(){}
		/** �������R���X�g���N�^
         * @param [in] float	a	:���x�N�g��x�l
		 * @param [in] float	b	:���x�N�g��y�l
		 * @param [in] float	c	:���x�N�g��z�l
		 * @param [in] float	d	:��]��
		 **/
		Quaternion( float a,float b,float c,float d ) : v(a,b,c),w(d) {}
		
		/** �������R���X�g���N�^
		 * @param [in] Vector3	&axis	:��(�P�ʃx�N�g��)
		 * @param [in] float	r		:��]��
		 **/
		Quaternion( Vector3 &axis,float r ){ Rotation(axis,r); }
		
		/** ��]���w��
		 * @param [in] Vector3	&axis	:���i�P�ʃx�N�g��)
		 * @param [in] float	r		:��]��
		 * @return	��]��̎������g
		 **/
		inline Quaternion &Rotation( Vector3 &axis,float r ){
			r *= 0.5f;
			v = axis*Math::sin(r);
			w = Math::cos(r);
			return *this;
		}

		/** �t��] */
		inline Quaternion &Invert(){
			v.x=-v.x; v.y=-v.y; v.z=-v.z;
			return *this;
		}
		
		/** ���K�� */
		inline Quaternion &Normalize(){
			float len = 1/sqrtf( v.Length2() + w*w );
			v*=len; w*=len;
			return *this;
		}

		//---- �l�����Z -----
		/** ��Z�I�y���[�^
		 * @param [out]	Quaternion	&q	:�|�����킹��Quaternion�N���X
		 **/
		inline Quaternion operator * ( const Quaternion &q ) const{
			Quaternion t;
			t.v = v*q.w + q.v*w + math::Vector3::Cross( q.v,v );
			t.w = w*q.w - Vector3::Dot( q.v,v );
			return t;
		};
		
		/** ��Z�I�y���[�^
		 * @param [in]	float	f		:�|�����킹��X�J���[�l
		 **/
		inline Quaternion operator * ( float f ) const {
			return Quaternion( v.x*f,v.y*f,v.z*f,w*f );
		}
		
		/** ��Z����I�y���[�^
		 * @param [in]	Quaternion	&q	:�|�����킹��Quaternion�N���X
		 **/
		inline Quaternion &operator *= ( const Quaternion &q ){ return *this=*this * q;}

		/** ���Z����I�y���[�^
		 * @param [in]	Quaternion	&q	:�������킹��Quaternion�N���X
		 **/
		inline Quaternion operator + ( const Quaternion &q ) const {
			return Quaternion( q.v.x+v.x, q.v.y+v.y, q.v.z+v.z, q.w+w ); }

		/** ���Z����I�y���[�^
		 * @param [in]	Quaternion	&q	:�������킹��Quaternion�N���X
		 **/
		inline Quaternion operator - ( const Quaternion &q ) const {
			return Quaternion( q.v.x-v.x, q.v.y-v.y, q.v.z-v.z, q.w-w ); }
		
		/** ���ʏ�̐��`���
		 * @param [in] Quaternion	&q1		:4����1
		 * @param [in] Quaternion	&q2		:4����2
		 * @param [in] float		t		:����
		 * @return ���ʏ�̐��K�����ꂽ�l
		 **/
		static Quaternion Slerp( const Quaternion &q1,
								 const Quaternion &q2,
								 const float t );

	};

	/** �� */
	class Plane
	{
	public:
		Vector3		n;		//�@���x�N�g��
		float		d;		//�ʂ��猴�_�܂ł̋���

		Plane(){}
		
		/** �������R���X�g���N�^
		 * @param [in] Vector3	&norm	:�@���x�N�g��
		 * @param [in] float	dist	:�ʂ��猴�_�܂ł̋���
		 **/
		Plane( Vector3 &norm,float dist ) : n(norm),d(dist){};
		
		/** �������R���X�g���N�^
		 * @param [in] Vector3	&p1		:�|���S�����W
		 * @param [in] Vector3	&p2		:�|���S�����W
		 * @param [in] Vector3	&p3		:�|���S�����W
		 **/
		Plane( Vector3 &p1,Vector3 &p2,Vector3 &p3 );	//�ʃf�[�^�쐬����@���x�N�g���A����������
		
		/** �ʂɑ΂��Ă̓_�̈ʒu�𒲂ׂ�
		 * @param [in] Vector3 &p		:�_�̈ʒu
		 **/
		float Side( Vector3 &p ){
			float len = Vector3::Dot( this->n,p );
			return len+this->d;
		}

		/** �_�Ɩʂ̋����𒲂ׂ�
		 * @param [in] Vector3 &p		:�_�̈ʒu
		 **/
		float Distance( Vector3 &p ){ 
			float len = Side( p );
			if( len<0 ) len=-(len);			//�}�C�i�X�Ȃ畄�����]
			return len;
		}

		/** �ʂƐ��̌������擾
		 * @param [in] Vector3	&s		:���̊J�n���W
		 * @param [in] Vector3	&e		:���̏I�����W
		 * @param [in] Vector3	*ret	:��_���W
		 * @return *ret	 0:�����Ȃ�	1:��������	-1:�ʂɐ���
		 **/
		int Intersect( Vector3 &s,Vector3 &e,Vector3 *ret );

		/** �ʂƃ��C�i����)�̌�_���擾
		 * @param [in] Vector3	&pos	:�n�_
		 * @param [in] Vector3	&ray	:�����̕����i�P�ʃx�N�g��)
		 * @param [in] Vector3	*ret	:��_���W
		 * @return *ret	 0:�����Ȃ�	1:��������	2:���Α��ɐ���	-1:�ʂɐ���
		 **/
		int IntersectRay( Vector3 &pos,Vector3 &ray,Vector3 *ret );

		/** �|���S���̓��O����
		 * @param [in]	Vector3	&a		:���ʏ�̓_1
		 * @param [in]	Vector3	&b		:���ʏ�̓_2
		 * @param [in]	Vector3	&c		:���ʏ�̓_3
		 * @param [in]	Vector3	&pos	:���ʏ�̌�_
		 * @return	true:�����Ă���		false:�����ĂȂ�
		 **/
		bool Inside( Vector3 &a,Vector3 &b,Vector3 &c,Vector3 &pos );
	};
}}