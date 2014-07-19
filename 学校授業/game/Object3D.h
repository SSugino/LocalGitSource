/** @file
 * 3Dオブジェクトクラスの定義
 * @date 2009/07/21
 * @author 杉野
 **/

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"
#include "Object2D.h"
#pragma once

namespace sugino{
namespace game{
	class Light3D;			//仮定義
	class DirectionLight3D;
	class PointLight3D;

	typedef DirectionLight3D DirectLight;	//省略
	typedef PointLight3D	 PointLight;

	/** 3D頂点構造体 */
	struct Vertex3D
	{
		/** ローカル座標 */
		math::Vector3 p;
		/** 頂点色 */
		DWORD	c;
		/** トランスフォーム後のグローバル座標時の頂点データ */
		math::Vector3 gp;
		/** テクスチャ座標 */
		math::Vector2 t;
		/** 法線ベクトル */
		math::Vector3 n;		//面の向き（単位ベクトル)
		/** トランスフォーム後の法線ベクトル */
		math::Vector3 gn;
		/** ボーン行列のインデックス */
		WORD blend1;				//頂点がどのボーンを参照するか
		/** ボーン行列のインデックス(親番号) */
		WORD blend2;
		/** ウェイト値 */
		float weight;				//親ボーンの影響をどのくらい受けるか
	};
	//----------------------------------------------------------------------------------------
	/** アニメのモーション構造体 */
	struct Motion3D{
		math::Quaternion rot;
		Motion3D( math::Vector3 &axis,float rot ) : rot(axis,rot) {}
		Motion3D( float a,float b,float c,float d ) : rot(a,b,c,d) {}
	};
	//----------------------------------------------------------------------------------------
	/** キーフレーム構造体 */
	struct KeyFrame3D{
		float			time;			//時間
		Motion3D	 *motion;			//モーションデータ
	};
	//----------------------------------------------------------------------------------------
	/** 3Dボーン(関節)データ構造体 */
	struct Bone3D
	{	
		/** ボーン名 */
		std::string name;

		/** 原点 */
		math::Vector3 local;			//x,y,zの座標がLoadで読み込まれ入る
		/** 親ボーン番号 */
		int parent;
		/** コンストラクタ */
		Bone3D(){}
		/** 初期化
		 * @param [in] Vector3	&v	:初期座標
		 * @param [in] int p		:親ボーンの番号
		 */
		Bone3D( math::Vector3 &v,int p ) : local(v),parent(p){} 
	};
	//-----------------------------------------------------------------------------------------
	/** 3D図形オブジェクト */
	class Object3D
	{
	private:
		/** 頂点 */
		system::AutoArray<Vertex3D>							vertex;
		/** テクスチャ */
		system::AutoArray<graphics::TextureColorTVertex>	tvertex;
		/** 面 */
		system::AutoArray<ModelFace>						face;
		/** オブジェクト名 */
		std::string											name;
	public:
		/** .RDXをXML形式での読み込み、オブジェクトを初期化
		 * @param [in] char *text	: テキストデータz
		 * @return 読み込んだテキストの終了アドレス
		 **/
		char *Load( char* text );

		/** オブジェクト名取得 */
		const char *getName(){ return name.data(); }
		
		/** 面、頂点数のサイズ分メモリ確保
		 * @param [in] int	v_size 	:頂点個数
		 * @param [in] int	f_size  :面個数
		 **/
		void Create( int v_size, int f_size );		//メモリ確保
		
		/** オブジェクトの面描画
		 * @param [in] IRender *r		:画像元　
		 **/
		void Draw( graphics::IRender *r );
		
		/** 座標変換
		 * @param [in] const Matrix4x4 &m1	:行列
		 * @param [in] const Matrix4x4 &m2	:カメラ行列
		 **/
		void Transform( const math::Matrix4x4 &m1,const math::Matrix4x4 &m2 );
		
		/** アニメ用座標変換
		 * @param [in] Matrix4x4 *m	:アニメ配列
		 * @param [in] Matrix4x4 &m2	:カメラ行列
		 **/
		void Transform( math::Matrix4x4 *m,math::Matrix4x4 &m2 );

		/** 背景用変換
	 	 * @param [in] Matrix4x4 &vps	:camera*Projection*screenのカメラ行列
		 **/
		void Transform( math::Matrix4x4 &vps );

		/** プリトランスフォーム
		 *  頂点数分変換された座標を今の座標に変換していく
		 **/
		void PreTransform()
		{ 
			for(int i=0;i<vertex.size();i++)
			{
				vertex[i].p = vertex[i].gp;
				vertex[i].n = vertex[i].gn;
			}
		}
		
		/** 複数ライトのライティング
		 * @param [out] Light3D **lights	:Light3D派生の光源データ配列
		 * @param [in]	int		count		:配列の個数　lightを参照するときの添え字に
		 * @param [out] Color	&amb		:環境色
		 **/
		void Lighting( Light3D **lights,int count,math::Color &amb );
		
		/** ライト一個のライティング
		 * @param [out] Light3D **lights	:Light3D派生の光源データ配列
		 * @param [out] Color	&amb		:環境色
		 **/
		void Lighting( Light3D &_light, math::Color &amb );
		
