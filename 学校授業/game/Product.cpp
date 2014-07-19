/** @file
 * 演出クラスの実装
 * @date 2009/08/27
 * @author 杉野
 **/

#include "Product.h"
using namespace sugino;
using namespace math;
using namespace game;
using namespace	graphics;

//３Ｄ空間に線を描画 (汎用性は･･･ )
int Poly::PolyLine( IRender *render,Vector3 &s, Vector3 &e, float w, Vector3 &cpos, Matrix4x4 &vps )
{
	Vector ray,cray,rv;
	ray = e - s;	//ラインの方向
	cray = e - cpos;	//カメラ位置から終点の方向
	rv = Vector3::Cross( ray,cray );
	if( rv.Length() == 0 ) return 1;	//カメラと同じ方向は面が作れないので戻す
	rv = rv.Normalize() * w;
	
	//各頂点を求める
	Vector v1,v2,v3,v4;
	v1 = s - rv;		//始点左
	v2 = e - rv;		//終点左
	v3 = e + rv;		//終点右
	v4 = s + rv;		//始点右

	//各頂点位置をテクスチャ座標とする
	Vector2 t1,t2,t3,t4;
	t1 = Vector2( v1.x,v1.y );
	t2 = Vector2( v2.x,v2.y );
	t3 = Vector2( v3.x,v3.y );
	t4 = Vector2( v4.x,v4.y );
	
	drawSquare( render,v1,v2,v3,v4,Color( 0xffFFFFFF ),t1,t2,t3,t4,vps);
	return 0;
}

void Poly::drawSquare( IRender *render,Vector3 &v1, Vector3 &v2, Vector3 &v3, Vector3 &v4,const DWORD col, Vector2 &t1, Vector2 &t2, Vector2 &t3, Vector2 &t4, Matrix4x4 &vps)
{
	Vector4 tv1,tv2,tv3,tv4;
	tv1 = vps.Transform( v1 ); tv1.w = 1/tv1.w;	
	tv1.x *= tv1.w;tv1.y *=tv1.w;tv1.z*=tv1.w;	//各要素にｗかける

	tv2 = vps.Transform( v2 ); tv2.w = 1/tv2.w;	
	tv2.x *= tv2.w;tv2.y *=tv2.w;tv2.z*=tv2.w;

	tv3 = vps.Transform( v3 ); tv3.w = 1/tv3.w;	
	tv3.x *= tv3.w;tv3.y *=tv3.w;tv3.z*=tv3.w;

	tv4 = vps.Transform( v4 ); tv4.w = 1/tv4.w;	
	tv4.x *= tv4.w;tv4.y *=tv4.w;tv4.z*=tv4.w;

	//面を作る
	TextureColorTVertex tv[4];
	tv[0] = TextureColorTVertex( tv1,t1,col );
	tv[1] = TextureColorTVertex( tv2,t2,col );
	tv[2] = TextureColorTVertex( tv3,t3,col );
	tv[3] = TextureColorTVertex( tv4,t4,col );

	WORD face[6]={0,1,2,0,2,3};
	render->drawPolygon( tv,face,4,2 );
}
//---------------------------------------------------------------------------

void PolyList::addPoint( Vector3 &p1,Vector3 &p2,Color &col )
{
	PolyPoint p;
	p.v1 = p1;
	p.v2 = p2;
	p.c  = col;

	plist.push_back( p ); 
}

void PolyList::Move()
{
	std::list<PolyPoint>::iterator i =	plist.begin();	//先頭(begin)に
	while(i != plist.end()){								//終わるまで
		//動かす
		math::Vector3 dir1,dir2;	//方向
		
		dir1 = i->v2 - i->v1;	//点2への方向
		dir2 = i->v1 - i->v2;	//点1への方向
		/*
		dir1.Normalize();
		i->v1 = dir1*1.2f;		//とりあえず方向のほうへ向かっていく
		dir2.Normalize();
		i->v2 = dir2*1.2f;
		*/
		//アルファ値いじる
		i->c.a-=0.005f;

		//死亡処理 とりあえずアルファが無くなったら
		if( i->c.a<0 ) i = plist.erase(i);
		else		++i;
	}
}

