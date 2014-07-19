/** @file
 * 跳ね返り判定の実装
 * @date 2009/10/1
 * @author 杉野
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
	Vector dir	 = a.p - b.p;			//方向ベクトル
	Vector v;
	const Vector &bx = b.ax;			//各軸の長さ
	const Vector &by = b.ay;
	const Vector &bz = b.az;
	
	float x = Vector3::Dot( bx,dir );	//各軸の最近点を求める
	float y = Vector3::Dot( by,dir );
	float z = Vector3::Dot( bz,dir );
		
	if( x <= -b.s.x ) x = -b.s.x;		//軸の飽和処理
	if( x >= +b.s.x ) x = +b.s.x;
	if( y <= -b.s.y ) y = -b.s.y;
	if( y >= +b.s.y ) y = +b.s.y;
	if( z <= -b.s.z ) z = -b.s.z;
	if( z >= +b.s.z ) z = +b.s.z;

	v = (bx * x) + (by * y) + (bz * z) + b.p;	//各軸長さ × 最近点までの距離
	dir = a.p - v;						//方向ベクトル
	v = dir;							//とっておく
	float len = (a.s * a.s) - dir.Length2();
	if( len > 0 )
	{
		//めり込み量を求める
		*ret = Vector3::NormalVector( v ) * len;
		//内積を求めて反射する軸を決める
		float dx = Vector3::Dot( *ret,bx ); if( dx<0 ) dx = -dx;
		float dy = Vector3::Dot( *ret,by ); if( dy<0 ) dy = -dy; 
		float dz = Vector3::Dot( *ret,bz ); if( dz<0 ) dz = -dz;

		return true;
	}	
	return false;
}

bool Rebound::Refrect( BoundingSphere3D &a, BoundingLine3D &b, math::Vector3 *mov )
{
	Vector	cross;		//交点
	Vector3 dir;		//めり込んだ法線
	//retが方向ベクトル crossは交点
	if( Intersect3D::Intersect( a,b,&dir,&cross )!=true)return false;		//交差したか
	bool hit = false;
	{
		Vector3 cv=a.p - b.s;
		Vector3  v=b.e - b.s;
		float   len=v.Length();
		Vector3 nv=v/len;
		float	dt  = Vector3::Dot( cv,nv );
		if( dt>=0 && dt<=len ){	//バットの範囲内なら
			dir = Vector3( -nv.z,0,nv.x );	//９０度回転
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
		//Vector3 baxis = ( b.e-b.s );					//バットの軸
		//a.p += dir;

		//反射する軸を決める(Z軸)
		Matrix4x4 m;
		//dir.Normalize();								//Intersectで求まったdirを反射軸として使う
		m.Reflect( dir,Vector(0,1,0),Vector3(1,1,-1) );
		*mov = m.TransformRotation( *mov );
		
		return true;
	}
	return false;
}

bool Rebound::Refrect( BoundingSphere3D &a, BoundingOBox3D &b, math::Vector3 *mov )
{
	Vector	cross;		//交点
	Vector3 dir;		//めり込んだ法線
	//retが方向ベクトル crossは交点
	if( Intersect3D::Intersect( a,b,&dir ))		//交差した
	{
		//Vector3 baxis = ( b.e-b.s );					//バットの軸
		//a.p += dir;

		//反射する軸を決める(Z軸)
		Matrix4x4 m;
		dir.Normalize();								//Intersectで求まったdirを反射軸として使う
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