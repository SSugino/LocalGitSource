/** @file
 * 2Dオブジェクトクラスの定義
 * @date 2009/05/27
 * @author 杉野
 **/

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

#pragma once

namespace sugino{
namespace game{
	class Light2D;	//仮定義

	/** 辺 */
	typedef WORD ModelEdge[2];				
	
	/** 面 */
	typedef WORD ModelFace[3];
	
	/** 2D頂点構造体 */
	struct Vertex2D
	{
		/** ローカル座標 */
		math::Vector2 p;
		
		/** 頂点色 */
		DWORD	c;
		
		/** グローバル座標時の頂点データ */
		math::Vector2 gp;

		/** テクスチャ座標 */
		math::Vector2 t;
		
		/** ボーン行列のインデックス */
		WORD blend1;
		
		/** ボーン行列のインデックス(親番号) */
		WORD blend2;
		/** ウェイト値 */
		float weight;

		Vertex2D(){}
		Vertex2D(math::Vector2 &pos,DWORD col,math::Vector2 &tex ) :
		p(pos),c(col),t(tex){ }
		
	};

	/** 2Dモーションデータ構造体 */
	struct Motion2D
	{
		/** 角度 */
		float		r;
		/** 大きさ */
		math::Vector2 size;

		Motion2D(){}
		Motion2D( float rr,const math::Vector2 &s ) : r(rr),size(s) {}
		/** ラジアン値初期化 */
		Motion2D( float rr ) : r(rr),size( 1,1) {}
		/** 角度指定 */
		Motion2D( int rr ) : r(rr*math::Math::PI/180),size(1,1) {}
	};

	/** 2Dキーフレームデータ構造体
	 *	アニメーションの終了はNULL	
	 **/
	struct KeyFrame2D
	{
		/** 時間 */
		float		time;		//何タイム目に"コレ"を出すとなる
		/** 角度やサイズの入ったモーションデータを受け取る */
		Motion2D	*motion;
	};
	
	/** 2Dボーン(関節)データ構造体 */
	struct Bone2D
	{
		/** ボーン名 */
		std::string name;		
		/** 原点 */
		math::Vector2 local;
		/** 親ボーン番号 */
		int parent;
		
		Bone2D(){}
		Bone2D( math::Vector2 &v,int p ) : local(v),parent(p){} 
	};

	/** 2D図形オブジェクト */
	class Object2D
	{
	private:
		/** 頂点 */
		system::AutoArray<Vertex2D>							vertex;
		/** テクスチャ */
		system::AutoArray<graphics::TextureColorTVertex>	tvertex;
		/** 面 */
		system::AutoArray<ModelFace>						face;
		/** 辺 */
		system::AutoArray<ModelEdge>						edge;

		/** オブジェクト名 */
		std::string											name;
	public:
		/** 多角形の作成
		 * @param [in] int	 n		:角数
		 * @param [in] floatg size	:サイズ
		 * @param [in] DWORD  col	:色
		 * @param [in] float  sr	:開始角度
		 **/
		void Create( int n,float size,DWORD col,float sr=0 );
		/** テキストデータからおぶじぇくとを初期化する
		 * @param [in] text	: テキストデータ
		 * @return 読み込んだテキストの終了アドレス
		 **/
		char *Create( char *text );
		/** 座標変換
		 * @param [in] Vector2 &pos	: 数学座標系の座標
		 * @param [in]   float zoom : 拡大サイズ(0.0-1.0)
		 * @param [in]	   int  w	: 画面幅
		 * @param [in]	   int  h	: 画面高さ
		 **/
		void Transform( math::Vector2 &pos,float zoom,int w,int h );
		/** 座標変換
		 * @param [in] Vector2 &pos		: 数学座標系の座標
		 * @param [in] Vector2 &scale	: 拡大サイズの座標(0.0-1.0)
		 * @param [in]	 float  r		: ラジアン値(PI/180･･･etc)
		 * @param [in]	   int  w		: 画面幅
		 * @param [in]	   int  h		: 画面高さ
		 **/
		void Transform( math::Vector2 &pos, math::Vector2 &scale,float r,int w,int h );
		/** 座標変換(行列)
		 * @param [in] Matrix3x3 &m		:行列( 移動･回転･スケール )何でもかまわない（グローバル変換用）
		 * @param [in] Matrix3x3 &m2	:行列( 移動･回転･スケール・カメラ )
		 **/
		void Transform( const math::Matrix3x3 &m, const math::Matrix3x3 &m2 );
		
