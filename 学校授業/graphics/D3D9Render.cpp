/** @file
 * Direct3D�`��֐��C���^�[�t�F�[�X�N���X�̎���
 * @date 2009/05/13
 * @author ����
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
 *  IImage����̔h��
 **/
class D3D9Texture : public IImage
{
private:
	LPDIRECT3DTEXTURE9 tex;
	LPDIRECT3DSURFACE9 surface;
	D3DSURFACE_DESC	   desc;
public:
	enum{
		/** D3D9texture��ID */
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
	/** �R���X�g���N�^ */
	D3D9Texture( LPDIRECT3DTEXTURE9 _tex )
	{
		tex = _tex;
		tex->GetSurfaceLevel( 0,&surface );
		surface->GetDesc( &desc );
	}
	/** �e�N�X�`���擾 */
	inline LPDIRECT3DTEXTURE9 getTexture(){ return tex; }

	/** �T�[�t�F�C�X�擾 */
	inline LPDIRECT3DSURFACE9 getSurface(){ return surface; }
};

/** Direct3DRenderTexture
 *  D3D9Texture����̔h��
 *  ���z��ʓI�Ȃ���
 **/
class D3D9RenderTexture : public D3D9Texture
{
private:
	LPDIRECT3DSURFACE9	zbuf;
public:
	enum{
		ID = 9998
	};
	/** ������ */
	D3D9RenderTexture( LPDIRECT3DTEXTURE9 tex ) : D3D9Texture( tex ){ 
		zbuf = NULL;
	}
	
	virtual ~D3D9RenderTexture(){ Dispose(); }
	
	virtual int getID(){ return ID; }
	/** ��� */
	void Dispose() { if(zbuf) zbuf->Release();
						zbuf=NULL; }
	/** �y�o�b�t�@�擾 */
	LPDIRECT3DSURFACE9 getZbuffer(){ return zbuf; }
	/** �y�o�b�t�@�Z�b�g */
	void setZbuffer( LPDIRECT3DSURFACE9 z ){ zbuf = z; }
};


/** ������
 * @param[in] win:�\����
 * @param[in] full:�X�N���[�����[�h
 **/
void D3D9Render::Initialize( Window* win,bool full )
{
	Uninitialize();
	int w,h;
	win->getSize( &w,&h );
	lp3d = Direct3DCreate9( D3D_SDK_VERSION );
	if( lp3d==NULL ) throw Exception( "D3D�̍쐬�Ɏ��s" );

	D3DDISPLAYMODE mode;
	//�O���{�ł̃f�B�X�v���C�ւ̕`����@�̎擾
	lp3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT,&mode );
	format = mode.Format;			//���݂̉�ʃ��[�h

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
	//�f�B�X�v���C�ւ̃A�_�v�^�i�O���{�j��\�����߂̃f�o�C�X���쐬
	//�n�[�h�E�F�A�ɔC����i���_����������j

	//ret = lp3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
	//						  d3dpp.hDeviceWindow,
	//						  D3DCREATE_HARDWARE_VERTEXPROCESSING,
	//						  &d3dpp,&lpdev );
	
	///�n�[�h�E�F�A�����łȂ����,
	//�\�t�g�E�F�A�ł̒��_�����ɂ���
	//if( ret!=D3D_OK ){
		ret = lp3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
								 d3dpp.hDeviceWindow,
								 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								 &d3dpp,&lpdev );
		//����ł��f�o�C�X�쐬�ł��Ȃ����
		if( ret!=D3D_OK ) throw Exception( "D3D�f�o�C�X�쐬���s" );
	//}

	// Font Sprite Create
	setFont( Font( 12,"MS GOTHIC") );//����12��MS�S�V�b�N
	D3DXCreateSprite( lpdev,&sp );
	if( sp==NULL ) throw Exception( "Font�p�X�v���C�g�쐬���s" );

	Clear(0);					//target���N���A���Ă���
	setAlphaBlend( ALM_ALPHA );//�A���t�@�u�����h�Ŏw��
	
	lpdev->GetRenderTarget(0,&back);
	lpdev->GetDepthStencilSurface(&zbuf);

	setDefaultState();
}

//�R���X�g���N�^
D3D9Render::D3D9Render()
{
	back=NULL;
	zbuf=NULL;

	lp3d = NULL;
	lpdev = NULL;
	//Font�p
	dxfont = NULL;
	sp = NULL;
}

//�Еt��
void D3D9Render::Uninitialize()
{
	if(back){back->Release();back=NULL;}
	if(zbuf){zbuf->Release();zbuf=NULL;}
	if( sp ){ sp->Release();sp=NULL; }
	if( dxfont ){ dxfont->Release();dxfont=NULL; }
	if( lpdev ){ lpdev->Release();	lpdev=NULL; }
	if( lp3d ){ lp3d->Release(); lp3d=NULL; }
}
/* �r���[�|�[�g�ݒ�
 * @param [in] const Rect &r :�͈�
 */
