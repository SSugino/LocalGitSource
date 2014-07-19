/** @file
 * Direct3D描画関数インターフェースクラスの定義
 * @date 2009/05/13
 * @author 杉野
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../system/Window.h"
#include <d3dx9.h>

namespace sugino{
/** グラフィックの領域 */
namespace graphics{
		/**　レンタリングクラス : IRenderからの派生
		 * 　Direct3Dを使ったレンタリングを行う
		 **/
		class D3D9Render : public IRender
		{
		private:
			//テクスチャの塗りつぶし関数が使う関数タイプ
			LPDIRECT3D9				lp3d;
			//レンダリングの実行、リソースの作成、システムレベル変数の処理、
			//ガンマ曲線レベルの調整、パレットの処理、シェーダの作成を行う
			LPDIRECT3DDEVICE9		lpdev;
			//画面モードや最終的な画像転送方法などの設定をする
			D3DPRESENT_PARAMETERS	d3dpp;
			//サーフェイスフォーマット列挙体
			D3DFORMAT				format;
			//Font用
			LPD3DXFONT				dxfont;
			LPD3DXSPRITE			sp;
			
			//RenderTaget保存用
			LPDIRECT3DSURFACE9		back;		//バックバッファ
			LPDIRECT3DSURFACE9		zbuf;		//Zバッファ
	
			IRestoreEvent			*restore;	//デバイスロスト時の復旧

			/** 初期設定を行う */
			void setDefaultState();
		public:
			D3D9Render();

			virtual ~D3D9Render(){ Uninitialize(); } ;
			virtual void Initialize( system::Window* w,bool full );
			virtual void setViewport( const system::Rect &vp );
			virtual void Clear( DWORD col );
			virtual void ClearZ();
			virtual bool beginScene();
			virtual void endScene();
			virtual void Update( system::Window* w,const system::Rect &r );
			virtual void drawPixel( int x,int y,DWORD col );

			virtual void setRestoreEvent( IRestoreEvent *e ){ restore=e; }

			IImage *createImage( const char *name );//IRender派生読み込み関数
			void setTexture( IImage *tex );//IRender派生設定

			/** 仮想画面の作成
			 * @param [in] int w		:画面幅
			 * @param [in] int h		:画面高さ
			 * @return	autoptrで用意されたIImageの型
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 **/
			IImage *createImage( int w,int h );			//RenderTexture(仮想画面)の作成
			
			/** 仮想画面のセット
			 * @param [in] tex			:createで作られた画像変数
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 * @note render->setTexture( display );
			 **/
			void setRenderTarget( IImage *tex );		//RenderTarget切り替え

			/** 描画画面の取得
			 * @param [in] tex			:createで作られた画像変数
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 * @note render->getTargetImage( display );
			 **/ 
			void getTargetImage ( IImage *tex );		//今現在、描画されている画面の取得

			/** アルファモードの設定 */
			ALPHA_MODE setAlphaBlend( ALPHA_MODE alm );
			
			/** カリングモードの設定 */
			CULLING_MODE setCullMode( CULLING_MODE culm );
			
			/** Zバッファモードの設定 */
			ZBUFFER_MODE setZBufferMode( ZBUFFER_MODE zm );
			
			/** シェーディングモードの設定 */
			SHADE_MODE setShedingMode( SHADE_MODE sm );
			
			/** テクスチャ縮小フィルタ */
			FILTER_MODE setTextureMinFilterMode( FILTER_MODE min );
			
			/** テクスチャ拡大フィルタ */
			FILTER_MODE setTextureMaxFilterMode( FILTER_MODE max );
			
			/** テクスチャボーダーモード */
			BORDER_MODE setTextureBorderMode( BORDER_MODE mode );


			//
			void setFont( const system::Font &font );
			void drawString( int x,int y,const char*str,DWORD col );

			/** float数値表示可能文字列表示
			 * 引数const charに %fを入れる事
			 */
			void drawString( int x,int y,const char *str,float value,DWORD col );

			void drawLine( ColorTVertex *v,WORD *index,int vn,int n );
			void drawLine( TextureTVertex *v,WORD *index,int vn,int n );
			void drawLine( TextureColorTVertex *v,WORD *index,int vn,int n );

			void drawPolygon( ColorTVertex *v,WORD *index,int vn,int n );
			void drawPolygon( TextureTVertex *v,WORD *index,int vn,int n );
			void drawPolygon( TextureColorTVertex *v,WORD *index,int vn,int n );
			//
			LPDIRECT3DDEVICE9 getDevice(){ return lpdev; }
	
			void Uninitialize();

			/** Renderから描画範囲の取得
			 * @param [out] Rect *vp	:描画範囲
			 */
			void getViewport( system::Rect *r );
			
		};
	}
}