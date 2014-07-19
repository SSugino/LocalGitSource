/** @file
 * 描画関数インターフェースクラスの定義
 * @date 2009/05/13
 * @author 杉野
 **/

#pragma once

#include "../system/Window.h"
#include "../math/Vector.h"

namespace sugino{
namespace graphics{
		//-------------------------------------------------------------------------
		//Virtex Formats
		//-------------------------------------------------------------------------
		/**　点・色の指定 */
		struct ColorTVertex
		{
			float x,y,z,w;
			DWORD c;
			ColorTVertex(){};
			ColorTVertex(float _x,float _y,DWORD _c) :
				x( _x),y( _y),z(0),w(1),
					c( _c) {};
			/** 2Dベクトル用 */
			ColorTVertex( const math::Vector2 &v,DWORD _c) :
			x(v.x),y(v.y),z(0),w(1),
				c( _c) {};
			/** 3Dベクトル用 */
			ColorTVertex( const math::Vector3 &v,DWORD _c) :
			x(v.x),y(v.y),z(v.z),w(1),
				c( _c) {};
			/** 4次元ベクトル用 */
			ColorTVertex( const math::Vector4 &v,DWORD _c) :
			x(v.x),y(v.y),z(v.z),w(v.w),
				c( _c) {};


		};
		/** テクスチャ座標のみの点　*/
		struct TextureTVertex
		{
			float x,y,z,w;
			float tx,ty;
			TextureTVertex() {};
			TextureTVertex( float _x,float _y,float _tx,float _ty ) :
				x( _x),y( _y),z(0),w(1),
				tx( _tx),ty( _ty) {};
			/** 2Dベクトル用 */
			TextureTVertex( const math::Vector2 &v,float _tx,float _ty ) :
				x( v.x),y( v.y),z(0),w(1),
				tx( _tx),ty( _ty) {};
			
			/** 3Dベクトル用 */
			TextureTVertex( const math::Vector3 &v,float _tx,float _ty,float _tz ) :
				x(v.x),y( v.y ),z(v.z),w(1),
				tx( _tx),ty( _ty) {};

