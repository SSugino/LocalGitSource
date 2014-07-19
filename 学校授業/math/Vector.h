/** @file
 * ベクトル関数クラスの定義
 * @date 2009/05/26
 * @author 杉野
 **/

#pragma once
#include "../system/Template.h"
#include "Float.h"
#include <math.h>

namespace sugino{
namespace math{
	class Vector3;
	typedef Vector3	Vector;	

	/** ２次元ベクトルクラス */
	class Vector2{
	public:
		float x;
		float y;
		Vector2(){}	//デフォルトコンストラクタ
		Vector2( float xx,float yy ) : x(xx),y(yy) {}
		explicit Vector2( float r ) : x( Math::cos(r) ),y( Math::sin(r) ) {}//角度のベクトル
		Vector2 &setVector( float _x,float _y ) { x=_x,y=_y; return *this; }
		
		//---- 四則演算 -----
		/** 加算 */
		inline Vector2 operator + ( const Vector2 &v ) const{ return Vector2( x+v.x,y+v.y ); }	
		/** 減算 */
		inline Vector2 operator - ( const Vector2 &v ) const{ return Vector2( x-v.x,y-v.y ); }
		/** 乗算 */
		inline Vector2 operator * ( const float s ) const{ return Vector2( x*s,y*s ); }		
		/** 除算 */
		inline Vector2 operator / ( const float s ) const{ return *this * Float::inv(s); }

		//---- 代入演算 -----
		inline Vector2 &operator += ( const Vector2 &v ){ x+=v.x;y+=v.y; return *this; }
		inline Vector2 &operator -= ( const Vector2 &v ) { x-=v.x;y-=v.y;return *this; }
		inline Vector2 &operator *= ( const float s ) { x*=s; y*=s; return *this;}
		inline Vector2 &operator /= ( const float s ) { return *this *=Float::inv(s); }	//逆数かける
		//符号
		Vector2 operator + () const { return *this; }
		/** 符号反転 */
		Vector2 operator - () const { return Vector2( -x,-y ) ; }				
		/** 長さの２乗 */
		float Length2() const { return x*x + y*y; }		
		/** 三平方の定理で長さを求める(内積) */
		float Length()  const { return Math::sqrt( Length2() ); }	// x*x + y*y
		/** 自分自身の正規化(単位ベクトル〈長さ"1"〉にする) */
		Vector2 &Normalize(){ return *this /= Length(); }							
		
		/** 引数で受け取ったベクトルの正規化(単位ベクトル)長さ"1"に */
		static Vector2 NormalVector( const Vector2 &v ){ return v / v.Length(); }

		/** 内積計算
		 * @return float cos長さ
		 **/
		static float Dot( const Vector2 &a,const Vector2 &b ){ return a.x*b.x + a.y*b.y; }
			
		/** 外積計算
		 * @return float sin長さ
		 **/
		static float Cross( const Vector2 &a,const Vector2 &b ){ return a.x*b.y - a.y*b.x; }
		
		/** 二つのベクトルのなす角度の取得(A→B)
		 * A "から" Bに向かっての回転
		 * return ラジアン値角度
		 **/
		static float Angle( const Vector2 &a,const Vector2 &b ){ return Math::atan2( Cross(a,b),Dot(a,b) ); }

		/** 90度回転したベクトルを作る */
		inline Vector2 Rotate90() const { return Vector2( -y,x ); }
										  
		/** -90度回転したベクトルを作る */
		inline Vector2 RotateInvert90() const {	return Vector2( y,-x ); }
		
		/** 角度指定回転
		 * @param [in] float r:角度
		 **/
		inline Vector2 Rotate( float r ) const 
		{ 
			Vector2 rot(r);
			return Vector2( x*rot.x - y*rot.y ,
							x*rot.y + y*rot.x );
		}

		/** 逆回転のベクトル */
		inline Vector2 Invert() const {	return Vector2( x,-y ); }
		
		/** zeroベクトル */
		static const Vector2 zero;						
	};
	/** ３次元ベクトル */
	class Vector3
	{
	public:	
		float x;	//横
		float y;	//高さ
		float z;	//奥域

