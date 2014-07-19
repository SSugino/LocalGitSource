/** @file
 * Direct3D描画関数インターフェースクラスの実装
 * @date 2009/05/13
 * @author 杉野
 **/
#include "D3D9Render.h"
#include "../system/Exception.h"

#pragma comment( lib,"d3d9.lib" )
#pragma comment( lib,"d3dx9.lib" )
#pragma comment( lib,"dxguid.lib" )

using namespace sugino;
using namespace graphics;
using namespace system;

/** Direct3D Texture Image 
 *  IImageからの派生
 **/
class D3D9Texture : public IImage
{
private:
	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DSURFACE9 surface;
	D3DSURFACE_DESC	   desc;
public:
	enum{
		/** D3D9textureのID */
		ID = 9999

	};

	virtual ~D3D9Texture(){ Dispose(); }
	virtual int getWidth(){ return desc.Width; }
	virtual int getHeight(){ return desc.Height; }
	virtual int getID(){ return ID; }


	//--------------------------------------------------------------
	void Dispose()
	{
		if( surface ){ surface->Release(); surface=NULL; }
		if( tex ){ tex->Release(); tex=NULL; }
	}
	/** コンストラクタ */
	D3D9Texture( LPDIRECT3DTEXTURE9 _tex )
	{
		tex = _tex;
		tex->GetSurfaceLevel( 0,&surface );
		surface->GetDesc( &desc );
	}
	/** テクスチャ取得 */
	inline LPDIRECT3DTEXTURE9 getTexture(){ return tex; }

	/** サーフェイス取得 */
	inline LPDIRECT3DSURFACE9 getSurface(){ return surface; }
};

/** Direct3DRenderTexture
 *  D3D9Textureからの派生
 *  仮想画面的なもの
 **/
class D3D9RenderTexture : public D3D9Texture
{
private:
	LPDIRECT3DSURFACE9	zbuf;
public:
	enum{
		ID = 9998
	};
	/** 初期化 */
	D3D9RenderTexture( LPDIRECT3DTEXTURE9 tex ) : D3D9Texture( tex ){ 
		zbuf = NULL;
	}
	
	virtual ~D3D9RenderTexture(){ Dispose(); }
	
	virtual int getID(){ return ID; }
	/** 解放 */
	void Dispose() { if(zbuf) zbuf->Release();
						zbuf=NULL; }
	/** Ｚバッファ取得 */
	LPDIRECT3DSURFACE9 getZbuffer(){ return zbuf; }
	/** Ｚバッファセット */
	void setZbuffer( LPDIRECT3DSURFACE9 z ){ zbuf = z; }
};


/** 初期化
 * @param[in] win:表示先
 * @param[in] full:スクリーンモード
 **/
void D3D9Render::Initialize( Window* win,bool full )
{
	Uninitialize();
	int w,h;
	win->getSize( &w,&h );
	lp3d = Direct3DCreate9( D3D_SDK_VERSION );
	if( lp3d==NULL ) throw Exception( "D3Dの作成に失敗" );

	D3DDISPLAYMODE mode;
	//グラボでのディスプレイへの描画方法の取得
	lp3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT,&mode );
	format = mode.Format;			//現在の画面モード

	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = !full;
	d3dpp.BackBufferFormat = format;
	d3dpp.BackBufferWidth = w;
	d3dpp.BackBufferHeight = h;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.hDeviceWindow = win->getHwnd();
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	int ret;
	//ディスプレイへのアダプタ（グラボ）を表すためのデバイスを作成
	//ハードウェアに任せる（頂点処理させる）

	//ret = lp3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
	//						  d3dpp.hDeviceWindow,
	//						  D3DCREATE_HARDWARE_VERTEXPROCESSING,
	//						  &d3dpp,&lpdev );
	
	///ハードウェア処理でなければ,
	//ソフトウェアでの頂点処理にする
	//if( ret!=D3D_OK ){
		ret = lp3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
								 d3dpp.hDeviceWindow,
								 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								 &d3dpp,&lpdev );
		//これでもデバイス作成できなければ
		if( ret!=D3D_OK ) throw Exception( "D3Dデバイス作成失敗" );
	//}

	// Font Sprite Create
	setFont( Font( 12,"MS GOTHIC") );//高さ12のMSゴシック
	D3DXCreateSprite( lpdev,&sp );
	if( sp==NULL ) throw Exception( "Font用スプライト作成失敗" );

	Clear(0);					//targetをクリアしておく
	setAlphaBlend( ALM_ALPHA );//アルファブレンドで指定
	
	lpdev->GetRenderTarget(0,&back);
	lpdev->GetDepthStencilSurface(&zbuf);

	setDefaultState();
}

