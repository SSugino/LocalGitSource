/** @file
 * �`��֐��C���^�[�t�F�[�X�N���X�̒�`
 * @date 2009/05/13
 * @author ����
 **/

#pragma once

#include "../system/Window.h"
#include "../math/Vector.h"

namespace sugino{
namespace graphics{
		//-------------------------------------------------------------------------
		//Virtex Formats
		//-------------------------------------------------------------------------
		/**�@�_�E�F�̎w�� */
		struct ColorTVertex
		{
			float x,y,z,w;
			DWORD c;
			ColorTVertex(){};
			ColorTVertex(float _x,float _y,DWORD _c) :
				x( _x),y( _y),z(0),w(1),
					c( _c) {};
			/** 2D�x�N�g���p */
			ColorTVertex( const math::Vector2 &v,DWORD _c) :
			x(v.x),y(v.y),z(0),w(1),
				c( _c) {};
			/** 3D�x�N�g���p */
			ColorTVertex( const math::Vector3 &v,DWORD _c) :
			x(v.x),y(v.y),z(v.z),w(1),
				c( _c) {};
			/** 4�����x�N�g���p */
			ColorTVertex( const math::Vector4 &v,DWORD _c) :
			x(v.x),y(v.y),z(v.z),w(v.w),
				c( _c) {};


		};
		/** �e�N�X�`�����W�݂̂̓_�@*/
		struct TextureTVertex
		{
			float x,y,z,w;
			float tx,ty;
			TextureTVertex() {};
			TextureTVertex( float _x,float _y,float _tx,float _ty ) :
				x( _x),y( _y),z(0),w(1),
				tx( _tx),ty( _ty) {};
			/** 2D�x�N�g���p */
			TextureTVertex( const math::Vector2 &v,float _tx,float _ty ) :
				x( v.x),y( v.y),z(0),w(1),
				tx( _tx),ty( _ty) {};
			
			/** 3D�x�N�g���p */
			TextureTVertex( const math::Vector3 &v,float _tx,float _ty,float _tz ) :
				x(v.x),y( v.y ),z(v.z),w(1),
				tx( _tx),ty( _ty) {};

