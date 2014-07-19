/** @file
 * 各判定クラスの実装
 * @date 2009/06/05
 * @author 杉野
 **/

#include "Collision.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"



using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

/** 衝突判定 円と円 */ 
bool Collision2D::Collision( BoundingCircle2D &a,BoundingCircle2D &b )
{
	/*
	Vector2 pos = b.p-a.p;		// b円中心からa円中心の位置ベクトル
	float r = a.s + b.s;		// 合計半径
	float len = pos.Length2();	// b円中心からa円中心の長さ
	float l = len-r*r;			// 中心間の長さ - 合計半径 補正量
	if( r > len )				// 中心の距離が半径の合計よりも小さい
	{
		return true;
	}
	return false;
	*/
	Vector2 v=a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )return true;
	return false;

}

/** 交差判定 円と円 */
bool Intersect2D::Intersect( BoundingCircle2D &a,BoundingCircle2D &b,Vector2 *ret )
{	
	Vector2 v=a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )
	{
		float s = ( a.s+b.s)*( a.s+b.s)-v.Length2();
		v.Normalize();
		*ret = v*s;		
		return true;
	}
	return false;
}

/** 衝突判定 四角と四角 */
bool Collision2D::Collision( BoundingBox2D &a,BoundingBox2D &b )
{
	Vector2 v = a.p - b.p;			//方向ベクトル
	float dx = v.x;					//ベクトル値の保存
	float sizex = a.s.x + b.s.x;	//合計サイズ	

	if( dx < 0 ){ dx = -dx;	}		//負なら[反転]
	if( dx > sizex ){ return false;	}//中心の距離が半径の合計よりも小さい
	
	float dy = v.y;
	float sizey = a.s.y + b.s.y;	//半径Y

	if( dy < 0 ){ dy = -dy; }
	if( dy > sizey ) { return false; }

	return true;
}

/** 交差判定 四角と四角 */
bool Intersect2D::Intersect( BoundingBox2D &a,BoundingBox2D &b,Vector2 *ret )
{
	float ax = 1,ay = 1;			//とって置く

	float vx = a.p.x - b.p.x;	if( vx<0 ){ vx=-vx; ax = -1; }
	float sizex = b.s.x + a.s.x;	//半径
	float dx = sizex - vx;			//めり込み量
	if( dx < 0 )  return false;			//めり込んでないなら

	float vy = a.p.y - b.p.y;	if( vy<0 ){ vy=-vy; ay = -1; }
	float sizey = b.s.y + a.s.y;	//半径
	float dy = sizey - vy;			//めり込み量
	if( dy < 0 )  return false;			//めり込んでないなら

	if( dx < dy )					//めり込み量がｙ多い
	{
		*ret = Vector2(dx*ax,0);
	}else{
		*ret = Vector2(0,dy*ay);
	}
	return true;
}

/** 衝突判定 線と線 */
bool Collision2D::Collision( BoundingLine2D &a,BoundingLine2D &b )
{	
	Vector2 aa,bb;			//計算用
	Vector2 av1,av2,av3;	//方向ベクトル用
	Vector2 bv1,bv2,bv3;
	av1 = a.e-a.s;				
	av2 = b.e-a.s;			//斜めのベクトル
	av3 = b.s-a.s;			//		〃（ただし下

	float c1 = aa.Cross( av1,av2 );	//ラインＢの終点が
	float c2 = aa.Cross( av1,av3 );	//ラインＢの始点が
	
	if( c1 * c2 > 0 )return false;
	
	bv1 = b.e-b.s;
	bv2 = a.e-b.s;
	bv3 = a.s-b.s;

	float c3 = bb.Cross( bv1,bv2 );	//ラインＡの終点が
	float c4 = bb.Cross( bv1,bv3 );	//ラインＡの始点が

	if( c3 * c4 > 0 )return false;

	return true;
}

/** 衝突判定 円と線 */
bool Collision2D::Collision( BoundingCircle2D &a,BoundingLine2D &b )
{
	Vector2 p;					//交点用
	Vector2 lv;					//ライン
	Vector2 v1,n;				//単位ベクトル
	float dot;					//内積値用

	v1 = a.p - b.s;								//ライン始点から円の中心に向かう
	lv = b.e - b.s;								//ラインの始点から終点に向かうベクトル
	n  = Vector2::NormalVector(lv);				//単位ベクトル用にコピー（ライン始点終点）
	dot = Vector2::Dot( v1,n );					//内積求める	(cos値が求まる) X

	if( dot < 0 ){ p = b.s; }						//始点　内積が始点より外側
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//終点  外側
	else p=n*dot+b.s;								//交点

	return ((a.p-p).Length2()<a.s*a.s);				//縦軸長さ　> 半径二乗
}

