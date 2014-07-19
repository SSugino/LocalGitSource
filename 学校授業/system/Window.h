/** @file
 * �E�B���h�E�Y�I�u�W�F�N�g�N���X�̒�`
 * @date 2009/05/08
 * @author ����
 **/

#pragma once;
#include <list>
#include <algorithm>
#include "Application.h"

namespace sugino{
/** �V�X�e���̗̈� */
namespace system{
	
	class Window;									//����`

	/** �C�x���g�����̃C���^�[�t�F�[�X�N���X */
	class IWindowEvent
	{
	public:
		/** ���b�Z�[�W����
		 * @param [in] Window *w;(�h������Ȃ�)�ΏۃE�B���h�E���
		 * @param [in] UINT msg ;���b�Z�[�W
		 * @param [in] WPARAM wp;�p�����[�^
		 * @param [in] LPARAM lp;�p�����[�^
		 * @reeval 1�ق��̃C�x���g�������Ȃ�
		 * @reeval 0����
		 **/
		virtual int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp ) =0;
		virtual ~IWindowEvent(){}					//�J������
	};

	/** �E�B���h�E�Ǘ��N���X */
	class Window
	{
	private:
		HWND hwnd;									//�E�B���h�E�n���h��
		BOOL active;								//TURE:�A�N�e�B�u FALSE:��A�N�e�B�u
		static LRESULT CALLBACK WindowProc( HWND hw,UINT msg,WPARAM wp,LPARAM lp);//���b�Z�[�W����
 		
		/** �C�x���g�����֐��@
		 * @param [in] UINT msg	 : �s������
		 * @param [in] WPARAM wp : ���b�Z�[�W�p�����[�^
		 * @param [in] LPARAM lp : ���b�Z�[�W�p�����[�^
		 **/
		int Evant( UINT msg,WPARAM wp,LPARAM lp );	//���b�Z�[�W���󂯎���ăA�N�e�B�u��Ԃ��擾�A�߂�l���A��܂Ń��b�Z�[�W����
		std::list<IWindowEvent *> wevent;			//�C�x���g���X�g
		void EventClear();							//���X�g�̍폜
		
	public:
		Window();
		~Window(){ EventClear();Destroy(); }
		
		/** �E�B���h�E���� */
		int Create();
		
		/** �E�B���h�E�J�� */
		void Destroy(){ if(hwnd){ DestroyWindow( hwnd ); hwnd=NULL; } }
		
		/** �\�� */
		void Show(){ ShowWindow( hwnd, SW_SHOW ); }
		
		/** ��\�� */
		void Hide(){ ShowWindow( hwnd, SW_HIDE ); }
		
		/**�@�ĕ`��v���@*/
		void Refresh(){ InvalidateRect( hwnd,NULL,FALSE ); }
		
		/** �n���h���擾
		 * return	hwnd	:�E�B���h�E�n���h��
		 */
		inline HWND getHwnd(){ return hwnd; }		//�n���h���擾

		/** ��ʏ�Ԏ擾
		 * return	IsWindowVisible( hwnd )!=0
		 **/
		inline bool isVisible(){ return IsWindowVisible( hwnd )!=0;   };

		/** �A�N�e�B�u��Ԃ̎擾
		 * return active	:�A�N�e�B�u
		 * @note	true:�A�N�e�B�u	false:��A�N�e�B�u
		 **/
		inline bool isActive(){ return active!=0; };

		/** ��ʐݒ�̎擾
		 * return	GetWindowLong( hwnd,GWL_STYLE )
		 **/
		inline DWORD getStyle(){ return GetWindowLong( hwnd,GWL_STYLE ); }

		/** �C�x���g�������X�g�ɒǉ� */ 
		void addEvent( IWindowEvent *e ){ wevent.push_back(e);	}
		
		/** �C�x���g�p�֐� */
		static bool doEvents();
		
		/** �X�^�C����` */
		enum WINDOW_STYLE{
			/** �T�C�Y��ύX�s�ɂ��� */
			WS_GAME = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			/** �^�C�g�� */
			WS_POP = WS_POPUP | WS_SYSMENU,
			/** �ʏ�E�B���h�E */
			WS_APP = WS_OVERLAPPEDWINDOW
		};

		/** �f�t�H���g�͒����Ɉʒu�ύX */
		enum WINDOW_SET_POSITION{ WSP_CENTER = 0x8000000 };

		/** �^�C�g���̃Z�b�g
		 * @param [in]	�^�C�g����
		 **/
		void setTitle( const char* t ){ SetWindowText( hwnd,t); }
		
		/** �T�C�Y�̃Z�b�g�i�N���C�A���g�̈�) 
		 * @param [in]	int	w	:��
		 * @param [in]	int h	:����
		 **/
		void setSize( int w,int h );
		
		/** �T�C�Y�̎擾
		 * @param [out]	int* w	:��
		 * @param [out]	inj* h	:����
		 * @note �����͕ϐ�������	
		 * int wid,hei; getStyle( &wid,&hei );
		 **/
		void getSize( int *w,int *h ); //�ʂ̏ꏊ�Ɏ��̂�����̂�*
		
		/** �X�^�C���̃Z�b�g
		 * @param [in] s �X�^�C���萔
		 **/
		void setStyle( WINDOW_STYLE s );
		
		/** �E�B���h�E�̈ʒu���Z�b�g*/
		void setPosition( int x = WSP_CENTER,int y=WSP_CENTER );
		
