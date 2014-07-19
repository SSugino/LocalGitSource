/** @file
 * �V�[���̃N���X�̒�`
 * @date 2009/06/21
 * @author ����
 **/

#pragma once


#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
#include "../input/Input.h"

namespace sugino{
namespace game{

	class SceneManager;
	class ISceneEffect;

	/** �V�[���̊��N���X */
	class SceneBase
	{
	protected:
		/** �ǂ̃V�[���ł����ʂ͂ǂ�ǂ�ǉ����Ă���
		 *	��L�[�Ƃ��@���C������K�v�ȕϐ��Ƃ��������ł��󂯎�� */
		/** �摜�� */
		graphics::IRender	*render;
		/** �V�[�� */
		SceneManager		*scene;
		/** �L�[ */
		input::Key			*key;
		/** �}�E�X */
		input::Mouse		*mouse;
	public:
		/** �R���X�g���N�^�ŉ摜����� */
		SceneBase( graphics::IRender *rend,input::Key *k,input::Mouse *m )
		{
			render = rend;
			key	   =  k;
			mouse  =  m;
		}
		virtual ~SceneBase( ){}
		/** ������ */
		virtual void Init()=0;
		/** �v�Z�Ȃǃ��C�� */
		virtual void Main()=0;
		/** �\�� */
		virtual void Draw()=0;

		void setManager( SceneManager *sm){ scene = sm; }
	};
	
	/** �V�[���؂�ւ��̊��N���X */
	class ISceneEffect
	{
	public:
		//�h����ŏ���������̂ŏ����ŗp��
		virtual ~ISceneEffect(){}
		virtual void Init()=0;
		virtual void Main()=0;
		virtual void Draw()=0;
		/** ���o���I��������? */	
		virtual bool isEnd()=0;
	};

	/** �V�[���̊Ǘ��N���X */
	class SceneManager
	{
	private:
		/** ���̃V�[�� */
		SceneBase *now;
		/** ���̃V�[�� */
		SceneBase *next;
		/** �V�[���ɓ����Ă���G�t�F�N�g */
		ISceneEffect *se_in;
		/** �V�[�����I���Ƃ��̃G�t�F�N�g */
		ISceneEffect *se_out;
		
		void setNextScene();
	public:
		/** NULL������ */
		SceneManager(){ now=NULL;next=NULL;se_in=NULL;se_out=NULL; }
		~SceneManager()
		{
			if( now ) delete now; now=NULL;
			if( next ) delete next; next=NULL;
			if( se_in ) delete se_in; se_in=NULL;
			if( se_out ) delete se_out; se_out=NULL;
		}
		
		/** �e�V�[���̏����� */
		void Init();
		/** ���C�� */
		void Main();
		/** �\�� */
		void Draw();
		/** �V�[���̕ύX ���������Ă� */
		void setScene( SceneBase *sb,ISceneEffect *out,ISceneEffect *in )
		{
			/** �ڍs���̓Z�b�g�ł��Ȃ�/�����o����xdelete */
			if( next ){ delete sb;delete out;delete in;return ; }
			
			if( se_out ){ delete se_out;}	//��x���
			if( se_in ){ delete se_in;}
			se_out=out;				//�t�F�[�h�A�E�g������
			se_in=in;				//�t�F�[�h�C��������
			next  = sb;				//���݂̃V�[������
			if( se_out==NULL )		//�t�F�[�h�������Ȃ��Ȃ�
			{
				setNextScene();
			}else se_out->Init();
		}
		/** �t�F�[�h�C����Ԃ𒲂ׂ�i�I���������H�j */
		bool isEffectIn(){ return ( se_out==NULL&&se_in ); }
		/** �t�F�[�h�A�E�g��Ԃ𒲂ׂ�i�I���������H�j */
		bool isEffectOut(){ return ( se_out!=NULL ); }
		/** �G�t�F�N�g���́`�̏���/�������Ȃ� */
		bool isEffect(){ return isEffectIn()||isEffectOut() ; } 
	};

	
	/** �t�F�[�h�A�E�g�N���X(�������Z���F) */
	class FadeOut : public ISceneEffect
	{
	private:
		/** �摜�� */
		graphics::IRender *render;
		/** �h��Ԃ������F */
		DWORD col;
		/** �A���t�@�l */
		float alpha;
		/** �h��Ԃ����x */
		float speed;
		/** �`��͈� */
		system::Rect rc;
	public:
		/** ������
		 * @param [in] IRender *r	:�摜��
		 * @param [in] DWORD	c	:�F 0x
		 * @param [in] float   sp	:�A���t�@�l�ϓ��X�s�[�h	
		 **/
		FadeOut( graphics::IRender *r,DWORD c,float sp )
		{
			render	=r;
			col		=c;
			speed	=sp;
			alpha	=0.0f;	//�t�F�[�h�A�E�g�Ȃ̂ŏ����l��0
			render->getViewport( &rc );
		}
		/** ������ */
		void Init(){}
		/** ���C�� */
		void Main()
		{
			alpha += speed;
			if( alpha >=1.0 ) alpha = 1.0;	
		}

		/** �\�� */
		void Draw()
		{
			DWORD c = (DWORD)(alpha*255);
			render->drawFill( rc,col|(c<<24) );
		}

		/** �I���������ǂ��� */
		bool isEnd()
		{
			return alpha >= 1.0;
		}
	};

	/** �t�F�[�h�C���N���X�i�Z���F�������j */
	class FadeIn : public ISceneEffect
	{
		private:
		/** �摜�� */
		graphics::IRender *render;
		/** �h��Ԃ������F */
		DWORD col;
		/** �A���t�@�l */
		float alpha;
		/** �h��Ԃ����x */
		float speed;
		/** �`��͈� */
		system::Rect rc;
	public:
		/** ������
		 * @param [in] IRender *r	:�摜��
		 * @param [in] DWORD	c	:�F 0x
		 * @param [in] float   sp	:�A���t�@�l�ϓ��X�s�[�h	
		 **/
		FadeIn( graphics::IRender *r,DWORD c,float sp )
		{
			render	=r;
			col		=c;
			speed	=sp;
			alpha	=1.0f;	//�t�F�[�h�A�E�g�Ȃ̂ŏ����l��0
			render->getViewport( &rc );
		}
		
		/** ������ */
		void Init(){}
		/** ���C�� */
		void Main()
		{
			alpha-= speed;
			if( alpha <=0.0 ) alpha = 0;
		}
		/** �\�� */
		void Draw()
		{
			DWORD c = (DWORD)(alpha*255);
			render->drawFill( rc,col|(c<<24) );
		}
		/** �I���������ǂ��� */
		bool isEnd()
		{
			return alpha <= 0.0;
		}
	};
}}