/** 交差判定 円と線 */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingLine2D &b, Vector2 *ret)
{
	Vector2 p;					//交点用
	Vector2 lv;					//ライン
	Vector2 v1,n;				//単位ベクトル
	float dot;					//内積値用

	v1 = a.p - b.s;							//ライン始点に向かう
	lv = b.e - b.s;								//ラインの始点から終点に向かうベクトル
	n  = Vector2::NormalVector(lv);				//単位ベクトル用にコピー（ライン始点終点）
	dot = Vector2::Dot( v1,n );					//内積求める	(cos値が求まる) X

	if( dot < 0 ){ p = b.s; }						//始点　内積が始点より外側
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//終点  外側
	else p=n*dot+b.s;								//交点
	
	float len = (a.p-p).Length2();					//(円の中心 - 交点の長さ)
	Vector2 vy = a.p - p;							//交点からの方向ベクト
	float v = a.s*a.s - vy.Length2();				//補正量
	Vector2 pn = vy.Normalize();					//Y軸の単位ベクトル
	if ( len < a.s*a.s )							//(円の中心 - 交点の長さ)　< 半径二乗 (当たったのなら)  
	{
		*ret = pn * v;	
		return true;
	}
	return false;
}

/** 交差判定 円と四角 */
bool Collision2D::Collision( BoundingCircle2D &a, BoundingBox2D &b)
{			
	float px=a.p.x;
	if( px> (b.p.x+b.s.x) ) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x) ) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y) ) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y) ) py=(b.p.y-b.s.y);

	Vector2 p(px,py);
	Vector2 v = p-a.p;		//最近点から円へのベクトル	
	float len = v.Length2();
	float r = a.s*a.s;		//半径を二乗しておく
	
	if( len < r	)			//最近点から円までの長さが半径より小さい
	{
		return true;
	}
	return false;
}

/** 接触判定 円と四角 */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingBox2D &b, Vector2 *ret )
{
	//最近点
	float px=a.p.x;
	if( px> (b.p.x+b.s.x) ) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x) ) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y) ) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y) ) py=(b.p.y-b.s.y);

	Vector2 p(px,py);
	Vector2 v = p-a.p;		//最近点から円へのベクトル	
	float len = v.Length2();
	float r = a.s*a.s;		//半径を二乗しておく

	if( len < r	)			//最近点から円までの長さが半径より小さい
	{
		float cr = len-r;				//補正量
		*ret = v.Normalize() * cr;//正規化*長さ
		return true;
	}
	return false;
}


/** 衝突判定 円と回転箱 */
bool Collision2D::Collision( BoundingCircle2D &a, BoundingOBox2D &b)
{
	Vector2 cv = a.p - b.p;						//円へのベクトル
	const Vector2 &vy = b.r;							//r(Y軸回転度)を
	const Vector2 &vx = b.r.RotateInvert90();			//X軸が無いのでrから-90度したものをX軸とする
	
	float dx = Vector2::Dot( cv,vx );			//内積求める	(cos値が求まる) Xの"長さ"が求まる
	if( dx >  +b.s.x )  dx= +b.s.x;
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector2::Dot( cv,vy );			//Y軸上の長さdyが求まる
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	Vector2 p=b.p + vx*dx + vy*dy;

	Vector2 v = p - a.p;						//最近点から円へのベクトル	
	float len = v.Length2();					//長さ二乗
	float r = a.s*a.s;							//半径も二乗しておく
	
	if( len < r	)								//最近点から円までの長さが半径より小さい 円と円の判定
	{
		return true;				
	}
	return false;
}