		//---------------------------------------------------------
		//setter
		//---------------------------------------------------------
		//三角形
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3 )
		{
			Object2D::setFace( f,n1,n2,n3 );		//Object2DのsetFaceを使わせてもらう
		}
		//四角形で初期化
		static void setFace( ModelFace *f,WORD n1,WORD n2,WORD n3,WORD n4 ){
			setFace( f+0,n1,n2,n3 );setFace( f+1,n1,n3,n4 );
		}
		//----------------------------------------------------------
		// getter
		//----------------------------------------------------------
		/** 頂点取得 */
		Vertex3D &getVertex( int i ){ return vertex[i]; }

		/** 頂点"数"取得 */
		int getVertexCount(){ return vertex.size(); }

		/** 面取得 */
		ModelFace &getFace( int i ){ return face[i]; }

		/** 面"数"取得 */
		int getFaceCount(){ return face.size(); }
		
		/** プリライティング */
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
		
		/** トランスフォーム後の色のセット */
		void setTColor( DWORD c )
		{
			for(int i=0;i<vertex.size();i++) tvertex[i].c = c;
		}

		/** 法線ベクトル初期化 */
		void Normalize();

		/** 法線の正規化
		 * 行列にスケーリングを入れたら単位ベクトルが無くなり正確にライティングされないので
		 * 正規化させる。（拡大縮小行うときのみ使用)
		 **/
		void NormalNomalize()
		{
			for(int i=0;i<vertex.size();i++)
			{
				if( vertex[i].gn.Length2()!=0 ) vertex[i].gn.Normalize();
			}
		}

		/** アルファ値による霧の演出
		 * projのPersective最後の引数（見える範囲）に合わせる
		 * @param [in] float fog_s	:開始位置
		 * @param [in] float fog_e	:終了位置
		 **/
		void setFogAlpha( float fog_s,float fog_e );
		
		/** 背景色による霧の演出
		 * projのPersective最後の引数（見える範囲）に合わせる
		 * @param [in] float fog_s	:開始位置
		 * @param [in] float fog_e	:終了位置
		 * @param [in] Color fog_col:背景色
		 **/
		void setFogColor( float fog_s,float fog_e,math::Color &fog_col );

		//-----------------------------------------------------------
		//				基本図形
		//-----------------------------------------------------------
		/** 箱作成 */
		void CreateBox( DWORD col );
		
		/** 板作成
		 * @param [in] int	nx	:横枚数
		 * @param [in] int	nz	:奥行き枚数
		 * @param [in] DWORD col	:色
		 **/
		void CreatePlane( int nx,int nz,DWORD col );
		
		/** 球作成
		 * @param [in] int		nr	:円の分割数
		 * @param [in] Vector2	*v	:回転データ
		 * @param [in] int		nv	:データの個数
		 * @param [in] DWORD		col	:色
		 **/
		void CreateRotate( int nr,math::Vector2 *v,int nv,DWORD col );

		/** 筒作成
		 * @param [in] int	nr		: 円分割
		 * @param [in] int	n		: 縦分割
		 * @param [in] float top		: 上面サイズ
		 * @param [in] float bottom	: 下面サイズ
		 * @param [in] DWPRD	col		: 色
		 **/
		void CreateCylinder( int nr,int n,float top,float bottom,DWORD col );

		/** 球作成
		 * @param [in] int	nr		:円分割
		 * @param [in] int	ny		:縦分割
		 * @param [in] DWORD col		:色
		 **/
		void CreateSphere( int nr,int ny,DWORD col );		
	};

	//---------------------------------------------------------------------------------------
	/** 3Dライト（光源）基底クラス */
	class Light3D
	{
	private:
		math::Color col;			//光の色
	protected:
		/** 明るさ取得
		 * @param [in] Vector3 &up	:トランスフォーム後の頂点座標
		 * @param [in] Vector3 &norm	:法線ベクトル/単位ベクトル(面の向き)
		 **/
		virtual float GetBright( math::Vector3 &vp,math::Vector3 &norm ) = 0;
	public:
		/** コンストラクタ */
		Light3D(){}
		/** デストラクタ */
		virtual ~Light3D(){}
		
		/** 光源色代入初期化
		 * @param [in] Color &_col	:光の色
		 **/
		Light3D( const math::Color &_col ) : col(_col){}
		
		/** 光源色取得 
		 * @param [in] Vector3	&vp		:トランスフォーム後の頂点座標
		 * @param [in] Vector3	&norm	:法線ベクトル/単位ベクトル(面の向き)
		 * @param [in] Color	&dif	:物体色
		 **/
		virtual math::Color GetLightingColor( math::Vector3 &vp,math::Vector3 &norm,math::Color &dif ){
			return dif * getColor() * GetBright( vp,norm );
		}
		
		/** ライトの色取得
		 * @return col	:光源色
		 **/
		math::Color &getColor(){ return col; }
	};