		/** 座標変換 行列をブレンドする
		 * @param [in] Matrix3x3 &m		:行列の配列
		 * @param [in] Matrix3x3 &m2	:
		 **/
		void Transform( math::Matrix3x3 *m, math::Matrix3x3 &m2 );

		/** オブジェクトの面描画
		 * @param [in] IRender *r		:画像元　
		 **/
		void Draw( graphics::IRender *r );

		/** オブジェクトの線を描画する
		 * @param [in] IRender *r		:画像元　
		 **/
		void DrawEdge( graphics::IRender *r );

		/** モデルのモーフィング
		 * @param [in]	Object2D &obj	:モデル１
		 * @param [in]	Object2D obj2	:モデル２
		 * @param [in]	   float  t		:補間値
		 **/
		void BlendObject( Object2D &obj1,Object2D &obj2,float t );

		//--------------------------------------------------------
		//	utility functions ユーティリティ関数
		//--------------------------------------------------------
		/** 面を初期化
		 * @param [out]	f:	初期化する面
		 * @param [in] n1:	インデックス１
		 * @param [in] n2:	インデックス２
		 * @param [in] n3:	インデックス３
		 **/
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3 )
		{
			(*f) [0]=n1;
			(*f) [1]=n2;
			(*f) [2]=n3;
		}

		/** 辺を初期化
		 * @param [out] f:	初期化する辺
		 * @param [in] n1:	インデックス１
		 * @param [in] n2:	インデックス２
		 **/
		static void setEdge( ModelEdge *f,WORD n1,WORD n2 )
		{
			(*f)[0]=n1;
			(*f)[1]=n2;
		}

		//----------------------------------------------------------
		// getter
		//----------------------------------------------------------
		/** 頂点取得 */
		Vertex2D &getVertex( int i ){ return vertex[i]; }

		/** 頂点"数"取得 */
		int getVertexCount(){ return vertex.size(); }

		/** 面取得 */
		ModelFace &getFace( int i ){ return face[i]; }

		/** 面"数"取得 */
		int getFaceCount(){ return face.size(); }

		/** 辺取得 */
		ModelEdge &getEdge( int i ){ return edge[i]; }

		/**辺"数"取得 */
		int getEdgeCount(){ return edge.size(); }