/** 交差判定 円と回転箱 */
bool Intersect2D::Intersect( BoundingCircle2D &a, BoundingOBox2D &b, Vector2 *ret)
{
	Vector2 cv = a.p - b.p;						//円へのベクトル
	const Vector2 &vy = b.r;							//r(Y軸回転度)を
	const Vector2 &vx = b.r.RotateInvert90();			//X軸が無いのでrから-90度したものをX軸とする
	
	float dx = Vector2::Dot( cv,vx );			//内積求める	(cos値が求まる) Xの"長さ"が求まる
	if( dx >  +b.s.x )  dx= +b.s.x;
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector2::Dot( cv,vy );			//Y軸上の長さdyが求まる
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	Vector2 p = b.p + vx*dx + vy*dy;			//最近点を求める 箱の中心から[X軸]上に"内積値" + [Y軸]上に"内積値"
	Vector2 v = p - a.p;						//最近点から円へのベクトル	
	float len = v.Length2();					//長さ二乗
	float r = a.s*a.s;							//半径も二乗しておく
	
	if( len < r	)								//最近点から円までの長さが半径より小さい 円と円の判定
	{
		float cr = len-r;						//補正量
		*ret = v.Normalize() * cr;				//正規化*長さ
		return true;				//HIT　！!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	}
	return false;
}

/** 接触判定 回転箱*回転箱(OBB*OBB) */
bool Collision2D::Collision( BoundingOBox2D &a, BoundingOBox2D &b )
{
	Vector2 abv = a.p - b.p;					//箱aから箱bのベクトル
	//軸
	const Vector2 &ay = a.r;					//箱AのY軸	(単位ベクトル)	
	const Vector2 &ax = a.r.RotateInvert90();	//箱AのX軸　(単位ベクトル)
	
	const Vector2 &by = b.r;					//箱BのY軸	(単位ベクトル)
	const Vector2 &bx = b.r.RotateInvert90();	//箱BのX軸　(単位ベクトル)

	float len;									//各軸上での距離
	
	float ax_bx;								//箱Aの[X]軸上での箱Bの[X]軸上の長さ
	float ax_by;								//箱Aの[X]軸上での箱Bの[Y]軸上の長さ

	float ay_bx;								//箱Aの[Y]軸上での箱Bの[X]軸上の長さ
	float ay_by;								//箱Aの[Y]軸上での箱Bの[Y]軸上の長さ

	//ax軸基準
	len = Vector2::Dot( ax,abv );if(len<0) len=-len;		//マイナスの値は正に（絶対値）
	ax_bx= Vector2::Dot( ax,bx );if(ax_bx<0) ax_bx=-ax_bx;
	ax_by= Vector2::Dot( ax,by );if(ax_by<0) ax_by=-ax_by;
	if( len > a.s.x + ax_bx*b.s.x+ax_by*b.s.y ) return false;

	//ay軸基準
	len = Vector2::Dot( ay,abv );if(len<0) len=-len;
	ay_bx= Vector2::Dot( ay,bx );if(ay_bx<0) ay_bx=-ay_bx;
	ay_by= Vector2::Dot( ay,by );if(ay_by<0) ay_by=-ay_by;
	if( len > a.s.y + ay_bx*b.s.x+ay_by*b.s.y ) return false;

	//bx軸基準
	//ax_bxの値などは上で取っているので書いてない
	len = Vector2::Dot( bx,abv );if(len<0) len=-len;		
	if( len > b.s.x + ax_bx*a.s.x+ay_bx*a.s.y ) return false;

	//by軸基準
	len = Vector2::Dot( by,abv );if(len<0) len=-len;
	if( len > b.s.y + ax_by*a.s.x+ay_by*a.s.y ) return false;

	return true;
}

/** 交差判定 回転箱*回転箱(OBB*OBB) */
bool Intersect2D::Intersect( BoundingOBox2D &a, BoundingOBox2D &b, Vector2 *ret)
{
	Vector2 abv = a.p - b.p;					//箱aから箱bのベクトル
	//軸
	const Vector2 &ay = a.r;					//箱AのY軸	(単位ベクトル)	
	const Vector2 &ax = a.r.RotateInvert90();	//箱AのX軸　(単位ベクトル)
	
	const Vector2 &by = b.r;					//箱BのY軸	(単位ベクトル)
	const Vector2 &bx = b.r.RotateInvert90();	//箱BのX軸　(単位ベクトル)
	
	float len;									//各軸上での距離  めり込み量
	
	float ax_bx;								//箱Aの[X]軸上での箱Bの[X]軸上の長さ
	float ax_by;								//箱Aの[X]軸上での箱Bの[Y]軸上の長さ

	float ay_bx;								//箱Aの[Y]軸上での箱Bの[X]軸上の長さ
	float ay_by;								//箱Aの[Y]軸上での箱Bの[Y]軸上の長さ

	Vector2 min_axis;							//最小軸
	float min;									//補正最小値
	float sgn;									//軸の符号反転
	//ax軸基準
	sgn=1;
	len  = Vector2::Dot( ax,abv );if(len<0) { len=-len;sgn=-1; }		//マイナスの値は正に（絶対値）軸方向を逆方向に
	ax_bx= Vector2::Dot( ax,bx );if(ax_bx<0) ax_bx=-ax_bx;				//軸の長さ	
	ax_by= Vector2::Dot( ax,by );if(ax_by<0) ax_by=-ax_by;
	//当たってんなら
	len = (a.s.x + ax_bx*b.s.x+ax_by*b.s.y) - len;						// 軸の長さ同士の合計 - 長さ = めり込み量										
	
	if( len < 0 ) return false;											//当たってない
	 min=len;															//minに短いほうのめり込み量保存 
	 min_axis = ax*sgn; 												//最小値の入ってる軸の方向を決める
	
	//ay軸基準
	sgn=1;
	len  = Vector2::Dot( ay,abv );if(len<0) { len=-len;sgn=-1; }
	ay_bx= Vector2::Dot( ay,bx );if(ay_bx<0) ay_bx=-ay_bx;
	ay_by= Vector2::Dot( ay,by );if(ay_by<0) ay_by=-ay_by;
	len = (a.s.y + ay_bx*b.s.x+ay_by*b.s.y) - len;
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = ay*sgn; }							//前回よりも短い			


	//bx軸基準
	//ax_bxの値などは上で取っているので書いてない
	sgn=1;
	len = Vector2::Dot( bx,abv );if(len<0) { len=-len;sgn=-1; }		
	len = (b.s.x + ax_bx*a.s.x+ay_bx*a.s.y) - len;						//めり込み量
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = bx*sgn; }							//前回よりも短いなら更新			

	//by軸基準
	sgn=1;
	len = Vector2::Dot( by,abv );if(len<0) { len=-len;sgn=-1; }
	len = (b.s.y + ax_by*a.s.x+ay_by*a.s.y) - len;						//めり込み
	if( len < 0 ) return false;
	if( min>len ){ min=len;min_axis = by*sgn; }							//前回よりも短いなら更新

	*ret = min_axis * min;												//最小軸で補正する
	return true;
}

/** 接触判定 回転箱と四角(OBB*AABB) */
bool Collision2D::Collision( BoundingOBox2D &a, BoundingBox2D &b)
{
	BoundingOBox2D b2( b.p,0,b.s );
	return Collision( a,b2 );
}

