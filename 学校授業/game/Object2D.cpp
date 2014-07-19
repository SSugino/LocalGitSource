/** @file
 * 2Dオブジェクトクラスの実装
 * @date 2009/05/27
 * @author 杉野
 **/

#include "Object2D.h"
#include "XML.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

void Object2D::Create(int n, float size, DWORD col, float sr )
{
	//--------------------------------
	//			確保
	//----------------------------------
	/** 頂点数 */
	vertex.resize( n+1 );
	/** テクスチャ頂点 */
	tvertex.resize( n+1 );
	/** 面数 */
	face.resize( n );
	/** 辺数 */
	edge.resize( n );

	/** 作成 */
	int i;
	
	/** ローカル座標 */
	vertex[0].p = Vector2::zero;
	/** テクスチャUV座標 */
	vertex[0].t = Vector2( 0.5f,0.5f );
	vertex[0].c= col;

	for(i=1;i<vertex.size();i++)
	{
		/** ローカル座標 */
		vertex[i].p = Vector2(sr)*size;
		/** テクスチャ座標 */
		vertex[i].t		= ( vertex[i].p/2 );
		vertex[i].t.y	=  -vertex[i].t.y;
		vertex[i].t	   +=	vertex[0].t;
		/** 頂点色 */	
		vertex[i].c = col;
		/** 角度移動 */
		sr -=( Math::PI*2/n );	// 角数 / 360 = 1角間の角度 
	}

	//面データ作成
	for(i=0;i<face.size();i++)
	{
		/** 面を作成 */
		setFace( &face[i],0,i+1,(i+1)%n+1 );
	}
	//線データ作成
	for(i=0;i<edge.size();i++)
	{
		setEdge( &edge[i],i+1,(i+1)%n+1 );
	}
}