	//---------------------------------------------------------------------------------------
	/** 平行光源 Light3D派生 */
	class DirectionLight3D : public Light3D		//光の方向のみのデータを持つライト　例）一定方向から全体を照らす
	{
	protected:
		math::Vector3	dir;					//ライト座標
		/** 明るさの取得
		 * @param [in]	Vector3 &vp		:トランスフォーム後の頂点座標
		 * @param [in]	Vector3 &norm	:法線ベクトル/単位ベクトル(面の向き)
		 **/
		virtual float GetBright( math::Vector3 &vp,math::Vector3 &norm ){
			float bright = math::Vector3::Dot( norm, -dir );
			if( bright<0 ) bright = 0;
			return bright;
		}
	public:
		/**　コンストラクタ
		 *@param [in] Vector3	_dir	:ライトを灯す場所
		 *@param [in] Color		&col	:灯すライトの色
		 **/
		DirectionLight3D( math::Vector3 _dir, math::Color &col ) :
		  Light3D( col ),dir(_dir) {}

		/** ライト座標取得 */
		math::Vector3 getDir(){ return dir; }
	};

	//----------------------------------------------------------------------------------------
	/** 点光源 Light3D派生 */
	class PointLight3D : public Light3D				//全方向を照らすライト　例）一部分を照らす
	{
	private:
		math::Vector3	pos;			//ライト座標
		float			att;			//光が届く距離
		/** 明るさ取得
		 * @param [in] Vector3 vp		:トランスフォーム後の頂点座標
		 * @param [in] Vector3 norm		:法線ベクトル（単位ベクトル)
		 * @return float bright*len		:今いる座標による明るさ
		 **/
		virtual float GetBright( math::Vector3 &vp, math::Vector3 &norm )
		{
			math::Vector3 dir		= pos - vp;
			float len				= 1 - dir.Length()*att; if(len<0)len=0;
			float bright			= math::Vector3::Dot( norm,dir.Normalize() );
			if( bright<0 ) bright	= 0;
			return bright * len;
		}
	public:
		/**　コンストラクタ
		 *@param [in] Vector3	_dir	:ライトを灯す場所
		 *@param [in] Color		&col	:灯すライトの色
		 *@param [in] float		_att	:光が届く距離(1では少なすぎる）
		 **/
		PointLight3D( math::Vector3 &_pos, math::Color &col, float _att ) :
		  Light3D( col ), pos(_pos), att(1/_att) {}

		  /** 光源取得 */
		  math::Vector3 getPos(){ return pos; }
	};
	
	//--------------------------------------------------------------------------------------
	/** モデル(オブジェクト集合体)データ */
	class Model3D
	{
	private:
		/** 複数オブジェクトを扱う配列 */
		system::AutoArray< Object3D >	objs;
		/** 複数ボーンを扱う配列 */
		system::AutoArray< Bone3D >		bones;
	public:
		/** コンストラクタ */
		Model3D(){};
		
		/** ロード
		 * @param [in] char* file_name	: 読み込むファイル名 RDXファイル
		 **/
		void Load( const char *file_name );

		/** 破棄 */
		void Dispose(){ objs.dispose(); }

		/** オブジェクト数の取得
		 * @return 配列個数
		 **/
		int getObjectCount(){ return objs.size(); }

		/** 指定オブジェクトのアドレス取得 */
		Object3D *getObject(int no){return &objs[no];}

		/** 参照
		 * @param [in] no	:参照したい配列の要素
		 * @return objs[no]	:参照した配列の中身
		 **/
		Object3D &operator[] (int no){ return objs[no]; }

		/** 名前から参照
		 * @param[in] const char* model_name	:モデル名
		 * @return	objs[i]						:モデルの配列
		 **/
		Object3D &operator[] ( const char* model_name )
		{
			for( int i=0;i<objs.size();i++ )
			{
				if( objs[i].getName() == model_name ) return objs[i];	//名前が一致したらその配列の中身を返す
			}
			throw system::Exception( "存在しないモデルを参照しました(モデル名確認)" );
		}
		
		/**	指定した配列番号から取得 */
		Bone3D &getBoneArray(int no){ return bones[no]; };
		
		/** ボーン配列取得 */
		inline Bone3D *getBone(){ return &bones[0]; }

		/** ボーン数取得 */
		inline int getBoneCount() { return bones.size(); };

		/** 指定した名前から取得 */
		Bone3D &getBoneName( const char *name )
		{
			for( int i=0;i<bones.size();i++ )
			{
				if( bones[i].name == name ) return bones[i];	//名前が一致したらその配列を返す
			}
			throw system::Exception( "存在しないボーンを参照しました(ボーン名確認)" );
		};
	};
	
	//---------------------------------------------------------------------------------------
	/** １つのモデルで複数のアニメーションが出来るクラス */
	class ModelAnimation3D{
	private:
		Bone3D			*bone;						//回転の原点データ配列
		system::AutoArray< math::Matrix4x4 > mat;	//アニメーション行列の配列
	public:
		void Init( Bone3D *_bone,int size ){
			bone = _bone;
			mat.resize( size );
		}
		/** アニメーションのモーションをセット */
		int setMotion( math::Matrix4x4 &root,KeyFrame3D *kf,float *time );

		math::Matrix4x4 *getMatrix(){ return &mat[0]; }
	};
}}