		/** �A�C�R���ݒ� 
		 * @param [in]	res_name :���\�[�X��
		 **/
		void setIcon( char* res_name );
		
		/** ���\�[�X�t�@�C���̓ǂݍ���
		 * @param [in]	���\�[�X�t�@�C����
		 **/
		void setMenu( const char* res_name );	
		
		/** �����ݒ���ꊇ�ō쐬
		 * @param [in] const char* t :�^�C�g��
		 * @param [in] int w		 :��ʕ�
		 * @param [in] int h		 :��ʍ���
		 * @param [in] WINDOW_STYLE  :WS_GAME �T�C�Y�ύX�s��
		 *							 :WS_POP  �^�C�g��
		 *							 :WS_APP  �ʏ�E�B���h�E
		 **/
		int Create( const char* t,int w,int h,WINDOW_STYLE s );
	};

	/** �I���C�x���g���s���֐��@:�@IWindowEvent����̔h��
	 *  @note �~�{�^���ŃE�B���h�E��������ɃA�v���P�[�V�������I������ۂɂ̓E�B���h�E�ɑ΂��ēo�^����
	 **/
	class DestroyEvent : public IWindowEvent 
	{
		int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp );
	};

	/** �y�C���g�C�x���g�p�C���^�[�t�F�[�X�N���X
	 * @note�y�C���g�������s���A�v���P�[�V�����͂��̃N���X���C���v�������g(�C���^�[�t�F�[�X��h�����ɒǉ�)��
	 * Update���I�[�o�[���C�h����
	 **/
	class IPaintEvent {
	public:
	/** �ĕ`��C�x���g
	 * @param dc �`��̈�̃f�o�C�X�R���e�L�X�g
	 **/
	virtual void Update( HDC dc ) = 0;
	};

	/** �y�C���g�C�x���g�����N���X : IWindowEvent����̔h��
	 * �y�C���g�C�x���g�̃C�x���g���s���N���X
	 **/
	class PaintEvent : public IWindowEvent {
	private:
		IPaintEvent *pevent;				//���g�̃|�C���^������
		int Event( Window *w,UINT msg,WPARAM wp,LPARAM lp );
	public:
		/** �R���X�g���N�^
		 * @param [in]	IPaintEvent	*p	:�y�C���g�����̎������s�����N���X�̃|�C���^������������
		 **/
		PaintEvent( IPaintEvent *p ) { pevent=p; }
	};
	
	/** ���j���[�C�x���g�C���^�t�F�[�X�N���X */
	class IMenuEvent {
	public:
		/** ���j���[ID�����C�x���g
		 * @param [in] UINT	id :���\�[�X����ID
		 **/
		virtual void Menu( UINT id ) = 0;
	};
	
	/** ID�C�x���g�����N���X
	 *	ID�C�x���g�̃C�x���g���s���N���X
	 **/
	class MenuEvent : public IWindowEvent {
	private:
		IMenuEvent *mevent;
		int Event( Window *w, UINT msg,WPARAM wp,LPARAM lp );
	public:
		/** �R���X�g���N�^
		 * @param [in] IMenuEvent *m	:�y�C���g�����̎������s�����N���X�̃|�C���^������������
		 **/
		MenuEvent( IMenuEvent *m ) { mevent=m; }
	};
	
	/** POINT�\���̃N���X (���g��x,y)*/
	class Point : public POINT {
	public:
		Point(){}
		Point( int px,int py ){ x=px, y=py; }
	};

	/** RECT�\���̃N���X(���g��x1,y1,x2,y2) */
	class Rect : public RECT{
	public:
		Rect(){}
		Rect( int x1,int y1,int x2,int y2 ){
			left=x1, top=y1, right=x2, bottom=y2; }
	};
	
	/** �t�H���g�Ȉ՗��p�N���X */
	class Font{
		HFONT font;
	public:
		Font(){ font = NULL; }
		/** �T�C�Y�Ə����w��
		 * @param [in] int		height :�����̍���
		 * @param [in] char*	name	:������(�S�p����)
		 * @note ��2�������̖�: �l�r �S�V�b�N�@�l�r �����Ȃ�
		 **/
		Font( int height,char *name ){ Create( height,name ); }	
		
		/** ��� */
		~Font(){ Delete(); }
		void Delete(){ if(font){ DeleteObject(font); font=NULL; } }
		
		/** �t�H���g�̍쐬
		 * @param [in] int	height	:�����̍���
		 * @param [in] char	*name	:������(�S�p����)
		 * @param [in] int	bold	:����
		 * @param [in] BOOL italic	:�C�^���b�N��
		 * @note ��2�������̖�: �l�r �S�V�b�N�@�l�r �����Ȃ�
		 **/
		void Create( int height,char *name,int bold=FW_REGULAR,BOOL italic=FALSE ){
			Delete();
			font = ::CreateFont( height,	//����
					0,0,0,					//������,�e�L�X�g�̊p�x,x���p�x
					bold,					//�t�H���g�̑���
					italic,0,0,				//�C�^���b�N��,�A���_�[���C��,�ł�������
					SHIFTJIS_CHARSET,		//�����Z�b�g
					OUT_DEFAULT_PRECIS,		//�o�͐��x
					CLIP_DEFAULT_PRECIS,	//�N���b�s���O���x
					PROOF_QUALITY,			//�o�͕i��
					FIXED_PITCH | FF_MODERN,//�s�b�`�ƃt�@�~���[
					name );					//���̖�
				}
		/**�L���X�g */
		operator HFONT() const { return font; }
	};	
}}