		/** 光源の計算
		 * @param [out] Light2D Lights	: 加えたい光源色
		 * @param [out] int		l_count : ライティングさせる個数
		 **/
		void Lighting( Light2D *lights,int l_count );
		/** プリレンダリング(あらかじめ作る) 
		 *	頂点データを今の頂点に代入していく
		 **/
		void PreTransform()
		{ 
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].p = vertex[i].gp;
			}
		}
		/** プリレンダリングのライティング
		 *  頂点数分の色に面の色を代入していく
		 **/
		void PreLighting()
		{
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].c = tvertex[i].c;
			}
		}
		/** 色のセット */
		void setColor( DWORD c )
		{
			for(int i=0;i<vertex.size();i++) vertex[i].c = c;
		}

		/** 名前の取得 */
		const std::string &getName(){ return  name; } 
	};

	/** 2D図形光源クラス */
	class Light2D
	{
	public:
		/** 光源の位置 */
		math::Vector2 pos;
		/** 光の色 */
		math::Color dif;
		/** 減衰度(Attenuation) */
		float att;
		/** コンストラクタ */
		Light2D(){}
		/** 初期化
		 * @param [in] Vector2	 &p	: 光源のベクトル座標
		 * @param [in] Color	&di : 光源の色
		 * @param [in] float	 at : 減衰度（光源の届く距離) 0～∞
		 **/
		Light2D( math::Vector2 &p,math::Color &di,float at ) { pos=p; dif = di;att=1/at; }
		
		/** 自分自身の初期化
		 * @param [in] Vector2	 &p	: 光源のベクトル座標
		 * @param [in] Color	&di : 光源の色
		 * @param [in] float	 at : 減衰度（光源の届く距離)
		 **/
		void Init( math::Vector2 &p,math::Color &di,float at ) { *this=Light2D( p,di,at ) ;}
		
		/** デストラクタ */
		virtual ~Light2D(){}

		/** 引数の明るさ分、引数の頂点に加えていく
		 * @param[out] Color	*oc	:頂点の色
		 * @param[out] Vector2	&vp :出したい色の位置
		 **/
		virtual void getBrightColor( math::Color *oc,math::Vector2 &vp ) const
		{
			float length = (vp-pos).Length()*att;
			float bright = 1-length;
			if( bright<0 ) return;
			if( bright>1 ) bright =1;
			*oc += dif*bright;		//明るさ分頂点カラーに加える
		}
	};
	
	/** 2Dオブジェクトまとめて扱うクラス */
	class Model2D
	{
	private:
		/** オブジェクト配列 */
		system::AutoArray<Object2D> objs;			//複数なので's'が付いてる
		/** ボーン配列 */
		system::AutoArray<Bone2D>	bones;
	public:
		Model2D(){};
		
		/** ロード
		 * @param [in] char* name	:読み込むファイル
		 **/
		void Load( char *name );

		/** 破棄 */
		void Dispose(){ objs.dispose(); }

		/** オブジェクト数の取得 */
		int getObjectCount(){ return objs.size(); }

		/** 参照
		 * @param [in] no	:参照したい配列の要素
		 * @return objs[no]	:参照した配列
		 **/
		Object2D &operator[] (int no){ return objs[no]; }

		/** 名前から参照
		 * @param[in] const char* name	:モデル名
		 * @return	objs[i]				:モデルの配列
		 **/
		Object2D &operator[] ( const char* name )
		{
			for( int i=0;i<objs.size();i++ )
			{
				if( objs[i].getName() == name ) return objs[i];	//名前が一致したらその配列を返す
			}
			throw system::Exception( "存在しないモデルを参照しました(モデル名確認)" );
		}


		/** ボーン配列取得 */
		Bone2D *getBone(){ return bones; }
		/**	指定した配列から取得 */
		Bone2D &getBone(int no){ return bones[no]; };
		/** 指定した名前から取得 */
		Bone2D &getBone( const char *name )
		{
			for( int i=0;i<bones.size();i++ )
			{
				if( bones[i].name == name ) return bones[i];	//名前が一致したらその配列を返す
			}
			throw system::Exception( "存在しないボーンを参照しました(ボーン名確認)" );
		};

		int getBoneCount() { return bones.size(); };

	};
	
	/** １つのモデルで複数のアニメーションが出来るクラス */
	class ModelAnimation2D
	{
	private:
		/** 参照用のボーンポインタ */
		Bone2D					*bone;
		/** アニメーション行列の配列 */
		system::AutoArray<math::Matrix3x3>	amat;
		
		/** モーションの設定
		 * @param [out] KeyFrame *kf	:キーフレーム
		 * @param [out] float *time		:時間
		 **/
		int setMotionMatrix( KeyFrame2D *kf,float *time);
		/** ボーンの設定
	 	 * @param [out] Matrix3x3 &root	:全部に掛ける行列
		 **/
		void setBoneMatrix( math::Matrix3x3 &root );
		
		
	public:
		void Init( Bone2D *b,int bc ) { bone=b; amat.resize(bc); }
		void Init( Model2D &m ) { Init( m.getBone(),m.getBoneCount() ); }
		int setAnimation( math::Matrix3x3 &root,KeyFrame2D *kf,float *time )
		{
			int ret = setMotionMatrix( kf,time );
			setBoneMatrix( root );
			return ret;
		}
		math::Matrix3x3 *getMatrix(){ return &amat[0]; }
	};
}}