//------------------------------------------------------------------------------------------
/** 接触判定 球と球 */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingSphere3D &b )
{
	Vector3 v= a.p-b.p;
	if( v.Length2() < ( a.s+b.s)*( a.s+b.s) )return true;
	return false;
}

/** 交差判定 球と球 */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingSphere3D &b,Vector3 *ret )
{
	Vector3 v = a.p-b.p;
	float s  = a.s+b.s;//半径の合計
	float len = v.Length2();//二乗
	
	if( s*s > len ){
		len = Math::sqrt( len );
		*ret = v*( (s-len) /len );	//割合を求める

		return true;
	}
	return false;
}

/** 接触判定 AABBとAABB*/
bool Collision3D::Collision( BoundingBox3D &a, BoundingBox3D &b )
{
	Vector3 v = a.p - b.p;			//方向ベクトル
	float dx = v.x;					//ベクトル値の保存
	float sizex = a.s.x + b.s.x;	//合計サイズ	

	if( dx < 0 ){ dx = -dx;	}		//負なら[反転]
	if( dx > sizex ){ return false;	}//中心の距離が半径の合計よりも小さい
	
	float dy = v.y;
	float sizey = a.s.y + b.s.y;	//半径Y

	if( dy < 0 ){ dy = -dy; }
	if( dy > sizey ) { return false; }

	float dz = v.z;
	float sizez = a.s.z + b.s.z;	//半径Z
	if( dz < 0 ){ dz = -dz; }
	if( dz > sizez ) { return false; }

	return true;
}

/** 交差判定 AABBとAABB*/
bool Intersect3D::Intersect( BoundingBox3D &a,BoundingBox3D &b,Vector3 *ret )
{
	float ax=1,ay=1,az=1;										//とって置く

	float vx = a.p.x - b.p.x;	if( vx<0 ){ vx=-vx; ax = -1; }
	float sizex = b.s.x + a.s.x;								//半径
	float dx = sizex - vx;										//めり込み量
	if( dx < 0 )  return false;									//めり込んでないなら

	float vy = a.p.y - b.p.y;	if( vy<0 ){ vy=-vy; ay = -1; }
	float sizey = b.s.y + a.s.y;								//半径
	float dy = sizey - vy;										//めり込み量
	if( dy < 0 )  return false;									//めり込んでないなら

	float vz = a.p.z - b.p.z;	if( vz<0 ){ vz=-vz; az = -1; }
	float sizez = b.s.z + a.s.z;								//半径
	float dz = sizez - vz;										//めり込み量
	if( dz < 0 )  return false;									//めり込んでないなら

	if( dx<dy )
	{
		if( dx<dz )ret->setVector( dx*ax,0,0 );
		else	   ret->setVector( 0,0,dz*az );
	}else{
		if( dy<dz )ret->setVector( 0,dy*ay,0 );
		else	   ret->setVector( 0,0,dz*az );
	}
	return true;
}

/** 衝突判定 球と線 */
bool Collision3D::Collision( BoundingSphere3D &a,BoundingLine3D &b )
{
	Vector3 p;					//交点用
	Vector3 lv;					//ライン
	Vector3 v1,n;				//単位ベクトル
	float dot;					//内積値用

	v1 = a.p - b.s;								//ライン始点から球の中心に向かう
	lv = b.e - b.s;								//ラインの始点から終点に向かうベクトル
	n  = Vector3::NormalVector(lv);				//単位ベクトル用にコピー（ライン始点終点）
	dot = Vector3::Dot( v1,n );					//内積求める	(cos値が求まる) X

	if( dot < 0 ){ p = b.s; }						//始点　内積が始点より外側なら端を中心とする
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//終点  外側なら外側の端を中心とする
	else p=n*dot+b.s;								//交点
	
	BoundingSphere3D bs( p,b.width );				//交点を中心とした球

	if( Collision( a,bs ) )
	{
		return true;//( (a.p-p).Length2()<a.s*a.s );			//縦軸長さ　< 半径二乗
	}
	return false;
}

/** 交差判定 球と線 */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingLine3D &b, Vector3 *ret)
{
	Vector3 p;										//交点用
	Vector3 lv;										//ライン
	Vector3 v1,n;									//単位ベクトル
	float dot;										//内積値用

	v1 = a.p - b.s;									//ライン始点に向かう
	lv = b.e - b.s;									//ラインの始点から終点に向かうベクトル
	n  = Vector3::NormalVector(lv);					//単位ベクトル用にコピー（ライン始点終点）
	dot = Vector3::Dot( v1,n );						//内積求める	(cos値が求まる) X

	if( dot < 0 ){ p = b.s; }						//始点　内積が始点より外側
	else if( dot*dot > lv.Length2() ){ p = b.e;}	//終点  外側
	else p=n*dot+b.s;								//交点
	
	BoundingSphere3D bs( p,b.width );				//交点を中心とした球
	if( Intersect( a,bs,ret ))						//球同士の判定
	{
		return true;
	}
	return false;
}