void D3D9Render::setViewport( const system::Rect &r )
{
	//�N���b�v�͈͐ݒ�
	D3DVIEWPORT9 view = {r.left, r.top, r.right-r.left, r.bottom-r.top,0,1};
	lpdev->SetViewport( &view );
}
/** ��ʍX�V
 * @param[in] win:�\����
 * @param[in] &r :�N���b�v�͈�
 **/
void D3D9Render::Update( Window* win,const Rect &r )
{
	//�f�o�C�X�����L����o�b�N�o�b�t�@�̃V�[�P���X�̒��ŁA���̃o�b�t�@�̃R���e���c�𑗂�(�摜�]��)
	// �\���@�� Update(�X�V) �� �\�� �� Update(�X�V) �����
	int ret = lpdev->Present( &r,NULL,win->getHwnd(),NULL );
	if( ret==D3DERR_DEVICELOST )
	{
		
		if(restore)restore->Dispose();
		//z�o�b�t�@�ƃo�b�N�o�b�t�@�Ď擾
		if(zbuf){zbuf->Release();zbuf=NULL;}
		if(back){back->Release();back=NULL;}

		//���X�g(�f�o�C�X��������)����
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
				throw Exception( "���X�ƃf�o�C�X�̕����Ɏ��s���܂���" );
			}
		}
	}
}
//��ʂ̃N���A
void D3D9Render::Clear(DWORD col)
{
	//lpdev->Clear( 0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,col,1.0f,0 );//�o�b�t�@�����F�œh�邩(�F��0xAARRGGBB)
	lpdev->Clear( 0,NULL,D3DCLEAR_TARGET,col,1.0f,0 );//�o�b�t�@�����F�œh�邩(�F��0xAARRGGBB)
}

//Z�o�b�t�@�N���A
void D3D9Render::ClearZ()
{
	lpdev->Clear( 0,NULL,D3DCLEAR_ZBUFFER,0,1.0f,0 );
}


//�`��J�n
bool D3D9Render::beginScene()
{
	return lpdev->BeginScene() == D3D_OK;//Lock �摜�`��O�ɋL�q
}
//�`��I��
void D3D9Render::endScene()
{
	lpdev->EndScene();//Unlock�@�`��I������L�q
}

///�摜�ǂݍ���
IImage *D3D9Render::createImage( const char *name )
{
	LPDIRECT3DTEXTURE9 tex = NULL;
	D3DXCreateTextureFromFile( lpdev,name,&tex );//�ǂݍ��� �g���q��(bmp/png/jpeg...)
	if(tex==NULL)//�摜�Ȃ�
	{
		std::string str="�摜�̓ǂݍ��ݎ��s\n"+std::string( name );
		throw Exception( str.data() );
	}
	return new_s D3D9Texture( tex );
}

//�摜�̃Z�b�g
void D3D9Render::setTexture( IImage *tex )
{
	if( tex==NULL ){ lpdev->SetTexture( 0,NULL ); return; }
//	if( tex->getID() != D3D9Texture::ID )
//		throw Exception( "D3DRender::setTexture()\n"
//						 "�e�N�X�`���ȊO��Image���w�肳��܂���");// ""  "" �����s�����鎞""�͓��g����
	D3D9Texture *t = ( D3D9Texture *)tex;		//down cast
	lpdev->SetTexture( 0,t->getTexture() );
}

//RenderTarget�؂�ւ�
void D3D9Render::setRenderTarget( IImage *tex )
{
	//���ɖ߂�
	if( tex==NULL ){
		lpdev->SetRenderTarget( 0,back );
		lpdev->SetDepthStencilSurface( zbuf );
		return;
	}
	
	if( tex->getID() != D3D9RenderTexture::ID ) throw system::Exception( "RenderTexture��ID����v���܂���" );

	D3D9RenderTexture *t=	(D3D9RenderTexture*) tex;	//�_�E���L���X�g
	lpdev->SetRenderTarget( 0,t->getSurface() );
	lpdev->SetDepthStencilSurface( t->getZbuffer() );
}

//�t�H���g�̐ݒ�
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
	if( dxfont==NULL ){ throw Exception( "Font�쐬�Ɏ��s" ); }
}
//�`��\��
void D3D9Render::drawString( int x,int y,const char *str,DWORD col )
{
	sp->Begin( D3DXSPRITE_ALPHABLEND );
	RECT r={ x,y,0,0 };
	dxfont->DrawText( sp,str,-1,&r,DT_LEFT | DT_CALCRECT, col );//�͈͌v�Z
	dxfont->DrawText( sp,str,-1,&r,DT_LEFT, col );//�`��

	sp->End();
}	


void D3D9Render::drawString( int x,int y,const char *str,float value,DWORD col )
{
	char buf[256];
	sprintf( buf,str,value);
	drawString(x,y,buf,col );					//FPS�\��
}