			/** 4�����x�N�g���p */
			TextureTVertex( const math::Vector4 &v,float _tx,float _ty,float _tz ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( _tx),ty( _ty) {};

		};
		/** ���W�ƐF�ƃe�N�X�`�����W�����_ */
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
			/** 2D�x�N�g���p */
			TextureColorTVertex( const math::Vector2 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(0),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			/** 3D�x�N�g���p */
			TextureColorTVertex( const math::Vector3 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			/** 3D�x�N�g���p */
			TextureColorTVertex( const math::Vector3 &v,const math::Vector3 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(1),
				tx( v2.x),ty( v2.y),
				c( _c) {};
			
			/** �l�����x�N�g���p */
			TextureColorTVertex( const math::Vector4 &v,const math::Vector3 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( v2.x),ty( v2.y),
				c( _c) {};

			/** �l�����x�N�g���p */
			TextureColorTVertex( const math::Vector4 &v,const math::Vector2 &v2,DWORD _c ) :
				x( v.x),y( v.y),z(v.z),w(v.w),
				tx( v2.x),ty( v2.y),
				c( _c) {};

		};

		/** �����_�����O�\�e�N�X�`���̔j���A���A�C�x���g�N���X��` */
		class IRestoreEvent
		{
		public:
			virtual ~IRestoreEvent(){}
			/** �j���̏������z�֐� */
			virtual void Dispose()=0;
			/** �����̏������z�֐� */
			virtual void Restore()=0;
		};

		/**�@�摜�i�e�N�X�`���j�������C���^�[�t�F�[�X */
		class IImage{
		public:
			virtual ~IImage(){};
			virtual int getWidth() = 0;
			virtual int getHeight() = 0;
			virtual int getID() = 0;
		};

		/** �����_�����O�C���^�[�t�F�[�X */
		class IRender{
		public:
			virtual ~IRender(){};
			virtual void Initialize( system::Window* win,bool full ) = 0;			//������
			virtual void setViewport( const system::Rect &vp ) = 0;				//�`��͈͐ݒ�
			virtual void getViewport( system::Rect *r )=0;							//�`��͈͎擾
			virtual void Clear( DWORD col ) = 0;									//��ʂ̃N���A
			virtual void Update( system::Window* win,const system::Rect &r ) = 0;	//��ʍX�V
			virtual bool beginScene() = 0;											//�`��J�n
			virtual void endScene() = 0;											//�`��I��
			virtual void drawPixel( int x,int y,DWORD col ) = 0;					//�_��`��
			virtual IImage *createImage( const char *name ) = 0;					//�摜�̓ǂݍ���
			virtual IImage *createImage( int w,int h ) = 0;							//���z��ʉ摜�̓ǂݍ���
			virtual void setTexture( IImage *src ) = 0;								//�摜�̐ݒ�
			virtual void setRenderTarget( IImage *tex ) = 0;						//RenderTarget�̐؂�ւ�
			virtual void setRestoreEvent( IRestoreEvent *e )=0;						//���C�����ŕ�������������
			virtual void getTargetImage( IImage* tex )=0;							//�����݂̕`���ʂ��擾
			virtual void ClearZ() = 0;												//Z�o�b�t�@�̃N���A
			/** ���̕`�扼�z�֐�
			 * @param [in] ColorTVertex v		:�@�F���_
			 * @param [in] WORD			index	:�@���_�C���f�b�N�X
			 * @param [in] int			vn		:�@���_��
			 * @param [in] int			n		:�@������
			 **/
			virtual void drawLine( ColorTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawLine( TextureTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawLine( TextureColorTVertex *v,WORD *index,int vn,int n ) =0;

			virtual void drawPolygon( ColorTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawPolygon( TextureTVertex *v,WORD *index,int vn,int n ) =0;
			virtual void drawPolygon( TextureColorTVertex *v,WORD *index,int vn,int n ) =0;
			/** ������̕`��
			 * @param [in] x,y :�@���W
			 * @param [in] str :�@������
			 * @param [in] col :�@�`��F(0xAARRGGBB)
			 **/
			virtual void drawString( int x,int y,const char *str,DWORD col )  = 0;
			
			virtual void drawString( int x,int y,const char *str,float value,DWORD col )  = 0;
			/** ������`��p�̃t�H���g�̎w�� */
			virtual void setFont( const system::Font &font ) = 0;		
			
			/** �A���t�@���[�h */
			enum ALPHA_MODE{
				ALM_NONE  = 0,	//�A���t�@�Ȃ�
				ALM_ALPHA = 1,	//�A���t�@�u�����h
				ALM_ADD	  =	2	//���Z����
			};
			/** �A���t�@���[�h�̃Z�b�g */
			virtual ALPHA_MODE setAlphaBlend( ALPHA_MODE alm ) = 0;

			/** �J�����O���[�h�i�ʂ̕`��ݒ�j */
			enum CULLING_MODE{
				CM_NON	= 0,	//���ʕ`��
				CM_CCW	= 1,	//�u�\�v�ʕ`��( Counter Clock Wise ) �����v����
				CM_CW	= -1	//�u���v�ʕ`��( Clock Wise ) ���v���
			};
			/** �J�����O���[�h�̐ݒ� */
			virtual CULLING_MODE setCullMode( CULLING_MODE culm ) =	0;
			
			/** Z�o�b�t�@���[�h(���s����r) */
			enum ZBUFFER_MODE{
				ZM_OFF			= 0,	//��r���Ȃ�
				ZM_ON			= 1,	//��r����
				ZM_OFF_WRITE	= 2,	//��r�����ɁA�X�V
				ZM_ON_WRITE		= 3		//��r���X�V
			};
			
			/** Z�o�b�t�@���[�h�̐ݒ� */
			virtual ZBUFFER_MODE setZBufferMode( ZBUFFER_MODE zm ) = 0;
			
			/** �V�F�[�f�B���O�i�ʕ`��j���[�h */
			enum SHADE_MODE{
				SM_WIRE	= 0,			//���C���[�t���[��
				SM_FLAT = 1,			//�J�N�J�N��
				SM_GOURAUD= 2			//���炩
			};
			
			/** �V�F�[�f�B���O���[�h�̐ݒ� */
			virtual SHADE_MODE setShedingMode( SHADE_MODE sm ) = 0;

			/** �e�N�X�`���t�B���^�ݒ� */
			enum FILTER_MODE{
				FM_NONE		= 0,	// �t�B���^�Ȃ�
				FM_POINT	= 1,	// �_�t�B���^
				FM_LINEAR	= 2		// ���j�A�t�B���^
			};
			/** �e�N�X�`���k���t�B���^ */
			virtual FILTER_MODE setTextureMinFilterMode( FILTER_MODE min ) = 0;
			/** �e�N�X�`���g��t�B���^ */
			virtual FILTER_MODE setTextureMaxFilterMode( FILTER_MODE max ) = 0;

			/** �e�N�X�`���{�[�_�[���[�h */
			enum BORDER_MODE
			{
				BM_WRAP		= 1,	// �~��
				BM_MIRROR	= 2,	// ��
				BM_CLAMP	= 3		// ����
			};

			/** �e�N�X�`���{�[�_�[���[�h�̐ݒ� */
			virtual BORDER_MODE setTextureBorderMode( BORDER_MODE mode ) = 0;
			//-----------------------------------------------------------------------
			//		�`��֐�
			//-----------------------------------------------------------------------
			/** ���`��
			 * @param [in]	int		x1		:2�������W�n�_x
			 * @param [in]	int		y1		:2�������W�n�_y
			 * @param [in]	int		x2		:2�������W�I�_x
			 * @param [in]	int		y2		:2�������W�I�_y
			 * @param [in]	DWORD	col1	:�n�_�̐F
			 * @param [in]	DWORD	col2	:�I�_�̐F
			 **/
			void drawLine( int x1,int y1,int x2,int y2,DWORD col1,DWORD col2 );

			/** �h��Ԃ��l�p�`��
			 * @param [in]	Rect	&r		:2�������W��xy
			 * @param [in]	DWORD	col		:�F
			 **/
			void drawFill( const system::Rect &r,DWORD col );//�h��ׂ��l�p

			/** �l�p
			 * @param [in]	Rect	&r		:2�������W��xy
			 * @param [in]	DWORD	col		:�F
			 **/
			void drawBox( const system::Rect &r,DWORD col );//�g
			
			/** Draw�֐�
			 * @param [out] Rect(x1,y1,x2(��),y2(����) )	:�@�\����
			 * @param [in]�@img								:�o�������摜
			 * @param [out] Rect(x1,y1,x2(��),y2(����) )	:�@�\����
			 **/
			void drawImage( const system::Rect &dest,IImage *img, const system::Rect &src );//�摜�`��
			
			/** �Ăяo���₷���`��Draw�֐�
			 * @param [in]	int x		:�\������W
			 * @param [in]	int y		:
			 * @param [in]	int w		:�\���敝
			 * @param [in]	int h		:�\���捂��
			 * @param [in]	IImage *img	:�\���������摜
			 * @param [in]	int sx		:�\�������W(����(w)�擾���Ă����)
			 * @param [in]	int sy		:
			 * @note	�e�N�X�`���T�C�Y��2n��ɂ��邱��
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy )
			{
					drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sx+w,sy+h ) );//�Ăяo���₷���`�̊֐�
			}
			
			/** StretchDraw�֐�
			 * @param [in]	int		x	:�\������W
			 * @param [in]	int		y	:
			 * @param [in]	int		w	:�\���敝
			 * @param [in]	int		h	:�\���捂��
			 * @param [in]	IImage *img :�\�����摜
			 * @param [in]	int		sx	:�\�������W(����(w)�擾���Ă����)
			 * @param [in]	int		sw	:�g��k����
			 * @param [in]  int		sh	:�g��k������
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy,int sw,int sh )
			{
				drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sw/(sx+w),sy/(sh+h) ) );//�Ăяo���₷���`�̊֐�
			}
			
			/** �A���t�@�l�t��
			 * 
			 **/
			void drawImage( const system::Rect &r,IImage *img, const system::Rect &src,DWORD col );
		
			/** �A���t�@�l�t��
			 * �g���₷���`��
			 **/
			void drawImage( int x,int y,int w,int h,IImage *img,int sx,int sy,DWORD col )
			{
				drawImage( system::Rect( x, y, x+w, y+h ),img,system::Rect( sx,sy,sx+w,sy+h ),col );
			}
		};
	}
}