//コンストラクタ
D3D9Render::D3D9Render()
{
	back=NULL;
	zbuf=NULL;

	lp3d = NULL;
	lpdev = NULL;
	//Font用
	dxfont = NULL;
	sp = NULL;
}

//片付け
void D3D9Render::Uninitialize()
{
	if(back){back->Release();back=NULL;}
	if(zbuf){zbuf->Release();zbuf=NULL;}
	if( sp ){ sp->Release();sp=NULL; }
	if( dxfont ){ dxfont->Release();dxfont=NULL; }
	if( lpdev ){ lpdev->Release();	lpdev=NULL; }
	if( lp3d ){ lp3d->Release(); lp3d=NULL; }
}
/* ビューポート設定
 * @param [in] const Rect &r :範囲
 */
void D3D9Render::setViewport( const system::Rect &r )
{
	//クリップ範囲設定
	D3DVIEWPORT9 view = {r.left, r.top, r.right-r.left, r.bottom-r.top,0,1};
	lpdev->SetViewport( &view );
}
/** 画面更新
 * @param[in] win:表示先
 * @param[in] &r :クリップ範囲
 **/
void D3D9Render::Update( Window* win,const Rect &r )
{
	//デバイスが所有するバックバッファのシーケンスの中で、次のバッファのコンテンツを送る(画像転送)
	// 表示　→ Update(更新) → 表示 → Update(更新) →･･･
	int ret = lpdev->Present( &r,NULL,win->getHwnd(),NULL );
	if( ret==D3DERR_DEVICELOST )
	{
		
		if(restore)restore->Dispose();
		//zバッファとバックバッファ再取得
		if(zbuf){zbuf->Release();zbuf=NULL;}
		if(back){back->Release();back=NULL;}

		//ロスト(デバイスを失った)処理
		dxfont->OnLostDevice();
		sp->OnLostDevice();

		if( lpdev->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
		{
			D3DDISPLAYMODE mode;
			lp3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT,&mode );
			d3dpp.BackBufferFormat = mode.Format;
			if( lpdev->Reset( &d3dpp ) == D3D_OK )
			{
				setDefaultState();
				dxfont->OnResetDevice();
				sp->OnResetDevice();
				lpdev->GetRenderTarget(0,&back);
				lpdev->GetDepthStencilSurface(&zbuf);
				if(restore)restore->Restore();
			}else{
				throw Exception( "ロスとデバイスの復旧に失敗しました" );
			}
		}
	}
}
//画面のクリア
void D3D9Render::Clear(DWORD col)
{
	//lpdev->Clear( 0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,col,1.0f,0 );//バッファを何色で塗るか(色は0xAARRGGBB)
	lpdev->Clear( 0,NULL,D3DCLEAR_TARGET,col,1.0f,0 );//バッファを何色で塗るか(色は0xAARRGGBB)
}

//Zバッファクリア
void D3D9Render::ClearZ()
{
	lpdev->Clear( 0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0 );
}


//描画開始
bool D3D9Render::beginScene()
{
	return lpdev->BeginScene() == D3D_OK;//Lock 画像描画前に記述
}
//描画終了
void D3D9Render::endScene()
{
	lpdev->EndScene();//Unlock　描画終えたら記述
}

///画像読み込み
IImage *D3D9Render::createImage( const char *name )
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	D3DXCreateTextureFromFile( lpdev,name,&tex );//読み込み 拡張子は(bmp/png/jpeg...)
	if(tex==NULL)//画像なし
	{
		std::string str="画像の読み込み失敗\n"+std::string( name );
		throw Exception( str.data() );
	}
	return new_s D3D9Texture( tex );
}