void PolyList::Draw( IRender *render,Matrix4x4 &vps )
{
	if( plist.size() < 2 ) return;
	std::list< PolyPoint >::iterator	i = plist.begin();
	PolyPoint *p1,*p2;
	
	p1=&(*i);				//一つ前のデータ
	++i;				
	while( i!=plist.end() ){
		
		//テクスチャ座標
		math::Vector2 t1,t2,t3,t4;
		t1 = math::Vector2( i->v1.x,i->v1.y );
		t2 = math::Vector2( p1->v1.x,p1->v1.y );
		t3 = math::Vector2( p1->v2.x,p1->v2.y );
		t4 = math::Vector2( i->v2.x,i->v2.y );
		
		p2 = &(*i);				//新しいデータ
		drawSquare( render,p1->v1,p2->v1,p1->v2,p2->v2,i->c,t1,t2,t3,t4,vps );
		++i;
		p1 = p2;				//現在のデータを更新
	}
}

void PolyList::drawSquare( IRender *render,Vector3 &v1, Vector3 &v2, Vector3 &v3, Vector3 &v4, DWORD col, Vector2 &t1, Vector2 &t2, Vector2 &t3, Vector2 &t4, Matrix4x4 &vps)
{
	Vector4 tv1,tv2,tv3,tv4;
	tv1 = vps.Transform( v1 ); tv1.w = 1/tv1.w;	
	tv1.x *= tv1.w;tv1.y *=tv1.w;tv1.z*=tv1.w;	//各要素にｗかける

	tv2 = vps.Transform( v2 ); tv2.w = 1/tv2.w;	
	tv2.x *= tv2.w;tv2.y *=tv2.w;tv2.z*=tv2.w;

	tv3 = vps.Transform( v3 ); tv3.w = 1/tv3.w;	
	tv3.x *= tv3.w;tv3.y *=tv3.w;tv3.z*=tv3.w;

	tv4 = vps.Transform( v4 ); tv4.w = 1/tv4.w;	
	tv4.x *= tv4.w;tv4.y *=tv4.w;tv4.z*=tv4.w;

	//面を作る
	TextureColorTVertex tv[4];
	tv[0] = TextureColorTVertex( tv1,t1,col );
	tv[1] = TextureColorTVertex( tv2,t2,col );
	tv[2] = TextureColorTVertex( tv3,t3,col );
	tv[3] = TextureColorTVertex( tv4,t4,col );

	WORD face[6]={0,1,2,0,2,3};
	render->drawPolygon( tv,face,4,2 );
}

void PolyList::drawSquare( IRender *render,Vector3 &v1, Vector3 &v2, Vector3 &v3, Vector3 &v4, DWORD col, Matrix4x4 &vps)
{
	Vector4 tv1,tv2,tv3,tv4;
	tv1 = vps.Transform( v1 ); tv1.w = 1/tv1.w;	
	tv1.x *= tv1.w;tv1.y *=tv1.w;tv1.z*=tv1.w;	//各要素にｗかける

	tv2 = vps.Transform( v2 ); tv2.w = 1/tv2.w;	
	tv2.x *= tv2.w;tv2.y *=tv2.w;tv2.z*=tv2.w;

	tv3 = vps.Transform( v3 ); tv3.w = 1/tv3.w;	
	tv3.x *= tv3.w;tv3.y *=tv3.w;tv3.z*=tv3.w;

	tv4 = vps.Transform( v4 ); tv4.w = 1/tv4.w;	
	tv4.x *= tv4.w;tv4.y *=tv4.w;tv4.z*=tv4.w;

	//面を作る
	TextureColorTVertex tv[4];
	tv[0] = TextureColorTVertex( tv1,Vector2(0,0),col );
	tv[1] = TextureColorTVertex( tv2,Vector2(1,0),col );
	tv[2] = TextureColorTVertex( tv3,Vector2(1,1),col );
	tv[3] = TextureColorTVertex( tv4,Vector2(0,1),col );

	WORD face[6]={0,1,2,0,2,3};
	render->drawPolygon( tv,face,4,2 );
}