		Vector3(){}	//デフォルトコンストラクタ
		Vector3( float xx,float yy,float zz ) : x(xx),y(yy),z(zz) {}
		Vector3 &setVector( float _x,float _y,float _z ) { x=_x,y=_y,z=_z; return *this; }
		Vector3 &setVector( const Vector3 &v ) { x=v.x,y=v.y,z=v.z; return *this; }
		//---- 四則演算 -----
		/** 加算 */
		inline Vector3 operator + ( const Vector3 &v ) const{ return Vector3( x+v.x,y+v.y,z+v.z ); }	
		/** 減算 */
		inline Vector3 operator - ( const Vector3 &v ) const{ return Vector3( x-v.x,y-v.y,z-v.z ); }
		/** 乗算 */
		inline Vector3 operator * ( const float s ) const{ return Vector3( x*s,y*s,z*s ); }		
		inline Vector3 operator * ( const Vector3 &v ) const{ return Vector3( x*v.x,y*v.y,z*v.z ); }		
		/** 除算 */
		inline Vector3 operator / ( const float s ) const{ return *this * Float::inv(s); }
		inline Vector3 operator / ( const Vector3 &v ) const{ return Vector3( x*Float::inv(v.x),y*Float::inv(v.y),z*Float::inv(v.z) ); }
		//---- 代入演算 -----
		inline Vector3 &operator += ( const Vector3 &v ){ x+=v.x;y+=v.y;z+=v.z; return *this; }
		inline Vector3 &operator -= ( const Vector3 &v ) { x-=v.x;y-=v.y;z-=v.z; return *this; }
		inline Vector3 &operator *= ( const float s ) { x*=s; y*=s;z*=s; return *this;}
		inline Vector3 &operator *= ( const Vector3 &v ) { x*=v.x; y*=v.y;z*=v.z; return *this;}
		inline Vector3 &operator /= ( const float s ) { return *this *=Float::inv(s); }	//逆数かける

		//条件式
		inline bool operator == ( const Vector3 &v ){
			float xl,yl,zl;
			int hit=0;
			const float margin = 0.015f;					//Vector値とfloatの誤差分
			xl = x - v.x;
			if( xl<0 ) xl = -xl;							//マイナスなら符号反転
			if( xl < margin )hit++;							//距離を求め、1未満のかなり低い値ならイコール関係に
			yl = y - v.y;
			if( yl<0 ) yl = -yl;							
			if( yl < margin )hit++;		
			zl = z - v.z;
			if( zl<0 ) zl = -zl;
			if( zl < margin )hit++;
			
			if( hit==3 ) return true;						//x,y,zともに1未満のイコール関係状態なら
			return false;
		}
		
