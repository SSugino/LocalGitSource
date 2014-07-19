/** @file
 * 3Dオブジェクトクラスの実装
 * @date 2009/07/21
 * @author 杉野
 **/

#include "Object3D.h"
#include "XML.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;
using namespace system;

void Object3D::Create( int v_size,int f_size )
{
	//--------------------------------
	//			確保
	//----------------------------------
	/** 頂点数 */
	vertex.resize( v_size );
	/** テクスチャ頂点 */
	tvertex.resize( vertex.size() );
	/** 面数 */
	face.resize( f_size );
}

//XML形式で読み込み
char *Object3D::Load( char *text )
{
	XMLElement xobj( "OBJECT",text );			//タブ[OBJECT]を見る
	//各種メモリ確保 (テキストから指定文字列を取り出す)
	/** 頂点 */
	vertex.resize( atoi( xobj["vertex"] ) );	//その中でvertex文字列を探す
	/** テクスチャ頂点 */
	tvertex.resize( vertex.size() );			
	/** 面数 */
	face.resize( atoi( xobj["face"] ) );		//その中でface文字列を探す
	/** 名前 */
	name = xobj["name"] ;						//その中でname文字列を探す
	
	int i;
	//POSITION
	{
		XMLElement xpos( "POSITION",xobj.getText() );	//OBJECTタグ内で探す
		char *p = xpos.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( ☆,  ,  )の部分
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   ,☆, 　)
			float z = Float::Atof( p=strstr(p,",")+1 );	//(   ,  , ☆ )

			vertex[i].p.setVector(x,y,z);					//座標初期化
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

	//NORMAL
	{
		XMLElement xnormal( "NORMAL",xobj.getText() );
		char *p = xnormal.getText();
		for( i=0;i<vertex.size();i++ )
		{
			float x = Float::Atof( p=strstr(p,"(")+1 );	//( ☆ ,  ,  )の部分
			float y = Float::Atof( p=strstr(p,",")+1 );	//(   , ☆ , )
			float z = Float::Atof( p=strstr(p,",")+1 );	//(   ,  , ☆ )

			vertex[i].n.setVector(x,y,z);				//座標初期化
		}
	}

	//COLOR
	{
		XMLElement xcol( "COLOR", xobj.getText() );
		char *p = xcol.getText();
		for( i=0;i<vertex.size();i++ )
		{
			DWORD col = strtoul( p=strstr(p,"(")+1 ,NULL,16 );		//16進で取得
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

	return text+xobj.getLength();		//長さ分を返す
}

void Object3D::Transform( const Matrix4x4 &m1, const Matrix4x4 &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vertex3D		&ver = vertex[i];		//頂点データ
		ver.gn = m1.TransformRotation( ver.n );
		ver.gp = m1.TransformVector( ver.p );	//グローバル座標に持ってくる
		Vector4 v = m2.Transform( vertex[i].gp );					//カメラスクリーン変換
		v.w  = 1/v.w;			
		v.x *= v.w;v.y *=v.w;v.z *=v.w;//逆数掛ける

		tvertex[i] = graphics::TextureColorTVertex( v,ver.t,ver.c );
	}
}
//アニメの変換
void Object3D::Transform( Matrix4x4 *m,Matrix &m2 )
{
	for( int i=0;i<vertex.size();i++ )
	{
		Vertex3D	&ver = vertex[i];		//頂点データ
		Matrix		&mm1 = m[ver.blend1];	//ブレンド参照　（どのボーンを参照するか)
		Matrix		&mm2 = m[ver.blend2];
		Matrix m1=(mm1-mm2)*ver.weight+mm2;				//回転に親(weight)の影響を与える
		
		Vector4 v;

		ver.gn = m1.TransformRotation(ver.n );					//カメラスクリーン変換
		ver.gp = m1.TransformVector( ver.p );
		v = m2.Transform( ver.gp );
		v.w  = 1/v.w;			
		v.x *= v.w;v.y *=v.w;v.z *=v.w;//逆数掛ける
		tvertex[i] = graphics::TextureColorTVertex( v,ver.t,ver.c );
			
	}
}

//背景の変換
void Object3D::Transform( Matrix4x4 &vps )
{
	for(int i=0;i<vertex.size();i++)
	{
		Vertex3D &v = vertex[i];
		for(int i=0;i<vertex.size();i++)
		{
			v.gp =v.p; v.gn = v.n;				//背景は変換する必要がない
			Vector4 tv = vps.Transform( v.gp );
			tv.w =1/tv.w;
			tv.x *= tv.w;tv.y *=tv.w;tv.z=1/tv.w;
			tvertex[i] = TextureColorTVertex( tv,v.t,v.c );
		}
	}
}


void Object3D::Draw( graphics::IRender *r)
{
	r->drawPolygon( tvertex,&face[0][0],vertex.size(),face.size() );
}

//----------------------------------------------------------------------------------------
void Object3D::CreateBox( DWORD col )
{
	Create( 8,6*2 );	//頂点数 面数
	//頂点数初期化
	//手前の頂点
	vertex[0].p = math::Vector3( -1,+1,-1 );	vertex[0].c = col;
	vertex[1].p = math::Vector3( +1,+1,-1 );	vertex[1].c = col;
	vertex[2].p = math::Vector3( +1,-1,-1 );	vertex[2].c = col;
	vertex[3].p = math::Vector3( -1,-1,-1 );	vertex[3].c = col;
	//奥の頂点
	vertex[4].p = math::Vector3( -1,+1,+1 );	vertex[4].c = col;
	vertex[5].p = math::Vector3( +1,+1,+1 );	vertex[5].c = col;
	vertex[6].p = math::Vector3( +1,-1,+1 );	vertex[6].c = col;
	vertex[7].p = math::Vector3( -1,-1,+1 );	vertex[7].c = col;
	
	//テクスチャの初期化
	vertex[0].t.setVector( 0,0 );
	vertex[1].t.setVector( 2,0 );
	vertex[2].t.setVector( 2,2 );
	vertex[3].t.setVector( 0,2 );

	vertex[4].t.setVector( 0,0 );
	vertex[5].t.setVector( 2,0 );
	vertex[6].t.setVector( 2,2 );
	vertex[7].t.setVector( 0,2 );


	for(int i=0;i<vertex.size();i++)
	{
		vertex[i].n = vertex[i].p;		//頂点座標をそのまま法線ベクトルに
		vertex[i].n.Normalize();		//単位ベクトルに
	}

	//面初期化 (四角で初期化)
	setFace( &face[0],0,1,2,3 );
	setFace( &face[2],1,5,6,2 );
	setFace( &face[4],4,7,6,5 );
	setFace( &face[6],0,3,7,4 );
	setFace( &face[8],4,5,1,0 );
	setFace( &face[10],7,3,2,6 );
}

void Object3D::CreatePlane( int nx,int nz,DWORD col )
{
	Create( (nx+1)*(nz+1),nx*nz *2 );	//頂点数 面数*2(三角形なので *2)
	//増分求めて頂点数初期化
	float sx,sz=-1,ssx=-1;//ssxは手前のXのスタート地点
	float ax,az;
	ax = 2.0f/nx;	//x幅
	az = 2.0f/nz;	//z幅

	int i,j,n=0;
	for( i=0;i<=nz;i++ )	//x分   <'='　　イコールが大事
	{
		sx=ssx;
		for( j=0;j<=nx;j++ ) //z
		{
			vertex[n].p.setVector( sx,0,sz );
			vertex[n].n.setVector( 0,1,0 );//面の向き
			vertex[n].c  = col;//rand()|0xFF000000;
									//テクスチャ初期化
			sx += ax; n++;//幅分足す / 進める
		}
		sz += az;	//幅分足す
	}

	//面初期化 (四角で初期化)
	n=0;				//面の場所（個数）
	int b1=0;//段
	int b2=nx+1;//b1の上の段
	for( i=0;i<nz;i++ )	//  < イコールいらない
	{
		for( j=0;j<nx;j++ ) //
		{
			setFace( &face[n],b1+j,b2+j,b2+j+1,b1+j+1 );
			n+=2;
		}
		b1+=(nx+1);
		b2+=(nx+1);
	}
}

// nr:円の分割数 *v:回転(頂点)データ nv:データ個数 
void Object3D::CreateRotate( int nr, Vector2 *v, int nv, DWORD col )
{
	Create( nr*nv + 2, nr*(nv-1)*2 + (nr*2) );

	//増分求めて頂点数初期化
	int i,j,n=1;
	float ar = (Math::PI*2)/nr;			//	x幅  分割数/360
	//上の点
	vertex[0].p.setVector( 0,v[0].y,0 );
	vertex[0].c=col;
	//vertex[0].c=rand()|0xFF0000FF;

	//周りの点
	for( i=0;i<nv;i++ )			//縦
	{
		float size,y,r=0;
		size = v[i].x;			//半径
		y	 = v[i].y;			//始点

		for( j=0;j<nr;j++ )		//円ループ
		{
			float z = Math::cos( r )*size;		//Cos
			float x = Math::sin( r )*size;		//Sin
			
			vertex[n].p.setVector( x,y,z );
			vertex[n].c = col;
			//vertex[n].c=rand()|0xFF00FF00;
									//テクスチャ初期化
			n++;
			r += ar;				//円の増分
		}
	}
	//下の点
	vertex[n].p.setVector( 0,v[nv-1].y,0 );
	vertex[n].c = col;
	//vertex[n].c=rand()|0xFFFF0000;
	
	//----------------------------------------------------------------
	n=0;							//面の場所（個数）
	//上の面
	for(int i=0;i<nr;i++) setFace( &face[n++],0,1+i,1+(i+1)%nr );
	//中の面
	int b1=1;						//段
	int b2=nr+1;					//b1の下の段
	
	for( i=1;i<nv;i++ )	//  < イコールいらない
	{
		for( j=0;j<nr;j++ )
		{
			setFace( &face[n],b1+j,b2+j,b2+(j+1)%nr,b1+(j+1)%nr );
			n+=2;					//見ている面を進める
		}
		b1+=nr; //一段下へ
		b2+=nr;	//b1の一段下へ
	}
	//下の面
	
	int end = vertex.size()-1;		//終点の面（中心）
	for(i=0;i<nr;i++)setFace( &face[n++],end,(end-1)-i,(end-1)-(i+1)%nr );		//反時計回りで面を出す	★b1に直す
	//for(i=0;i<nr;i++)setFace( &face[n++],end,b1+i+1,(b1+i)%nr );
	this->Normalize();
}

void Object3D::CreateCylinder( int nr, int n, float top, float bottom, DWORD col )
{
	system::AutoArray< math::Vector2 > v( n+1 );
	float y=1,ay = 2.0f/n ;
	float s = top,as=( bottom-top)/n;

	for( int i=0;i<v.size();i++)
	{
		v[i].setVector( s,y );
		s+= as;
		y-= ay;
	}

	CreateRotate( nr,&v[0],v.size(),col );
}

void Object3D::CreateSphere(int nr, int ny, DWORD col)
{	
	AutoArray< math::Vector2 > v(nr+2);
	float ar = Math::PI/ny;			//一点間の角度 半周で良い
	float r = ar;
	for(int i=0;i<v.size();i++)
	{
		v[i].setVector( Math::sin( r ), Math::cos( r ) );	//円を描くベクトルをセット
		r+=ar;
	}
	CreateRotate( nr,v,ny,col );
	//上の点と下の点を移動
	vertex[0].p.y = +1;
	vertex[ vertex.size()-1 ].p.y = -1;
}

//----------------------------------------------------------------------------------------------------
void Object3D::Normalize()
{
	AutoArray< WORD > count( vertex.size() );//平均値取得用
	int i;
	for(i=0;i<vertex.size();i++)
	{
		vertex[i].n.setVector( 0,0,0 );
		count[i] = 0;
	}
	for(i=0;i<face.size();i++)
	{
		ModelFace &f = face[i];
		const Vector3 &v1 = vertex[f[0]].p;
		const Vector3 &v2 = vertex[f[1]].p;
		const Vector3 &v3 = vertex[f[2]].p;

		Vector3 n = Vector3::Cross( v2-v1,v3-v1 );
		if( n.Length2() )//垂直か？長さはあるか？
		{
			n.Normalize();
			vertex[f[0]].n += n;count[f[0]]++;
			vertex[f[1]].n += n;count[f[1]]++;
			vertex[f[2]].n += n;count[f[2]]++;
		}
	}
	for(i=0;i<vertex.size();i++)
	{
		if(count[i]) vertex[i].n /= (float) count[i];
	}
}

//-----------------------------------------------------------------------------------------
void Object3D::Lighting( Light3D **lights, int count, Color &amb )
{
	int i,j;
	for( i=0;i<vertex.size();i++ )
	{
		Vertex3D &v = vertex[i];
		Color out_col(amb),col( v.c );
		for( j=0;j<count;j++ )
		{
			out_col += lights[j]->GetLightingColor( v.gp, v.gn,col );
		}
		tvertex[i].c = out_col;
	}	
}

void Object3D::Lighting( Light3D &_light, math::Color &amb)
{
	Light3D *light[] = { &_light };
	Lighting( light,1,amb );			//一つだけのライティング処理 countは1
}

//------------------------------------------------------------------------------------------
void Object3D::setFogAlpha( float fog_s, float fog_e )
{
	int i;
	float fog_len = fog_e - fog_s;							//範囲

	for( i=0;i<vertex.size();i++ )
	{
		Color c( tvertex[i].c );
		float fz = ( (1/tvertex[i].w)-fog_s )/fog_len;		//0～1が求まる
		if( fz<0 ) fz=0;else if( fz>1.0f )fz = 1.0f;
		//アルファ値で背景に溶け込ます
		tvertex[i].c = tvertex[i].c & 0xFFFFFF|(DWORD)( 255-fz*255 )<<24;	//アルファ値のみを減らしていく
	}
}

void Object3D::setFogColor( float fog_s, float fog_e, Color &fog_col )
{
	int i;
	float fog_len = fog_e - fog_s;							//範囲

	for( i=0;i<vertex.size();i++ )
	{
		Color c( tvertex[i].c );
		float fz = ( (1/tvertex[i].w)-fog_s )/fog_len;		//0～1が求まる
		if( fz<0 ) fz=0;else if( fz>1.0f )fz = 1.0f;
		//フォグの色（背景色）に補間で溶け込ます
		tvertex[i].c = ( fog_col - c )*fz + c;
	}
}

//---------------------------------------------------------------------------------------------
void Model3D::Load( const char *file_name )
{
	DWORD size;
	system::AutoPtr<char> text;

	FILE *fp = fopen( file_name,"rb" );		//指定ファイルを読み込む
	if( fp==NULL ) throw system::Exception( "モデルファイルオープンエラー" );
	fseek( fp,0,2 );
	size = ftell(fp);						//ファイルサイズ取得
	fseek( fp,0,0 );
	
	text = new char[size+1];
	fread( text,size,1,fp );
	fclose( fp );							//ここまで読み込み
	
	/** モデルの読み込み */
	XMLElement xmodel( "MODEL3D",text );//先ほど読み込んだテキストから"MODEL3D"タグ検索
	int cnt = atoi( xmodel["count"] );	//その中でcount文字列を探し　文字を数値変換
	
	objs.resize( cnt );					//モデル個数(count)をメンバobjsにメモリ確保

	char* p = xmodel.getText();			//今のテキスト情報をとっておき
	for(int i=0;i<cnt;i++ )
	{
		p = objs[i].Load(p);			//Object3D::Load戻り値を受け取る=次のオブジェクトを参照する
	}

	/** ボーンの読み込み */
	try{											//BONE_LISTがないかもしれない･･･
		XMLElement xbone( "BONE_LIST",text );

		bones.resize( atoi( xbone["count"]) );		//ボーン個数メモリ確保
		char* p2=xbone.getText();					//BONE_LIST文字列を保存
		for(int i=0;i<bones.size();i++ )
		{
			XMLElement xbone3d( "BONE3D",p2 );
			bones[i].name = xbone3d["name"] ;
			bones[i].local.x = Float::Atof(xbone3d["x"] );	//属性値で( )で括られてないので
			bones[i].local.y = Float::Atof(xbone3d["y"] );
			bones[i].local.z = Float::Atof(xbone3d["z"] );
			bones[i].parent  = atoi( xbone3d["parent"] );
			p2+=xbone3d.getLength();							//次ボーンを参照
		}
	 }catch( system::Exception& ){}
}

//------------------------------------------------------------------------------------------
int ModelAnimation3D::setMotion( Matrix4x4 &root, KeyFrame3D *kf, float *time )
{
	int n = 0,ret = 0;
	//現在がどのモーションをしているか調べる
	while( kf[n+1].motion && kf[n+1].time < *time ) n++;

	//今のモーションと次のモーション間の時間を0～1.0にする
	float t = *time - kf[n].time;
	t = t/( kf[n+1].time - kf[n].time);
	if( t>1 ){ t = 1; }

	Motion3D *now = kf[n].motion;					//現在のアニメーション
	Motion3D *next = kf[n+1].motion;				//次のアニメーション

	//次のモーションがない場合は先頭に戻す
	if(next==0){
		next=kf[0].motion;
		if(t>=1){ *time=0; ret=1; }
	}

	int i;
	Matrix m;
	for( i=0;i<mat.size();i++ )
	{
		//補間
		Quaternion &q = Quaternion::Slerp( now->rot,next->rot, t );

		mat[i] = Matrix(q);			//回転行列を取得
		now++;next++;
		//Boneを原点として回転させた行列に
		Vector &v = bone[i].local;
		m.RotatePos( mat[i] ,v );		//任意点で回転させる

		if( i ) mat[i] = m*mat[ bone[i].parent ];
		else    mat[0] = m*root;
	}
	return ret;
}