//画像のセット
void D3D9Render::setTexture( IImage *tex )
{
	if( tex==NULL ){ lpdev->SetTexture( 0,NULL ); return; }
//	if( tex->getID() != D3D9Texture::ID )
//		throw Exception( "D3DRender::setTexture()\n"
//						 "テクスチャ以外のImageが指定されました");// ""  "" 複数行分ける時""は二回使える
	D3D9Texture *t = ( D3D9Texture *)tex;		//down cast
	lpdev->SetTexture( 0,t->getTexture() );
}

//RenderTarget切り替え
void D3D9Render::setRenderTarget( IImage *tex )
{
	//元に戻す
	if( tex==NULL ){
		lpdev->SetRenderTarget( 0,back );
		lpdev->SetDepthStencilSurface( zbuf );
		return;
	}
	
	if( tex->getID() != D3D9RenderTexture::ID ) throw system::Exception( "RenderTextureのIDが一致しません" );

	D3D9RenderTexture *t=	(D3D9RenderTexture*) tex;	//ダウンキャスト
	lpdev->SetRenderTarget( 0,t->getSurface() );
	lpdev->SetDepthStencilSurface( t->getZbuffer() );
}

//フォントの設定
void D3D9Render::setFont( const Font &font )
{
	if( dxfont ){ dxfont->Release(); dxfont=NULL; }
	LOGFONT log;
	GetObject( font,sizeof(LOGFONT),&log );
	D3DXFONT_DESCA fdesc ={
		log.lfHeight,log.lfWidth,log.lfWeight,0,log.lfItalic,log.lfCharSet,
		log.lfOutPrecision,log.lfQuality,log.lfPitchAndFamily
	};
	strcpy( fdesc.FaceName,log.lfFaceName );
	D3DXCreateFontIndirect( lpdev,&fdesc,&dxfont );
	if( dxfont==NULL ){ throw Exception( "Font作成に失敗" ); }
}
//描画表示
void D3D9Render::drawString( int x,int y,const char *str,DWORD col )
{
	sp->Begin( D3DXSPRITE_ALPHABLEND );
	RECT r={ x,y,0,0 };
	dxfont->DrawText( sp,str,-1,&r,DT_LEFT | DT_CALCRECT, col );//範囲計算
	dxfont->DrawText( sp,str,-1,&r,DT_LEFT, col );//描画

	sp->End();
}	


void D3D9Render::drawString( int x,int y,const char *str,float value,DWORD col )
{
	char buf[256];
	sprintf( buf,str,value);
	drawString(x,y,buf,col );					//FPS表示
}

//点の描画
void D3D9Render::drawPixel( int x, int y, DWORD col )
{
	D3DRECT r = { x,y,x+1,y+1 };
	lpdev->Clear( 1,&r,D3DCLEAR_TARGET,col,0,0 );
}
//ラインの描画
//色頂点Ver( IRender.h参照 )
void D3D9Render::drawLine( ColorTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(ColorTVertex) );
}
//テクスチャ座標のみの頂点( IRender.h参照 )
void D3D9Render::drawLine( TextureTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureTVertex) );
}
//座標と色とテクスチャ座標を持つ頂点( IRender.h参照 )
void D3D9Render::drawLine( TextureColorTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 |D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureColorTVertex) );
}

//ポリゴン描画
//色頂点Ver( IRender.h参照 )
void D3D9Render::drawPolygon( ColorTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(ColorTVertex) );
}
//ポリゴン描画
//テクスチャ座標のみの頂点( IRender.h参照 )
void D3D9Render::drawPolygon( TextureTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureTVertex) );
}

//座標と色とテクスチャ座標を持つ頂点( IRender.h参照 )
void D3D9Render::drawPolygon( TextureColorTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureColorTVertex) );
}
/** アルファモード変更
 * @param [in] ALPHA_MODE alm :モード(列挙体)
 *							  :0:ALM_NONE  アルファなし
 *							  :1:ALM_ALPHA ブレンド
 *							  :2:ALM_ADD   加算
 **/
