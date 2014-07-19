/** @file
 * 関数クラスの実装
 * @date 2009/05/26
 * @author 杉野
 **/

#include "Vector.h"
using namespace sugino;
using namespace math;
/** zero定義 */
const Vector2 Vector2::zero( 0,0 );

const Vector3 Vector3::zero( 0,0,0 );

/** 球面上の線形補間
 * @param [in] Quaternion	&q1		:4元数1
 * @param [in] Quaternion	&q2		:4元数2
 * @param [in] float		t		:時間
 * @return 球面上の正規化された値
 **/
Quaternion Quaternion::Slerp( const Quaternion &q1, const Quaternion &q2, const float t)
{
	Quaternion q;
	float c,tt;
	c = ( Vector3::Dot( q1.v,q2.v ) + q1.w*q2.w ); if(c<0) c=-c;

	if( c<1.0f ){
		float theta = Math::acos( c ),s = Math::sin( theta );
		tt = sinf( t*theta )/s;
		q = (q2-q1)*tt+q1;
	}else{
		q = (q2-q1)*t+q1;
	}
	return q.Normalize();
}

/** 引数つきコンストラクタ
 * @param [in] Vector3	&p1		:ポリゴン座標
 * @param [in] Vector3	&p2		:ポリゴン座標
 * @param [in] Vector3	&p3		:ポリゴン座標
 **/
Plane::Plane( Vector3 &p1, Vector3 &p2,Vector3 &p3 )
{	
	//法線初期化
	Vector v1,v2;
	v1 = p2 - p1;
	v2 = p3 - p1;

	n = Vector::Cross( v1,v2 ).Normalize(); //正規化した法線ベクトルもとめる

	//距離初期化 反対向きなのでマイナスを付ける
	d = -Vector3::Dot( p1,n );
}	

/** 面と線の交差を取得
 * @param [in] Vector3	&s		:線の開始座標
 * @param [in] Vector3	&e		:線の終了座標
 * @param [in] Vector3	*ret	:交点座標
 * @return *ret	 0:交差なし	1:交差あり	-1:面に水平
 **/
int Plane::Intersect( Vector3 &s, Vector3 &e, Vector3 *ret )
{
	//交差判定
	float s1= Plane::Side( s );
	float s2= Plane::Side( e );					//点の位置を調べる

	if( s1*s2 >0 ) return 0;					//交差してない
	//距離
	float d1=s1;if( d1<0 )d1=-d1;					//交差までの"垂直"の距離 //関数でDistanceもSideも変わらないから
	float d2=s2;if( d2<0 )d2=-d2;
	float dist = d1+d2;						//全体の距離
	
	if( dist==0 ){ *ret = s;return -1;}				//水平
	//"交差"までの長さを求める
	float t = (dist - d2)/dist;					//全体からの割合を求める MAXを1と考えて･･･
	*ret = s+( e - s )*t;						//線形補間
	return 1;
}

/** 面とレイ（光線)の交点を取得
 * @param [in] Vector3	&pos	:始点
 * @param [in] Vector3	&ray	:光線の方向（単位ベクトル)
 * @param [in] Vector3	*ret	:交点座標
 * @return *ret	 0:交差なし	1:交差あり	2:反対側に水平	-1:面に水平
 **/
int Plane::IntersectRay( Vector3 &pos, Vector3 &ray, Vector3 *ret )
{
	float dt= -Vector3::Dot( n,ray );
	float s1= Side( pos );			//符号も長さもほしい
	
	if( dt==0 )
	{
		if( s1!=0 )return 0;		//水平でなおかつ交差なし
		else{
			*ret= pos;
			return -1;				//面上
		}
	}

	//"交差"までの長さを求める
	float t = (s1/dt);					
	*ret = pos+ ray*t;				//交点求める		
	if( t>0 ) return 1;				//交差している
	return 2;
	
}

/** ポリゴンの内外判定
 * @param [in]	Vector3	&a		:平面上の点1
 * @param [in]	Vector3	&b		:平面上の点2
 * @param [in]	Vector3	&c		:平面上の点3
 * @param [in]	Vector3	&pos	:平面上の交点
 * @return	true:入っている		false:入ってない
 **/
bool Plane::Inside( Vector3 &a,Vector3 &b, Vector3 &c, Vector3 &pos )
{
	Vector v1,v2,v3;		//各頂点へのベクトル
	Vector crs1,crs2,crs3;	//各法線ベクトル
	v1 = a - pos;			//頂点へのベクトル
	v2 = b - pos;
	v3 = c - pos;
	
	//法線の方向を出す
	crs1 = Vector::Cross( v1,v2 );
	crs2 = Vector::Cross( v2,v3 );
	crs3 = Vector::Cross( v3,v1 );
	//同じ向きか？
	float dot1,dot2,dot3;
	dot1 = Vector::Dot( crs1,n );	if( dot1<0 )return false;			//cos値が負　つまり反対方向
	dot2 = Vector::Dot( crs2,n );	if( dot2<0 )return false;
	dot3 = Vector::Dot( crs3,n );	if( dot3<0 )return false;
	if( (dot1*dot2*dot3)>=0 ) return true;		//cos値が全て正なら
	return  false;
}