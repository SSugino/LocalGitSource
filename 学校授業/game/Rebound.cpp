/** @file
 * ���˕Ԃ蔻��̎���
 * @date 2009/10/1
 * @author ����
 **/
#pragma once


#include "Rebound.h"
#include "Collision.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../baseball/Ball.h"

using namespace sugino;
using namespace game;
using namespace math;

bool Rebound::Bound( const BoundingSphere3D &a, const BoundingOBox3D &b, math::Vector3 *ret, math::Matrix4x4 *rif )
{
	Vector dir	 = a.p - b.p;			//�����x�N�g��
	Vector v;
	const Vector &bx = b.ax;			//�e���̒���
	const Vector &by = b.ay;
	const Vector &bz = b.az;
	
	float x = Vector3::Dot( bx,dir );	//�e���̍ŋߓ_�����߂�
	float y = Vector3::Dot( by,dir );
	float z = Vector3::Dot( bz,dir );
		
	if( x <= -b.s.x ) x = -b.s.x;		//���̖O�a����
	if( x >= +b.s.x ) x = +b.s.x;
	if( y <= -b.s.y ) y = -b.s.y;
	if( y >= +b.s.y ) y = +b.s.y;
	if( z <= -b.s.z ) z = -b.s.z;
	if( z >= +b.s.z ) z = +b.s.z;

	v = (bx * x) + (by * y) + (bz * z) + b.p;	//�e������ �~ �ŋߓ_�܂ł̋���
	dir = a.p - v;						//�����x�N�g��
	v = dir;							//�Ƃ��Ă���
	float len = (a.s * a.s) - dir.Length2();
	if( len > 0 )
	{
		//�߂荞�ݗʂ����߂�
		*ret = Vector3::NormalVector( v ) * len;
		//���ς����߂Ĕ��˂��鎲�����߂�
		float dx = Vector3::Dot( *ret,bx ); if( dx<0 ) dx = -dx;
		float dy = Vector3::Dot( *ret,by ); if( dy<0 ) dy = -dy; 
		float dz = Vector3::Dot( *ret,bz ); if( dz<0 ) dz = -dz;

		return true;
	}	
	return false;
}

bool Rebound::Refrect( BoundingSphere3D &a, BoundingLine3D &b, math::Vector3 *mov )
{
	Vector	cross;		//��_
	Vector3 dir;		//�߂荞�񂾖@��
	//ret�������x�N�g�� cross�͌�_
	if( Intersect3D::Intersect( a,b,&dir,&cross )!=true)return false;		//����������
	bool hit = false;
	{
		Vector3 cv=a.p - b.s;
		Vector3  v=b.e - b.s;
		float   len=v.Length();
		Vector3 nv=v/len;
		float	dt  = Vector3::Dot( cv,nv );
		if( dt>=0 && dt<=len ){	//�o�b�g�͈͓̔��Ȃ�
			dir = Vector3( -nv.z,0,nv.x );	//�X�O�x��]
			float	dist = Vector3::Dot( dir,b.s );
			float	side = Vector3::Dot( dir,a.p ) - dist;
			side -= (b.width+a.s);
			if( side<0 && side>=-(a.s+b.width) )
			{
				hit = true;
			}
			
		}
	}
	if( hit )
	{
		//Vector3 baxis = ( b.e-b.s );					//�o�b�g�̎�
		//a.p += dir;

		//���˂��鎲�����߂�(Z��)
		Matrix4x4 m;
		//dir.Normalize();								//Intersect�ŋ��܂���dir�𔽎ˎ��Ƃ��Ďg��
		m.Reflect( dir,Vector(0,1,0),Vector3(1,1,-1) );
		*mov = m.TransformRotation( *mov );
		
		return true;
	}
	return false;
}

bool Rebound::Refrect( BoundingSphere3D &a, BoundingOBox3D &b, math::Vector3 *mov )
{
	Vector	cross;		//��_
	Vector3 dir;		//�߂荞�񂾖@��
	//ret�������x�N�g�� cross�͌�_
	if( Intersect3D::Intersect( a,b,&dir ))		//��������
	{
		//Vector3 baxis = ( b.e-b.s );					//�o�b�g�̎�
		//a.p += dir;

		//���˂��鎲�����߂�(Z��)
		Matrix4x4 m;
		dir.Normalize();								//Intersect�ŋ��܂���dir�𔽎ˎ��Ƃ��Ďg��
		m.Reflect( dir,Vector(0,1,0),Vector3(1,1,-1) );
		*mov = m.TransformRotation( *mov );
		return true;
	}
	return false;
}


Vector3 Rebound::getRightAxis( const Vector3 &dir, const Vector3 &up )
{
	Vector v90;
	v90 = Vector3::Cross( up,dir ).Normalize();
	return v90;
}