		inline bool operator == ( const float s ){
			float xl,yl,zl;
			int hit=0;
			const float margin = 0.015f;					//Vector値とfloatの誤差分
			xl = x - s;
			if( xl<0 ) xl = -xl;							//マイナスなら符号反転
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

		//符号
		/** + */
		Vector3 operator + () const { return *this; }
		/** - 符号反転 */
		Vector3 operator - () const { return Vector3( -x,-y,-z ) ; }				
		/** 長さの２乗 */
		float Length2() const { return x*x + y*y + z*z; }		
		/** 三平方の定理で長さを求める(内積) */
		float Length()  const { return Math::sqrt( Length2() ); }	// √(x*x + y*y + z*z)
		/** 自分自身の正規化(単位ベクトル〈長さ"1"〉にする) */
		Vector3 &Normalize(){ return *this /= Length(); }							
		
		/** 引数で受け取ったベクトルの正規化(単位ベクトル)長さ"1"に */
		static Vector3 NormalVector( const Vector3 &v ){ return v / v.Length(); }

		/** 内積計算
		 * @return float cos長さ
		 **/
		static float Dot( const Vector3 &a,const Vector3 &b ){ return a.x*b.x + a.y*b.y + a.z*b.z; }
		
		/**  ２つのベクトルの内積(参考書)
		 * @param [in] Vector3	:内積したいベクトル
		 * @return	内積した値
		 **/
		float dot3Vector( const Vector3 &v )const
		{
			return x * v.x + y * v.y + z * v.z;
		}
		/** 外積計算
		 * @return Vector3	:三次元ベクトル
		 **/
		static Vector3 Cross( const Vector3 &a,const Vector3 &b )
		{
			Vector3 v;
			v.z = a.x*b.y - a.y*b.x;
			v.x = a.y*b.z - a.z*b.y;
			v.y = a.z*b.x - a.x*b.z;

			return v;
		}
		/** 二つのベクトルの外積(参考書)
		 * @param [in] Vector3 v	:外積したいベクトル
		 * @return 新しいベクトル
		 **/
		Vector3 cross3Vector( const Vector3 &v ) const
		{
			return Vector3( y * v.z - z * v.y,
							z * v.x - x * v.z,
							x * v.y - y * v.x );
		}
			
		/** ベクトルの任意軸回転
		 * 自分自身をaxis軸上でrot分回転させる
		 * @param [in] Vector3	axis	:任意の軸
		 * @param [in] float	rot		:回転角度(ラジアン値)
		 **/
		Vector3 &Rotate( Vector3 &axis , float rot )
		{
			float len = Dot( *this,axis );
			Vector3 v = axis * len;
			Vector3 vx = *this - v;
			Vector3 vy = Cross( axis,vx );	//回転が逆なので

			*this = v + vx*Math::cos(rot) + vy*Math::sin(rot);
			return *this;
		}
		/** zeroベクトル */
		static const Vector3 zero;						
	};

	/** 4次元ベクトル */
	class Vector4
	{
	public:
		float	x,y,z,w;		//TransForm演算結果代入用
		
		Vector4(){}
		Vector4( float xx,float yy,float zz,float ww ) : x(xx),y(yy),z(zz),w(ww) {}
		Vector4 &setVector( float _x,float _y,float _z,float _w ) { x=_x,y=_y,z=_z,w=_w; return *this; }

	};
	
	/** 4元数 */
	class Quaternion
	{
	public:
		Vector3	v;
		float	w;				//回転量
		//デフォルトコンストラクタ
		Quaternion(){}
		/** 引数つきコンストラクタ
         * @param [in] float	a	:軸ベクトルx値
		 * @param [in] float	b	:軸ベクトルy値
		 * @param [in] float	c	:軸ベクトルz値
		 * @param [in] float	d	:回転量
		 **/
		Quaternion( float a,float b,float c,float d ) : v(a,b,c),w(d) {}
		
		/** 引数つきコンストラクタ
		 * @param [in] Vector3	&axis	:軸(単位ベクトル)
		 * @param [in] float	r		:回転量
		 **/
		Quaternion( Vector3 &axis,float r ){ Rotation(axis,r); }
		
		/** 回転を指定
		 * @param [in] Vector3	&axis	:軸（単位ベクトル)
		 * @param [in] float	r		:回転量
		 * @return	回転後の自分自身
		 **/
		inline Quaternion &Rotation( Vector3 &axis,float r ){
			r *= 0.5f;
			v = axis*Math::sin(r);
			w = Math::cos(r);
			return *this;
		}

		/** 逆回転 */
		inline Quaternion &Invert(){
			v.x=-v.x; v.y=-v.y; v.z=-v.z;
			return *this;
		}
		
		/** 正規化 */
		inline Quaternion &Normalize(){
			float len = 1/sqrtf( v.Length2() + w*w );
			v*=len; w*=len;
			return *this;
		}

		//---- 四則演算 -----
		/** 乗算オペレータ
		 * @param [out]	Quaternion	&q	:掛け合わせるQuaternionクラス
		 **/
		inline Quaternion operator * ( const Quaternion &q ) const{
			Quaternion t;
			t.v = v*q.w + q.v*w + math::Vector3::Cross( q.v,v );
			t.w = w*q.w - Vector3::Dot( q.v,v );
			return t;
		};
		