//交点の求まる球と線の判定
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingLine3D &b, Vector3 *ret, Vector3 *cross_pos )
{
	Vector3 p;										//交点用
	Vector3 lv;										//ライン
	Vector3 v1,n;									//単位ベクトル
	float dot;										//内積値用

	v1 = a.p - b.s;									//ライン始点に向かう
	lv = b.e - b.s;									//ラインの始点から終点に向かうベクトル
	n  = Vector3::NormalVector(lv);					//単位ベクトル用にコピー（ライン始点終点）
	dot = Vector3::Dot( v1,n );						//内積求める	(cos値が求まる) X

	if( dot < 0 ) p = b.s;							//始点　内積が始点より外側
	else if( dot*dot > lv.Length2() ) p = b.e;		//終点  外側
	else p=n*dot+b.s;								//交点
	*cross_pos = p;									//交点を返す
	BoundingSphere3D bs( p,b.width*1.5f );			//交点を中心とした球 球の半径同士で判定するのでスイングが早くてもいいように半径サイズを少し大きくしている
	if( Intersect( a,bs,ret ))						//球同士の判定
	{
		return true;
	}
	return false;
}

/** 接触判定　球とレイ(光線） */
bool Collision3D::Collision( BoundingSphere3D &a,Vector3 &pos,Vector3 &ray )
{
	Vector p;					//交点用
	Vector lv;					//ライン
	Vector v1,n;				//単位ベクトル
	float dot;					//内積値用

	v1 = a.p - pos;									//ライン始点から球の中心に向かう
	dot = Vector3::Dot( v1,ray );					//内積求める	(cos値が求まる) X

	if( dot < 0 ){ p = pos; }						//始点　内積が始点より外側ならposを中心に
	else p= pos+ray*dot;								//交点
	
	if( (a.p-p).Length2()<a.s*a.s ) return true;
	return false;
}

/** 交差判定　球とレイ(光線） */
bool Intersect3D::Intersect( BoundingSphere3D &a,Vector3 &pos,Vector3 &ray, Vector3 *cross_pos ) 
{
	Vector v  = pos - a.p;
	float d   = Vector3::Dot( ray,v );			//方向を求める
	float len = v.Length2() - a.s*a.s;
	if( len > 0 && d >0 ) return false;		//遠いし反対方向なので
	float dr= d*d - len;
	if( dr < 0  ) return false;					//接触してない
	float t=(-d)-Math::sqrt(dr);
	if(t<0)t=0;
	*cross_pos = pos+ray*t;						//交点の位置を求める
	return true;
}


/** 接触判定　球と四角(AABB) */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingBox3D &b)
{
	float px = a.p.x;
	if( px> (b.p.x+b.s.x)) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x)) px=(b.p.x-b.s.x);

	float py=a.p.y;
	if( py> (b.p.y+b.s.y)) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y)) py=(b.p.y-b.s.y);

	
	float pz=a.p.z;
	if( pz> (b.p.z+b.s.z)) pz=(b.p.z+b.s.z);
	if( pz< (b.p.z-b.s.z)) pz=(b.p.z-b.s.z);

	Vector3 p(px,py,pz);
	Vector3 v = p - a.p;					//最近点から球へのベクトル	
	float len = v.Length2();
	float r = a.s*a.s;						//半径を二乗しておく
	
	if( len < r	) return true;				//最近点から円までの長さが半径より小さい
	return false;
}

/** 交差判定 球と四角（AABB) */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingBox3D &b, Vector3 *ret )
{
	//クリッピングし最近点を求める
	float px = a.p.x;
	if( px> (b.p.x+b.s.x)) px=(b.p.x+b.s.x);
	if( px< (b.p.x-b.s.x)) px=(b.p.x-b.s.x);
	float py=a.p.y;
	if( py> (b.p.y+b.s.y)) py=(b.p.y+b.s.y);
	if( py< (b.p.y-b.s.y)) py=(b.p.y-b.s.y);
	float pz=a.p.z;
	if( pz> (b.p.z+b.s.z)) pz=(b.p.z+b.s.z);
	if( pz< (b.p.z-b.s.z)) pz=(b.p.z-b.s.z);

	//最近点
	Vector3 p(px,py,pz);

	Vector3 v = p - a.p;					//最近点から球へのベクトル
	//Normalizeする前に0だったら
	if( v==0 ){ v.x=0.001f,v.y=0.001f,v.z=0.001f; }

	float len = v.Length2();
	float r = a.s*a.s;						//半径を二乗しておく
	
	if( len < r	){							//球と球の判定を同じ
		float cr = len-r;					//補正量
		*ret = v.Normalize() * cr;			//正規化*長さ
		return true;
	}
	return false;
}

/** 接触判定　球と回転箱(OBB) */
bool Collision3D::Collision( BoundingSphere3D &a, BoundingOBox3D &b )
{
	Vector3 cv = a.p - b.p;							//円へのベクトル
	const Vector3 &vy = b.ay;						//各軸を参照型で
	const Vector3 &vx = b.ax;						
	const Vector3 &vz = b.az;
	
	//各軸を内積求めめ長さを取得しクリッピング
	float dx = Vector3::Dot( cv,vx );				
	if( dx >  +b.s.x )  dx= +b.s.x;				
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector3::Dot( cv,vy );	
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	float dz = Vector3::Dot( cv,vz );
	if( dz >  +b.s.z ) dz= +b.s.z;
	if( dz <  -b.s.z ) dz= -b.s.z;
	
	Vector3 p= b.p + vx*dx + vy*dy + vz*dz;	//最近点を求める

	Vector3 v = p - a.p;						//最近点から円へのベクトル	
	float len = v.Length2();					//長さ二乗
	float r = a.s*a.s;							//半径も二乗しておく
	
	if( len < r	) return true;					//球と球の判定をする
	return false;
}