void Object2D::Transform( math::Vector2 &pos,float zoom,int w,int h )
{
	int i;
	int wid = w/2;
	int hei = h/2;
 
	for( i=0;i<vertex.size();i++ )
	{
		Vector2 tv = vertex[i].p;
		tv = (tv * zoom) + pos;
		tv.x =  ( tv.x * hei )+(float)wid;
		tv.y =  ( tv.y * (-hei) )+(float)hei;

		tvertex[i] = graphics::TextureColorTVertex( tv,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( math::Vector2 &pos,math::Vector2 &scale,float r,int w,int h )
{
	int i;
	float c = Math::cos( r );
	float s = Math::sin( r );
	int wid = w/2;
	int hei = h/2;

	for( i=0;i<vertex.size();i++ )
	{
		Vector2 tv = vertex[i].p, v=tv;
		
		/** 拡大 */
		v.x = (tv.x * scale.x );			//式は分解する
		v.y = (tv.y * scale.y );
		
		/** 回転 */
		tv.x = (v.x*c) - (v.y*s); 
		tv.y = (v.x*s) + (v.y*c);


		/** 移動 */
		tv = ( tv + pos );
		
		tv.x =  ( tv.x * hei )+(float)wid;
		tv.y =  ( tv.y * (-hei) )+(float)hei;

		tvertex[i] = graphics::TextureColorTVertex( tv,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( const Matrix3x3 &m, const Matrix3x3 &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vector2 p;
		vertex[i].gp = m.TransformVector( vertex[i].p );	//原点移動変換
		p = m2.TransformVector( vertex[i].gp );				//カメラスクリーン変換	
		tvertex[i] = graphics::TextureColorTVertex( p,vertex[i].t,vertex[i].c );
	}
}

void Object2D::Transform( Matrix3x3 *m, Matrix3x3 &cm)
{
	int i;
	for( i=0;i<vertex.size();i++)
	{
		/** 自分の行列 */
		Matrix3x3 &m1 = m[vertex[i].blend1 ];
		/** 親の行列 */
		Matrix3x3 &m2 = m[vertex[i].blend2 ];
		/** m1とm2の補間 */
		Matrix3x3 mm = ( m1 - m2  ) * vertex[i].weight + m2 ;

		vertex[i].gp = mm.TransformVector( vertex[i].p );
		Vector2 v	 = cm.TransformVector( vertex[i].gp );
		tvertex[i]	 = TextureColorTVertex( v,vertex[i].t,vertex[i].c );
	}
}




void Object2D::Draw(sugino::graphics::IRender *r)
{
	r->drawPolygon( tvertex,&face[0][0],vertex.size(),face.size() );
}

void Object2D::DrawEdge(sugino::graphics::IRender *r)
{
	r->drawLine( tvertex,&edge[0][0],vertex.size(),edge.size() );
}

void Object2D::Lighting( Light2D *lights, int l_count )
{
	int i,j;
	for( i=0;i<vertex.size();i++ )
	{
		Vector2 &pos = vertex[i].gp;		//頂点位置
		Color c( vertex[i].c );				//頂点色
		for( j=0;j<l_count;j++ )
		{
			lights[j].getBrightColor( &c,pos );
		}
		tvertex[i].c=c;
	}
}

//テキストから読み込むCreate関数
char *Object2D::Create( char *text )
{
	XMLElement xobj( "OBJECT",text );			//タブ[OBJECT]を見る
	//各種メモリ確保 (テキストから指定文字列を取り出す)
	/** 頂点 */
	vertex.resize( atoi( xobj["vertex"] ) );	//その中でvertex文字列を探す
	/** テクスチャ頂点 */
	tvertex.resize( vertex.size() );			//その中で
	/** 面数 */
	face.resize( atoi( xobj["face"] ) );		//その中でface文字列を探す
	/** 辺数 */
	edge.resize( atoi( xobj["edge"] ) );		//その中でedge文字列を探す
	/** 名前 */
	name = xobj["name"] ;						//その中でname文字列を探す
	
	int i;
	//POSITION
	{
		XMLElement xpos( "POSITION",xobj.getText() );	//OBJECTタグ内で探す
		char *p = xpos.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( ☆ ,  ,  )の部分
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , ☆ , )
			vertex[i].p.setVector(x,y);					//座標初期化
		}
	}
	
	//COLOR
	{
		XMLElement xcol( "COLOR", xobj.getText() );
		char *p = xcol.getText();
		for( i=0;i<vertex.size();i++ )
		{
			DWORD col = strtoul( p=strstr(p,"(")+1 ,NULL,16 );
			vertex[i].c = col;
		}
	}
	
	//UV
	{
		XMLElement xuv( "UV",xobj.getText() );
		char *p = xuv.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( ☆ ,  ,  )の部分
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , ☆ , )
			vertex[i].t.setVector(x,y);					//座標初期化
		}
	}
	
	//WEIGHT
	{
		XMLElement xwei( "WEIGHT",xobj.getText() );
		char *p = xwei.getText();
		for( i=0;i<vertex.size();i++)
		{
			float wei = Float::Atof( p=strstr(p,"(")+1 );	//( ☆ ,  ,  )の部分
			vertex[i].weight = wei;
		}
	}

	//BLEND_INDEX
	{
		XMLElement	xblend( "BLEND_INDEX",xobj.getText() );
		char *p = xblend.getText();
		for( i=0;i<vertex.size();i++)
		{
			BYTE blend1 = (BYTE)atoi(  p=strstr(p,"(")+1 );
			BYTE blend2 = (BYTE)atoi(  p=strstr(p,",")+1 );
			vertex[i].blend1 = blend1;
			vertex[i].blend2 = blend2;
		}
	}
	//FACE 
	{
		XMLElement xface( "FACE",xobj.getText() );
		char *p = xface.getText();
		for( i=0;i<face.size();i++ )
		{
			WORD n1 = (WORD)atoi( p=strstr(p,"(")+1 );
			WORD n2 = (WORD)atoi( p=strstr(p,",")+1 );
			WORD n3 = (WORD)atoi( p=strstr(p,",")+1 );
			setFace( &face[i],n1,n2,n3 );
		}
	}

	//EDGE
	{
		XMLElement xedge( "EDGE",xobj.getText() );
		char *p = xedge.getText();
		for(i=0;i<edge.size();i++)
		{
			WORD n1 = (WORD)atoi( p=strstr(p,"(")+1 );
			WORD n2 = (WORD)atoi( p=strstr(p,",")+1 );
			setEdge( &edge[i],n1,n2 );
		}
	}
	return text+xobj.getLength();		//長さ分を返す
}

void Object2D::BlendObject( Object2D& obj1,Object2D &obj2,float t )
{
	/** 頂点数が異なる場合は処理しない */
	if( this->vertex.size() != obj1.vertex.size() ||
		obj1.vertex.size()	!= obj2.vertex.size() ) return;

	for( int i=0;i<vertex.size();i++ )
	{
		const Vector2 &v1 = obj1.vertex[i].p;
		const Vector2 &v2 = obj2.vertex[i].p;

		vertex[i].p = ( v2 - v1 )*t + v1;			//線形補間
	}
}


void Model2D::Load( char *name )
{
	DWORD size;
	system::AutoPtr<char> text;

	FILE *fp = fopen( name,"rb" );		//指定ファイルを読み込む
	if( fp==NULL ) throw system::Exception( "ファイルがオープンエラー" );
	fseek( fp,0,2 );
	size = ftell(fp);					//ファイルサイズ取得
	fseek( fp,0,0 );
	
	text = new char[size+1];
	fread( text,size,1,fp );
	fclose( fp );						//ここまで読み込み
	
	/** モデルの読み込み */
	XMLElement xmodel( "MODEL2D",text );//先ほど読み込んだテキストから"MODEL2D"タグ検索
	int cnt = atoi( xmodel["count"] );	//その中でcount文字列を探し　文字を数値変換
	
	objs.resize( cnt );					//メンバobjsメモリ確保

	char* p = xmodel.getText();			//今のテキスト情報をとっておき
	for(int i=0;i<cnt;i++ )
	{
		p = objs[i].Create(p);			//Create戻り値を受け取る=次のオブジェクトを参照する
	}

	/** ボーンの読み込み */
	XMLElement xbone( "BONE_LIST",text );
	
	bones.resize( atoi( xbone["count"]) );		//ボーン個数メモリ確保
	char* p2=xbone.getText();					//BONE_LIST文字列を保存
	for(int i=0;i<bones.size();i++ )
	{
		XMLElement xbone2d( "BONE2D",p2 );
		bones[i].name = xbone2d["name"] ;
		bones[i].local.x = Float::Atof(xbone2d["x"] );	//属性値で( )で括られてないので
		bones[i].local.y = Float::Atof(xbone2d["y"] );
		bones[i].parent  = atoi( xbone2d["parent"] );
		p2+=xbone2d.getLength();							//次ボーンを参照
	}
}


int ModelAnimation2D::setMotionMatrix( KeyFrame2D *kf,float *time )
{
	int n = 0,ret = 0;
	/** 再生位置検索 */
	while( kf[n+1].motion && kf[n+1].time< *time ) n++;		//引数キーフレーム配列のmotionと時間が一致しているか
	
	/** 補間値(0.0～1.0)を決める */
	float start = kf[n].time;	//開始時間保存
	float end = kf[n+1].time;	//終了時間保存
	/** 時間の補間 */
	float t = ( *time - start ) / ( end - start );

	Motion2D *now = kf[n+0].motion;		//今のモーション
	Motion2D *next = kf[n+1].motion;	//次のモーション

	if( next==NULL )
	{
		next = kf[0].motion;			//0番目参照
		ret=2;							//最後まで来たなら
		if( t>=1 )						//補間値最大なら
		{ *time=0; ret=1; }				//時間を0にしループアニメ終了
	}

	/** 補間 */
	int i;
	for( i=0;i<amat.size();i++,now++,next++ )
	{
		float	r = (	next->r		- now->r	)*t	+now->r;
		Vector2 s = (	next->size	- now->size	)*t +now->size;
		/** 行列掛け合わせ */
		amat[i] = Matrix3x3().Scale( s )  *  Matrix3x3().Rotate( r ); 

	}

	return ret;
}

void ModelAnimation2D::setBoneMatrix( Matrix3x3 &root )
{
	int i;
	Matrix3x3 t1,t2;
	for( i=0;i<amat.size();i++)
	{
		Vector2 &v = bone[i].local;
		t1.Translate( -v );			//始点を親ボーンに
		t2.Translate( v );

		if( i )
		{
			amat[i] = ( t1 * amat[i] * t2 ) * amat[bone[i].parent];	//親と合成
		}else{
			amat[i] = ( t1 * amat[i] * t2 ) * root;					//任意の行列
		}
	}
}