		/** 乗算オペレータ
		 * @param [in]	float	f		:掛け合わせるスカラー値
		 **/
		inline Quaternion operator * ( float f ) const {
			return Quaternion( v.x*f,v.y*f,v.z*f,w*f );
		}
		
		/** 乗算代入オペレータ
		 * @param [in]	Quaternion	&q	:掛け合わせるQuaternionクラス
		 **/
		inline Quaternion &operator *= ( const Quaternion &q ){ return *this=*this * q;}

		/** 加算代入オペレータ
		 * @param [in]	Quaternion	&q	:足し合わせるQuaternionクラス
		 **/
		inline Quaternion operator + ( const Quaternion &q ) const {
			return Quaternion( q.v.x+v.x, q.v.y+v.y, q.v.z+v.z, q.w+w ); }

		/** 減算代入オペレータ
		 * @param [in]	Quaternion	&q	:引き合わせるQuaternionクラス
		 **/
		inline Quaternion operator - ( const Quaternion &q ) const {
			return Quaternion( q.v.x-v.x, q.v.y-v.y, q.v.z-v.z, q.w-w ); }
		
		/** 球面上の線形補間
		 * @param [in] Quaternion	&q1		:4元数1
		 * @param [in] Quaternion	&q2		:4元数2
		 * @param [in] float		t		:時間
		 * @return 球面上の正規化された値
		 **/
		static Quaternion Slerp( const Quaternion &q1,
								 const Quaternion &q2,
								 const float t );

	};

	/** 面 */
	class Plane
	{
	public:
		Vector3		n;		//法線ベクトル
		float		d;		//面から原点までの距離

		Plane(){}
		
		/** 引数つきコンストラクタ
		 * @param [in] Vector3	&norm	:法線ベクトル
		 * @param [in] float	dist	:面から原点までの距離
		 **/
		Plane( Vector3 &norm,float dist ) : n(norm),d(dist){};
		
		/** 引数つきコンストラクタ
		 * @param [in] Vector3	&p1		:ポリゴン座標
		 * @param [in] Vector3	&p2		:ポリゴン座標
		 * @param [in] Vector3	&p3		:ポリゴン座標
		 **/
		Plane( Vector3 &p1,Vector3 &p2,Vector3 &p3 );	//面データ作成から法線ベクトル、長さ初期化
		
		/** 面に対しての点の位置を調べる
		 * @param [in] Vector3 &p		:点の位置
		 **/
		float Side( Vector3 &p ){
			float len = Vector3::Dot( this->n,p );
			return len+this->d;
		}

		/** 点と面の距離を調べる
		 * @param [in] Vector3 &p		:点の位置
		 **/
		float Distance( Vector3 &p ){ 
			float len = Side( p );
			if( len<0 ) len=-(len);			//マイナスなら符号反転
			return len;
		}

		/** 面と線の交差を取得
		 * @param [in] Vector3	&s		:線の開始座標
		 * @param [in] Vector3	&e		:線の終了座標
		 * @param [in] Vector3	*ret	:交点座標
		 * @return *ret	 0:交差なし	1:交差あり	-1:面に水平
		 **/
		int Intersect( Vector3 &s,Vector3 &e,Vector3 *ret );

		/** 面とレイ（光線)の交点を取得
		 * @param [in] Vector3	&pos	:始点
		 * @param [in] Vector3	&ray	:光線の方向（単位ベクトル)
		 * @param [in] Vector3	*ret	:交点座標
		 * @return *ret	 0:交差なし	1:交差あり	2:反対側に水平	-1:面に水平
		 **/
		int IntersectRay( Vector3 &pos,Vector3 &ray,Vector3 *ret );

		/** ポリゴンの内外判定
		 * @param [in]	Vector3	&a		:平面上の点1
		 * @param [in]	Vector3	&b		:平面上の点2
		 * @param [in]	Vector3	&c		:平面上の点3
		 * @param [in]	Vector3	&pos	:平面上の交点
		 * @return	true:入っている		false:入ってない
		 **/
		bool Inside( Vector3 &a,Vector3 &b,Vector3 &c,Vector3 &pos );
	};
}}