/** 交差判定 球と回転箱(OBB) */
bool Intersect3D::Intersect( BoundingSphere3D &a, BoundingOBox3D &b, Vector3 *ret)
{
	Vector3 cv = a.p - b.p;							//円へのベクトル
	const Vector3 &vy = b.ay;						//各軸を参照型で
	const Vector3 &vx = b.ax;						
	const Vector3 &vz = b.az;
	
	//各軸を内積求めめ長さを取得しクリッピング
	float dx = Vector3::Dot( cv,vx );				
	if( dx >  +b.s.x )  dx= +b.s.x;				
	if( dx <  -b.s.x )  dx= -b.s.x;

	float dy = Vector3::Dot( cv,vy );	
	if( dy >  +b.s.y ) dy= +b.s.y;
	if( dy <  -b.s.y ) dy= -b.s.y;
	
	float dz = Vector3::Dot( cv,vz );
	if( dz >  +b.s.z ) dz= +b.s.z;
	if( dz <  -b.s.z ) dz= -b.s.z;
	
	Vector3 p= b.p + vx*dx + vy*dy + vz*dz;	//最近点を求める

	Vector3 v = p - a.p;						//最近点から円へのベクトル	
	float len = v.Length2();					//長さ二乗
	float r = a.s*a.s;							//半径も二乗しておく
	
	if( len < r	)
	{
		if( v.Length() )						//0除算がでるか調べる
		{
			float cr = len-r;					//補正量
			*ret = v.Normalize() * cr;			//正規化*長さ
			return true;						//球と球の判定をする
		}
	}
	return false;
}
/** 軸から長さや補正量、補正向きを求める */
bool Collision3D::AxisLength( Vector3 &axis, Vector3 &dist,BoundingOBox3D &a, BoundingOBox3D &b, float *length,float *sign )
{
	//各軸上での長さ
	float asx = Vector3::Dot( axis,a.ax )*a.s.x;if( asx<0 )asx= -asx;	//負なら符号反転
	float asy = Vector3::Dot( axis,a.ay )*a.s.y;if( asy<0 )asy= -asy;
	float asz = Vector3::Dot( axis,a.az )*a.s.z;if( asz<0 )asz= -asz;
	
	float bsx = Vector3::Dot( axis,b.ax )*b.s.x;if( bsx<0 )bsx= -bsx;
	float bsy = Vector3::Dot( axis,b.ay )*b.s.y;if( bsy<0 )bsy= -bsy;
	float bsz = Vector3::Dot( axis,b.az )*b.s.z;if( bsz<0 )bsz= -bsz;

	float len = Vector3::Dot( axis,dist );								//箱間の距離
	*sign = +1;															//補正方向を正で初期化
	if( len<0 ) { len= -len;*sign=-1;}									//長さ負なら符号反転、補正方向も反転
	*length = (asx+asy+asz)+(bsx+bsy+bsz) -len;
	return *length > 0;													//長さがあるときのみ補正量を返す
}