			/** 4次元ベクトル用 */
			TextureTVertex( const math::Vector4 &v,float _tx,float _ty,float _tz ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( _tx),ty( _ty) {};

		};
		/** 座標と色とテクスチャ座標を持つ点 */
		struct TextureColorTVertex
		{
			float x,y,z,w;
			DWORD c;
			float tx,ty;
			TextureColorTVertex() {};
			TextureColorTVertex( float _x,float _y,float _tx,float _ty,DWORD _c ) :
				x( _x),y( _y),z(0),w(1),
				tx( _tx),ty( _ty),
				c( _c) {};
			/** 2Dベクトル用 */
			TextureColorTVertex( const math::Vector2 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(0),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			/** 3Dベクトル用 */
			TextureColorTVertex( const math::Vector3 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			/** 3Dベクトル用 */
			TextureColorTVertex( const math::Vector3 &v,const math::Vector3 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			
			/** 四次元ベクトル用 */
			TextureColorTVertex( const math::Vector4 &v,const math::Vector3 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( v2.x),ty( v2.y),
				c( _c) {};

			/** 四次元ベクトル用 */
			TextureColorTVertex( const math::Vector4 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( v2.x),ty( v2.y),
				c( _c) {};

		};

		/** レンダリング可能テクスチャの破棄、復帰イベントクラス定義 */
		class IRestoreEvent
		{
		public:
			virtual ~IRestoreEvent(){}
			/** 破棄の純粋仮想関数 */
			virtual void Dispose()=0;
			/** 復旧の純粋仮想関数 */
			virtual void Restore()=0;
		};

		/**　画像（テクスチャ）を扱うインターフェース */
		class IImage{
		public:
			virtual ~IImage(){};
			virtual int getWidth() = 0;
			virtual int getHeight() = 0;
			virtual int getID() = 0;
		};

		/** レンダリングインターフェース */
		class IRender{
		public:
			virtual ~IRender(){};
			virtual void Initialize( system::Window* win,bool full ) = 0;			//初期化
			virtual void setViewport( const system::Rect &vp ) = 0;				//描画範囲設定
			virtual void getViewport( system::Rect *r )=0;							//描画範囲取得
			virtual void Clear( DWORD col ) = 0;									//画面のクリア
			virtual void Update( system::Window* win,const system::Rect &r ) = 0;	//画面更新
			virtual bool beginScene() = 0;											//描画開始
			virtual void endScene() = 0;											//描画終了
			virtual void drawPixel( int x,int y,DWORD col ) = 0;					//点を描画
			virtual IImage *createImage( const char *name ) = 0;					//画像の読み込み
			virtual IImage *createImage( int w,int h ) = 0;							//仮想画面画像の読み込み
			virtual void setTexture( IImage *src ) = 0;								//画像の設定
			virtual void setRenderTarget( IImage *tex ) = 0;						//RenderTargetの切り替え
			virtual void setRestoreEvent( IRestoreEvent *e )=0;						//メイン側で復旧処理させる
			virtual void getTargetImage( IImage* tex )=0;							//今現在の描画画面を取得
			virtual void ClearZ() = 0;												//Zバッファのクリア
			/** 線の描画仮想関数
			 * @param [in] ColorTVertex v		:　色頂点
			 * @param [in] WORD			index	:　頂点インデックス
			 * @param [in] int			vn		:　頂点数
			 * @param [in] int			n		:　直線数
			 **/
			virtual void drawLine( ColorTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawLine( TextureTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawLine( TextureColorTVertex *v,WORD *index,int vn,int n ) =0;

			virtual void drawPolygon( ColorTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawPolygon( TextureTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawPolygon( TextureColorTVertex *v,WORD *index,int vn,int n ) =0;
			/** 文字列の描画
			 * @param [in] x,y :　座標
			 * @param [in] str :　文字列
			 * @param [in] col :　描画色(0xAARRGGBB)
			 **/
			virtual void drawString( int x,int y,const char *str,DWORD col )  = 0;
			
			virtual void drawString( int x,int y,const char *str,float value,DWORD col )  = 0;
			/** 文字列描画用のフォントの指定 */
			virtual void setFont( const system::Font &font ) = 0;		
			
			/** アルファモード */
			enum ALPHA_MODE{
				ALM_NONE  = 0,	//アルファなし
				ALM_ALPHA = 1,	//アルファブレンド
				ALM_ADD	  =	2	//加算合成
			};
			/** アルファモードのセット */
			virtual ALPHA_MODE setAlphaBlend( ALPHA_MODE alm ) = 0;

			/** カリングモード（面の描画設定） */
			enum CULLING_MODE{
				CM_NON	= 0,	//両面描画
				CM_CCW	= 1,	//「表」面描画( Counter Clock Wise ) 反時計周り
				CM_CW	= -1	//「裏」面描画( Clock Wise ) 時計回り
			};
			/** カリングモードの設定 */
			virtual CULLING_MODE setCullMode( CULLING_MODE culm ) =	0;
			
			/** Zバッファモード(奥行き比較) */
			enum ZBUFFER_MODE{
				ZM_OFF			= 0,	//比較しない
				ZM_ON			= 1,	//比較する
				ZM_OFF_WRITE	= 2,	//比較せずに、更新
				ZM_ON_WRITE		= 3		//比較し更新
			};
			
			/** Zバッファモードの設定 */
			virtual ZBUFFER_MODE setZBufferMode( ZBUFFER_MODE zm ) = 0;
			
			/** シェーディング（面描画）モード */
			enum SHADE_MODE{
				SM_WIRE	= 0,			//ワイヤーフレーム
				SM_FLAT = 1,			//カクカク感
				SM_GOURAUD= 2			//滑らか
			};
			
			/** シェーディングモードの設定 */
			virtual SHADE_MODE setShedingMode( SHADE_MODE sm ) = 0;

			/** テクスチャフィルタ設定 */
			enum FILTER_MODE{
				FM_NONE		= 0,	// フィルタなし
				FM_POINT	= 1,	// 点フィルタ
				FM_LINEAR	= 2		// リニアフィルタ
			};
			/** テクスチャ縮小フィルタ */
			virtual FILTER_MODE setTextureMinFilterMode( FILTER_MODE min ) = 0;
			/** テクスチャ拡大フィルタ */
			virtual FILTER_MODE setTextureMaxFilterMode( FILTER_MODE max ) = 0;

			/** テクスチャボーダーモード */
			enum BORDER_MODE
			{
				BM_WRAP		= 1,	// 円筒
				BM_MIRROR	= 2,	// 鏡
				BM_CLAMP	= 3		// 引廷
			};

			/** テクスチャボーダーモードの設定 */
			virtual BORDER_MODE setTextureBorderMode( BORDER_MODE mode ) = 0;
			//-----------------------------------------------------------------------
			//		描画関数
			//-----------------------------------------------------------------------
			/** 線描画
			 * @param [in]	int		x1		:2次元座標始点x
			 * @param [in]	int		y1		:2次元座標始点y
			 * @param [in]	int		x2		:2次元座標終点x
			 * @param [in]	int		y2		:2次元座標終点y
			 * @param [in]	DWORD	col1	:始点の色
			 * @param [in]	DWORD	col2	:終点の色
			 **/
			void drawLine( int x1,int y1,int x2,int y2,DWORD col1,DWORD col2 );

			/** 塗りつぶし四角描画
			 * @param [in]	Rect	&r		:2次元座標のxy
			 * @param [in]	DWORD	col		:色
			 **/
			void drawFill( const system::Rect &r,DWORD col );//塗り潰し四角

			/** 四角
			 * @param [in]	Rect	&r		:2次元座標のxy
			 * @param [in]	DWORD	col		:色
			 **/
			void drawBox( const system::Rect &r,DWORD col );//枠
			
			/** Draw関数
			 * @param [out] Rect(x1,y1,x2(幅),y2(高さ) )	:　表示先
			 * @param [in]　img								:出したい画像
			 * @param [out] Rect(x1,y1,x2(幅),y2(高さ) )	:　表示元
			 **/
			void drawImage( const system::Rect &dest,IImage *img, const system::Rect &src );//画像描画
			
			/** 呼び出しやすい形のDraw関数
			 * @param [in]	int x		:表示先座標
			 * @param [in]	int y		:
			 * @param [in]	int w		:表示先幅
			 * @param [in]	int h		:表示先高さ
			 * @param [in]	IImage *img	:表示したい画像
			 * @param [in]	int sx		:表示元座標(幅分(w)取得してくれる)
			 * @param [in]	int sy		:
			 * @note	テクスチャサイズは2n乗にすること
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy )
			{
					drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sx+w,sy+h ) );//呼び出しやすい形の関数
			}
			
			/** StretchDraw関数
			 * @param [in]	int		x	:表示先座標
			 * @param [in]	int		y	:
			 * @param [in]	int		w	:表示先幅
			 * @param [in]	int		h	:表示先高さ
			 * @param [in]	IImage *img :表示元画像
			 * @param [in]	int		sx	:表示元座標(幅分(w)取得してくれる)
			 * @param [in]	int		sw	:拡大縮小幅
			 * @param [in]  int		sh	:拡大縮小高さ
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy,int sw,int sh )
			{
				drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sw/(sx+w),sy/(sh+h) ) );//呼び出しやすい形の関数
			}
			
			/** アルファ値付き
			 * 
			 **/
			void drawImage( const system::Rect &r,IImage *img, const system::Rect &src,DWORD col );
		
			/** アルファ値付き
			 * 使いやすい形に
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy,DWORD col )
			{
				drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sx+w,sy+h ),col );
			}
		};
	}
}