IRender::ALPHA_MODE D3D9Render::setAlphaBlend( IRender::ALPHA_MODE alm )
{
	DWORD ae,db;
	
	lpdev->GetRenderState( D3DRS_ALPHABLENDENABLE, &ae );
	lpdev->GetRenderState( D3DRS_DESTBLEND, &db );
	
	switch( alm )
	{
	case ALM_NONE:
			lpdev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
			break;
	case ALM_ALPHA:
			lpdev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			lpdev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			lpdev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA  );
			break;
	case ALM_ADD:
			lpdev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
			lpdev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			lpdev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE  );
			break;		
	}
	if( ae==FALSE ) return ALM_NONE;
	if( db==D3DBLEND_ONE ) return ALM_ADD;
	return ALM_ALPHA;
}

/** カリングモード変更
 * @param [in] CULLING_MODE culm :モード(列挙体)
 *								 :0:	CM_NON	両面描画
 *								 :1:	CM_CCW	「表」面描画 反時計周り
 *								 :-1:	CM_CW「裏」面描画 時計回り
 **/
IRender::CULLING_MODE D3D9Render::setCullMode( IRender::CULLING_MODE culm )
{
	DWORD s;
	lpdev->GetRenderState( D3DRS_CULLMODE,&s );

	switch( culm )
	{
	case CM_CCW:
		lpdev->SetRenderState( D3DRS_CULLMODE,D3DCULL_CCW );	break;
	case CM_CW:
		lpdev->SetRenderState( D3DRS_CULLMODE,D3DCULL_CW );		break;
	case CM_NON:
		lpdev->SetRenderState( D3DRS_CULLMODE,D3DCULL_NONE );	break;
	}

	switch( s )
	{
	case D3DCULL_CCW:	return CM_CCW;
	case D3DCULL_CW:	return CM_CW;
	case D3DCULL_NONE:	
	default:			return CM_NON;
	}
}


/** Zバッファモード変更
 * @param [in] ZBUFFER_MODE zm :モード(列挙体)
 *							   :0:	ZM_OFF			比較しない
 *							   :1:	ZM_ON			比較する
 *							   :2:	ZM_OFF_WRITE	比較せず更新
 *							   :3:	ZM_ON_WRITE		比較して更新
 **/
IRender::ZBUFFER_MODE D3D9Render::setZBufferMode( IRender::ZBUFFER_MODE zm )
{
	DWORD ze,zw;
	lpdev->GetRenderState( D3DRS_ZENABLE, &ze );
	lpdev->GetRenderState( D3DRS_ZENABLE, &zw );

	lpdev->SetRenderState( D3DRS_ZENABLE, (zm&ZM_ON)!=0 );
	lpdev->SetRenderState( D3DRS_ZWRITEENABLE, (zm&ZM_OFF_WRITE)!=0 );	

	int ret = 0;
	if(ze) ret|=ZM_ON;			//or 1
	if(zw) ret|=ZM_OFF_WRITE;	//or 2
	return (ZBUFFER_MODE) ret;
}
/** シェーディングモードの設定
 * @param [in] SHADE_MODE sm	:モード(列挙体)
 * 
 *
 **/
IRender::SHADE_MODE D3D9Render::setShedingMode( IRender::SHADE_MODE sm )
{
	DWORD fmode,smode;
	
	lpdev->GetRenderState( D3DRS_FILLMODE,&fmode );
	lpdev->GetRenderState( D3DRS_SHADEMODE,&smode );

	if( sm==SM_WIRE ){
		lpdev->SetRenderState( D3DRS_FILLMODE,D3DFILL_WIREFRAME );
		lpdev->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
	}else if( sm == SM_FLAT ){
		lpdev->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );
		lpdev->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_FLAT );
	}else if( sm==SM_GOURAUD ){
		lpdev->SetRenderState( D3DRS_FILLMODE,D3DFILL_SOLID );
		lpdev->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
	}
	if( fmode == D3DFILL_WIREFRAME ) return SM_WIRE;
	if( smode == D3DSHADE_FLAT ) return SM_FLAT;

	return SM_GOURAUD;
}

/** シェーディングモードの設定
 * @param [in] FILTER_MODE min	:モード(列挙体)
 *							    :0:		FM_NONE			フィルタなし
 *								:1:		FM_POINT		点フィルタ
 *								:2:		FM_LINEAR		リニアフィルタ
 **/