//�_�̕`��
void D3D9Render::drawPixel( int x, int y, DWORD col )
{
	D3DRECT r = { x,y,x+1,y+1 };
	lpdev->Clear( 1,&r,D3DCLEAR_TARGET,col,0,0 );
}
//���C���̕`��
//�F���_Ver( IRender.h�Q�� )
void D3D9Render::drawLine( ColorTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(ColorTVertex) );
}
//�e�N�X�`�����W�݂̂̒��_( IRender.h�Q�� )
void D3D9Render::drawLine( TextureTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureTVertex) );
}
//���W�ƐF�ƃe�N�X�`�����W�������_( IRender.h�Q�� )
void D3D9Render::drawLine( TextureColorTVertex *v, WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 |D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_LINELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureColorTVertex) );
}

//�|���S���`��
//�F���_Ver( IRender.h�Q�� )
void D3D9Render::drawPolygon( ColorTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(ColorTVertex) );
}
//�|���S���`��
//�e�N�X�`�����W�݂̂̒��_( IRender.h�Q�� )
void D3D9Render::drawPolygon( TextureTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureTVertex) );
}

//���W�ƐF�ƃe�N�X�`�����W�������_( IRender.h�Q�� )
void D3D9Render::drawPolygon( TextureColorTVertex *v,WORD *index,int vn,int n )
{
	lpdev->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE );
	lpdev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0,vn,n,
								   index,D3DFMT_INDEX16,v,sizeof(TextureColorTVertex) );
}
/** �A���t�@���[�h�ύX
 * @param [in] ALPHA_MODE alm :���[�h(�񋓑�)
 *							  :0:ALM_NONE  �A���t�@�Ȃ�
 *							  :1:ALM_ALPHA �u�����h
 *							  :2:ALM_ADD   ���Z
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

/** �J�����O���[�h�ύX
 * @param [in] CULLING_MODE culm :���[�h(�񋓑�)
 *								 :0:	CM_NON	���ʕ`��
 *								 :1:	CM_CCW	�u�\�v�ʕ`�� �����v����
 *								 :-1:	CM_CW�u���v�ʕ`�� ���v���
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


/** Z�o�b�t�@���[�h�ύX
 * @param [in] ZBUFFER_MODE zm :���[�h(�񋓑�)
 *							   :0:	ZM_OFF			��r���Ȃ�
 *							   :1:	ZM_ON			��r����
 *							   :2:	ZM_OFF_WRITE	��r�����X�V
 *							   :3:	ZM_ON_WRITE		��r���čX�V
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
/** �V�F�[�f�B���O���[�h�̐ݒ�
 * @param [in] SHADE_MODE sm	:���[�h(�񋓑�)
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

/** �V�F�[�f�B���O���[�h�̐ݒ�
 * @param [in] FILTER_MODE min	:���[�h(�񋓑�)
 *							    :0:		FM_NONE			�t�B���^�Ȃ�
 *								:1:		FM_POINT		�_�t�B���^
 *								:2:		FM_LINEAR		���j�A�t�B���^
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
/** �V�F�[�f�B���O���[�h�̐ݒ�
 * @param [in] FILTER_MODE min	:���[�h(�񋓑�)
 *							    :0:		FM_NONE			�t�B���^�Ȃ�
 *								:1:		FM_POINT		�_�t�B���^
 *								:2:		FM_LINEAR		���j�A�t�B���^
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
/** �{�[�_�[���[�h�̐ݒ�
 * @param [in] BOORDER_MODE mode:���[�h(�񋓑�)
 *							    :1:		BM_WRAP			�~��
 *								:2:		BM_MIRROR		��
 *								:3:		BM_CLAMP		����
 * @return	�ݒ�O�̏��
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

	//�e�N�X�`���A���t�@�ƒ��_�A���t�@����Z����悤�ɐݒ�
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAOP,D3DTOP_MODULATE );
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE );
	lpdev->SetTextureStageState( 0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE );

	/*
	lpdev->SetRenderState( D3DRS_DITHERENABLE,TRUE );			//FALSE�̂ق��������Ƃ�������
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
	if( tex->getID() != D3D9RenderTexture::ID ) return;		//ID���s��v�Ȃ烊�^�[��
	D3D9Texture *t = ( D3D9Texture *) tex;
	LPDIRECT3DSURFACE9 src = NULL;							//��x�������
	lpdev->GetRenderTarget( 0,&src );						//�`���̎擾
	lpdev->StretchRect( src,NULL,t->getSurface(),NULL,D3DTEXF_POINT );

	if(src){
		src->Release();										//���
		src=NULL;											
	}
}

IImage *D3D9Render::createImage( int w,int h )
{
	LPDIRECT3DTEXTURE9 tex=NULL;
	D3DXCreateTexture( lpdev,w,h,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,
					   D3DPOOL_DEFAULT,&tex );
	if( tex==NULL )throw system::Exception( "RenderTexture�쐬���s" );
	D3D9RenderTexture *img = new_s D3D9RenderTexture( tex );
	
	LPDIRECT3DSURFACE9 zbuf = NULL;

	lpdev->CreateDepthStencilSurface( img->getWidth(),img->getHeight(),
									  D3DFMT_D16,D3DMULTISAMPLE_NONE,0,
									  FALSE,&zbuf,NULL );
	if( zbuf==NULL ){ delete img;throw system::Exception( "Z�o�b�t�@�쐬���s"); }

	img->setZbuffer( zbuf );
	return img;
}

