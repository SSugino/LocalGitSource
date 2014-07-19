/** @file
 * Direct3D�`��֐��C���^�[�t�F�[�X�N���X�̒�`
 * @date 2009/05/13
 * @author ����
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../system/Window.h"
#include <d3dx9.h>

namespace sugino{
/** �O���t�B�b�N�̗̈� */
namespace graphics{
		/**�@�����^�����O�N���X : IRender����̔h��
		 * �@Direct3D���g���������^�����O���s��
		 **/
		class D3D9Render : public IRender
		{
		private:
			//�e�N�X�`���̓h��Ԃ��֐����g���֐��^�C�v
			LPDIRECT3D9				lp3d;
			//�����_�����O�̎��s�A���\�[�X�̍쐬�A�V�X�e�����x���ϐ��̏����A
			//�K���}�Ȑ����x���̒����A�p���b�g�̏����A�V�F�[�_�̍쐬���s��
			LPDIRECT3DDEVICE9		lpdev;
			//��ʃ��[�h��ŏI�I�ȉ摜�]�����@�Ȃǂ̐ݒ������
			D3DPRESENT_PARAMETERS	d3dpp;
			//�T�[�t�F�C�X�t�H�[�}�b�g�񋓑�
			D3DFORMAT				format;
			//Font�p
			LPD3DXFONT				dxfont;
			LPD3DXSPRITE			sp;
			
			//RenderTaget�ۑ��p
			LPDIRECT3DSURFACE9		back;		//�o�b�N�o�b�t�@
			LPDIRECT3DSURFACE9		zbuf;		//Z�o�b�t�@
	
			IRestoreEvent			*restore;	//�f�o�C�X���X�g���̕���

			/** �����ݒ���s�� */
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

			IImage *createImage( const char *name );//IRender�h���ǂݍ��݊֐�
			void setTexture( IImage *tex );//IRender�h���ݒ�

			/** ���z��ʂ̍쐬
			 * @param [in] int w		:��ʕ�
			 * @param [in] int h		:��ʍ���
			 * @return	autoptr�ŗp�ӂ��ꂽIImage�̌^
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 **/
			IImage *createImage( int w,int h );			//RenderTexture(���z���)�̍쐬
			
			/** ���z��ʂ̃Z�b�g
			 * @param [in] tex			:create�ō��ꂽ�摜�ϐ�
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 * @note render->setTexture( display );
			 **/
			void setRenderTarget( IImage *tex );		//RenderTarget�؂�ւ�

			/** �`���ʂ̎擾
			 * @param [in] tex			:create�ō��ꂽ�摜�ϐ�
			 * @note autoptr<IRender>	render;
			 * @note autoptr< IImage >	display;
			 * @note display = render->createImage( 512,512 );
			 * @note render->getTargetImage( display );
			 **/ 
			void getTargetImage ( IImage *tex );		//�����݁A�`�悳��Ă����ʂ̎擾

			/** �A���t�@���[�h�̐ݒ� */
			ALPHA_MODE setAlphaBlend( ALPHA_MODE alm );
			
			/** �J�����O���[�h�̐ݒ� */
			CULLING_MODE setCullMode( CULLING_MODE culm );
			
			/** Z�o�b�t�@���[�h�̐ݒ� */
			ZBUFFER_MODE setZBufferMode( ZBUFFER_MODE zm );
			
			/** �V�F�[�f�B���O���[�h�̐ݒ� */
			SHADE_MODE setShedingMode( SHADE_MODE sm );
			
			/** �e�N�X�`���k���t�B���^ */
			FILTER_MODE setTextureMinFilterMode( FILTER_MODE min );
			
			/** �e�N�X�`���g��t�B���^ */
			FILTER_MODE setTextureMaxFilterMode( FILTER_MODE max );
			
			/** �e�N�X�`���{�[�_�[���[�h */
			BORDER_MODE setTextureBorderMode( BORDER_MODE mode );


			//
			void setFont( const system::Font &font );
			void drawString( int x,int y,const char*str,DWORD col );

			/** float���l�\���\������\��
			 * ����const char�� %f�����鎖
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

			/** Render����`��͈͂̎擾
			 * @param [out] Rect *vp	:�`��͈�
			 */
			void getViewport( system::Rect *r );
			
		};
	}
}