IRender::FILTER_MODE D3D9Render::setTextureMinFilterMode( IRender::FILTER_MODE min )
{
	DWORD val;
	lpdev->GetSamplerState( 0,D3DSAMP_MINFILTER,&val );
	lpdev->SetSamplerState( 0,D3DSAMP_MINFILTER,min );
	if( val==D3DTEXF_POINT ) return FM_POINT;
	if( val==D3DTEXF_LINEAR ) return FM_LINEAR;
	return FM_NONE;
}
/** シェーディングモードの設定
 * @param [in] FILTER_MODE min	:モード(列挙体)
 *							    :0:		FM_NONE			フィルタなし
 *								:1:		FM_POINT		点フィルタ
 *								:2:		FM_LINEAR		リニアフィルタ
 **/
IRender::FILTER_MODE D3D9Render::setTextureMaxFilterMode( IRender::FILTER_MODE max )
{
	DWORD val;
	lpdev->GetSamplerState( 0,D3DSAMP_MAGFILTER,&val );
	lpdev->SetSamplerState( 0,D3DSAMP_MAGFILTER,max );
	if( val==D3DTEXF_POINT ) return FM_POINT;
	if( val==D3DTEXF_LINEAR ) return FM_LINEAR;
	return FM_NONE;
}
/** ボーダーモードの設定
 * @param [in] BOORDER_MODE mode:モード(列挙体)
 *							    :1:		BM_WRAP			円筒
 *								:2:		BM_MIRROR		鏡
 *								:3:		BM_CLAMP		引延
 * @return	設定前の状態
 **/
IRender::BORDER_MODE D3D9Render::setTextureBorderMode( IRender::BORDER_MODE mode )
{
	DWORD ret;
	lpdev->GetSamplerState( 0,D3DSAMP_ADDRESSU,&ret );
	lpdev->SetSamplerState( 0,D3DSAMP_ADDRESSU,mode );
	lpdev->SetSamplerState( 0,D3DSAMP_ADDRESSV,mode );

	if( ret==D3DTADDRESS_WRAP ) return BM_WRAP;
	if( ret==D3DTADDRESS_MIRROR ) return BM_MIRROR;
	return BM_CLAMP;
}

void D3D9Render::setDefaultState()
{
	Clear(0);
	setAlphaBlend( ALM_ALPHA );
	this->setCullMode( IRender::CM_CCW );
	this->setZBufferMode( IRender::ZM_ON_WRITE );

	//テクスチャアルファと頂点アルファを乗算するように設定
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAOP,D3DTOP_MODULATE );
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE );
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE );

	/*
	lpdev->SetRenderState( D3DRS_DITHERENABLE,TRUE );			//FALSEのほうがいいときもある
	lpdev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE,TRUE );
	*/
}

void D3D9Render::getViewport( Rect *r )
{
	D3DVIEWPORT9 vp;
	lpdev->GetViewport(&vp);
	SetRect( r,vp.X,vp.Y,vp.X+vp.Width,vp.Y+vp.Height );
}

void D3D9Render::getTargetImage( IImage *tex )
{
	if( tex->getID() != D3D9RenderTexture::ID ) return;		//IDが不一致ならリターン
	D3D9Texture *t = ( D3D9Texture *) tex;
	LPDIRECT3DSURFACE9 src = NULL;							//一度解放して
	lpdev->GetRenderTarget( 0,&src );						//描画先の取得
	lpdev->StretchRect( src,NULL,t->getSurface(),NULL,D3DTEXF_POINT );

	if(src){
		src->Release();										//解放
		src=NULL;											
	}
}

IImage *D3D9Render::createImage( int w,int h )
{
	LPDIRECT3DTEXTURE9 tex=NULL;
	D3DXCreateTexture( lpdev,w,h,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
					   D3DPOOL_DEFAULT,&tex );
	if( tex==NULL )throw system::Exception( "RenderTexture作成失敗" );
	D3D9RenderTexture *img = new_s D3D9RenderTexture( tex );
	
	LPDIRECT3DSURFACE9 zbuf = NULL;

	lpdev->CreateDepthStencilSurface( img->getWidth(),img->getHeight(),
									  D3DFMT_D16,D3DMULTISAMPLE_NONE,0,
									  FALSE,&zbuf,NULL );
	if( zbuf==NULL ){ delete img;throw system::Exception( "Zバッファ作成失敗"); }

	img->setZbuffer( zbuf );
	return img;
}