/** 接触判定( OBB と OBB ) */
bool Collision3D::Collision( BoundingOBox3D &a, BoundingOBox3D &b )
{
	Vector3 v = a.p - b.p;					//箱aから箱bのベクトル
	Vector3 axis;
	float tlen,tsgn;							//引数用
	
	//箱aの各軸上判定
	if( !AxisLength( a.ax,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( a.ay,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( a.az,v, a,b, &tlen,&tsgn )) return false;
	//箱bの各軸上判定
	if( !AxisLength( b.ax,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( b.ay,v, a,b, &tlen,&tsgn )) return false;
	if( !AxisLength( b.az,v, a,b, &tlen,&tsgn )) return false;
	
	//箱同士の軸の外積で求め、長さで判定
	axis = Vector3::Cross( a.ax,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ax,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ax,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	axis = Vector3::Cross( a.ay,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ay,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.ay,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	axis = Vector3::Cross( a.az,b.ax );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.az,b.ay );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;
	axis = Vector3::Cross( a.az,b.az );
	if( axis.Length2() && !AxisLength( axis,v, a,b, &tlen,&tsgn )) return false;

	return true;
}

/** 交差判定 箱と箱*/
bool Intersect3D::Intersect( BoundingOBox3D &a, BoundingOBox3D &b, Vector3 *ret )
{
	Vector3 v = a.p - b.p;					//箱aから箱bのベクトル
	Vector3 axis;
	float tlen,tsgn,min,sgn;							//引数用
	
	//箱aの各軸上判定
	if( !Collision3D::AxisLength( a.ax,v, a,b, &tlen,&tsgn )) return false;
	min = tlen;	sgn = tsgn; *ret = a.ax;
	if( !Collision3D::AxisLength( a.ay,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=a.ay; }
	if( !Collision3D::AxisLength( a.az,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=a.az; }
	//箱bの各軸上判定
	if( !Collision3D::AxisLength( b.ax,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	if( !Collision3D::AxisLength( b.ay,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	if( !Collision3D::AxisLength( b.az,v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=b.ay; }
	//箱同士の軸の外積で求め、長さで判定
	axis = Vector3::Cross( a.ax,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }
	
	axis = Vector3::Cross( a.ax,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ax,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.ay,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.ax );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.ay );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	axis = Vector3::Cross( a.az,b.az );
	if( axis.Length2() && !Collision3D::AxisLength( axis.Normalize(),v, a,b, &tlen,&tsgn )) return false;
	if( min>tlen ){ min=tlen;sgn=tsgn; *ret=axis; }

	* ret *= min*sgn;
	return true;
}

bool Collision3D::Collision( BoundingBox3D &a, Vector3 &pos, Vector3 &ray )
{
	float min=0,max=FLT_MAX,tmp;		//tmpは入れ替え用
	//各軸で調べる
	float x1 = ( a.p.x-a.s.x ),x2 = ( a.p.x+a.s.x );
	if( ray.x ){						//傾きがある
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1～x2間の外側。交差しない
	}

	float y1 = ( a.p.y-a.s.y ),y2 = ( a.p.y+a.s.y );
	if( ray.y ){									//傾きがある
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{											//傾きなし
		if( pos.y <y1 || pos.y > y2 )return false;	//x1～x2間の外側。交差しない
	}

	float z1 = ( a.p.z-a.s.z ),z2 = ( a.p.z+a.s.z );
	if( ray.z ){	//傾きがある
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1～x2間の外側。交差しない
	}
	return true;
}

bool Intersect3D::Intersect(  BoundingBox3D &a,Vector3 &pos,math::Vector3 &ray,Vector3 *ret )
{
	float min=0,max=FLT_MAX,tmp;		//tmpは入れ替え用
	//各軸で調べる
	float x1 = ( a.p.x-a.s.x ),x2 = ( a.p.x+a.s.x );
	if( ray.x ){						//傾きがある
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1～x2間の外側。交差しない
	}

	float y1 = ( a.p.y-a.s.y ),y2 = ( a.p.y+a.s.y );
	if( ray.y ){									//傾きがある
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{											//傾きなし
		if( pos.y <y1 || pos.y > y2 )return false;	//x1～x2間の外側。交差しない
	}

	float z1 = ( a.p.z-a.s.z ),z2 = ( a.p.z+a.s.z );
	if( ray.z ){	//傾きがある
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1～x2間の外側。交差しない
	}
	*ret = pos + ray * min;
	return true;
}

bool Collision3D::Collision( BoundingOBox3D &a, Vector3 &pos1, Vector3 &ray1 )
{
	float min=0,max=FLT_MAX,tmp;
	Matrix irot( a.ax.x,a.ay.x,a.az.x,0,
				 a.ax.y,a.ay.y,a.az.y,0,
				 a.ax.z,a.ay.z,a.az.z,0,
				 0,0,0,1);

	Vector pos = irot.TransformRotation( pos1 - a.p );
	Vector ray = irot.TransformRotation( ray1 );

	//X軸
	float x1 = -a.s.x ,x2 = +a.s.x;
	if( ray.x ){						//傾きがある
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1～x2間の外側。交差しない
	}

	float y1 = -a.s.y,y2 = a.s.y;
	if( ray.y ){									//傾きがある
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{											//傾きなし
		if( pos.y <y1 || pos.y > y2 )return false;	//x1～x2間の外側。交差しない
	}

	float z1 = -a.s.z,z2 = +a.s.z;
	if( ray.z ){	//傾きがある
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1～x2間の外側。交差しない
	}
	
	return true;
}

bool Intersect3D::Intersect( BoundingOBox3D &a, Vector3 &pos1, Vector3 &ray1, Vector3 *ret)
{
	float min=0,max=FLT_MAX,tmp;
	Matrix irot( a.ax.x,a.ay.x,a.az.x,0,
				 a.ax.y,a.ay.y,a.az.y,0,
				 a.ax.z,a.ay.z,a.az.z,0,
				 0,0,0,1);

	Vector pos = irot.TransformRotation( pos1 - a.p );
	Vector ray = irot.TransformRotation( ray1 );

	//X軸
	float x1 = -a.s.x ,x2 = +a.s.x;
	if( ray.x ){						//傾きがある
		float t1 = (x1-pos.x)/ray.x;//if( t1<0 ) t1 =-(t1);
		float t2 = (x2-pos.x)/ray.x;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
	}else{
		if( pos.x <x1 || pos.x > x2 )return false;	//x1～x2間の外側。交差しない
	}

	float y1 = -a.s.y,y2 = a.s.y;
	if( ray.y ){									//傾きがある
		float t1 = (y1-pos.y)/ray.y;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (y2-pos.y)/ray.y;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp; }			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{											//傾きなし
		if( pos.y <y1 || pos.y > y2 )return false;	//x1～x2間の外側。交差しない
	}

	float z1 = -a.s.z,z2 = +a.s.z;
	if( ray.z ){	//傾きがある
		float t1 = (z1-pos.z)/ray.z;//if( t1<0 ) t1 =-(t1);		//逆数かける。x1までの長さなのでマイナスなら符号反転			
		float t2 = (z2-pos.z)/ray.z;//if( t2<0 ) t2 =-(t2);

		if( t1>t2 ){ tmp=t1;t1=t2;t2=tmp;}			//t1,t2の関係がt1のほうが大きくなってしまった場合入れ替える
		
		if( t1>min ) min = t1;						//最小値更新
		if( t2<max ) max = t2;						//最大値更新
		
		if( min > max ) return false;				//交差してない 
	}else{
		if( pos.y <y1 || pos.y > y2)return false;	//x1～x2間の外側。交差しない
	}
	*ret = pos1 + ray1*min;
	return true;
}

void Frustum::Create( Matrix4x4 &view, float wid, float hei, float near_len, float far_len)
{
	float nw = wid/2; 
	float nh = hei/2;							//幅に対しての割合

	float fw = far_len*nw;
	float fh = far_len*nh;

	//各点の初期化
	p[0].setVector( -nw, +nh,near_len);
	p[1].setVector( +nw, +nh,near_len);
	p[2].setVector( +nw, -nh,near_len);
	p[3].setVector( -nw, -nh,near_len);
	
	p[4].setVector( -fw, +fh,far_len);
	p[5].setVector( +fw, +fh,far_len);
	p[6].setVector( +fw, -fh,far_len);
	p[7].setVector( -fw, -fh,far_len);

	//各点をカメラの位置、座標にする
	for( int i=0;i<8;i++ ) p[i].setVector( p[i] );
	
	//面の初期化
	pnear	= Plane( p[0],p[3],p[2] );
	top		= Plane( p[0],p[1],p[5] );
	bottom	= Plane( p[3],p[7],p[6] );
	left	= Plane( p[0],p[4],p[7] );
	right	= Plane( p[1],p[2],p[6] );
	pfar	= Plane( p[4],p[5],p[6] );

}

void Frustum::Draw( IRender *r, Matrix4x4 &vps, Vector3 &dir, Color &col)
{
	//r->setShedingMode( r->SM_WIRE );
	//r->setCullMode( r->CM_CW );

	WORD index[6][4] = { {0,3,2,1},{0,1,5,4},{3,7,6,2},{0,4,7,3},{1,2,6,5},{4,5,6,7} };
	ColorTVertex tv[4];
	WORD face[6]={ 0,1,2,0,2,3 };
	Color amb = col*0.5f; amb.a=1;
	Vector3 norm[] = { pnear.n,top.n,bottom.n,left.n,right.n,pfar.n };
	for( int i=0;i<6;i++ )
	{
		float dt = -Vector3::Dot( norm[i],dir ); if( dt<0 )dt=0;
		DWORD c = amb + col*dt;

		for(int j=0;j<4;j++ )
		{
			Vector4 v=vps.Transform( p[ index[i][j] ] );
			v.w = 1/v.w;
			v.x*=v.w;v.y*=v.w;v.z*=v.w;
			tv[j] = ColorTVertex( v,c );
		}
		r->drawPolygon( tv,face,4,2 );
	}
	r->setShedingMode( r->SM_GOURAUD );
}

bool Frustum::Collision( BoundingSphere3D &sp)
{
	// 最適化の為、都合に合わせて判定する順番を決めること(よく使うであろう範囲から判定する
	if( pnear.Side( sp.p )+sp.s	< 0 )	return false;
	if( pfar.Side( sp.p )+sp.s < 0 )	return false;
	if( left.Side( sp.p )+sp.s < 0 ) return false;
	if( right.Side( sp.p )+sp.s< 0 ) return false;
	if( top.Side( sp.p )+sp.s  < 0 ) return false;
	if( bottom.Side( sp.p )+sp.s < 0 ) return false;

	return true;						//球が全て+側(視錐台範囲内）
}

bool Frustum::Collision( BoundingOBox3D &obox )
{
	Vector3 pos[8]={ Vector3( -obox.s.x,+obox.s.y,-obox.s.z ),
					 Vector3( +obox.s.x,+obox.s.y,-obox.s.z ),
					 Vector3( +obox.s.x,-obox.s.y,-obox.s.z ),
					 Vector3( -obox.s.x,-obox.s.y,-obox.s.z ),
					 
					 Vector3( -obox.s.x,+obox.s.y,+obox.s.z ),
					 Vector3( +obox.s.x,+obox.s.y,+obox.s.z ),
					 Vector3( +obox.s.x,-obox.s.y,+obox.s.z ),
					 Vector3( -obox.s.x,-obox.s.y,-obox.s.z )
		};
	Matrix4x4 m( obox.ax.x ,obox.ay.x,obox.ax.z, 0,
				 obox.ax.y ,obox.ay.y,obox.ax.y, 0,
				 obox.ax.z ,obox.ay.z,obox.ax.z, 0,
				 obox.p.x,obox.p.y,obox.p.z,1 );
	int i;
	for( i=0;i<8;i++){
		pos[i]=m.TransformVector( pos[i] );
		//都合に合わせて･･･
		if( pnear.Side( pos[i])<0 )	continue;
		if( pfar.Side( pos[i])< 0 )	continue;
		if( left.Side( pos[i])< 0 )	continue;
		if( right.Side( pos[i])< 0 )continue;
		if( top.Side( pos[i])< 0 ) 	continue;
		if( bottom.Side( pos[i])< 0 )continue;
		
		return true;							//全面内側なら
	}
	return false;
}					