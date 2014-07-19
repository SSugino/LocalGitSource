/** @file
 * �E�B���h�E�Y�I�u�W�F�N�g�N���X�̎���
 * @date 2009/05/08
 * @author ����
 **/
#include "Window.h"
#include "Exception.h"

using namespace sugino;
using namespace system;

//�R���X�g���N�^
Window::Window(){
	hwnd = NULL;
	active = FALSE;
}

//�E�B���h�E�쐬
int Window::Create()
{
	HINSTANCE inst	= GetModuleHandle(NULL);
	char	 *c_name = "_WINDOWS_CLASS_";
	WNDCLASS	wc;								//�N���X�̃X�^�C���w��

	if( !GetClassInfo( inst,c_name,&wc ) )		//���o�^
	{
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
		wc.hCursor = LoadCursor( NULL,IDC_ARROW );
		wc.hIcon = LoadIcon( NULL,IDI_APPLICATION );
		wc.hInstance = inst;
		wc.lpfnWndProc = WindowProc;
		wc.lpszClassName = c_name;
		wc.lpszMenuName = NULL;
		wc.style = 0;
		if( !RegisterClass( &wc ) ) throw Exception( "�N���X�o�^���s" );
	}

	//�g���X�^�C�������E�B���h�E�̍쐬
	hwnd = CreateWindowEx( 0,c_name,"TITLE",WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  NULL,NULL,inst,NULL );
	if( hwnd == NULL ) throw Exception( "�E�B���h�E�쐬���s" );;
	
	//�������g�̃A�h���X��ێ��i���b�Z�[�W�ۑ��p)
	SetWindowLong( hwnd,GWL_USERDATA,(LONG)this );		//�����݂̃E�B���h�E�̏��擾
	return 0;
}
//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK Window::WindowProc( HWND hw, UINT msg, WPARAM wp, LPARAM lp ) //���b�Z�[�W����
{
	int ret = 0;
	//this�|�C���^�擾
	Window *win = ( Window * )GetWindowLong( hw,GWL_USERDATA );
	if( win ) ret = win->Evant( msg,wp,lp );				//������Proc��
	if( !ret ) return DefWindowProc( hw,msg,wp,lp );		//�߂�l���Ȃ�,�~�ȊO�Ȃ�E�B���h�E�ɔC����
	return 0;												//�f�t�H���g
}

int Window::Evant( UINT msg,WPARAM wp,LPARAM lp )
{
	switch( msg )
	{
	case WM_ACTIVATE:
		active = (wp&0xFFFF);								//�A�N�e�B�u��Ԃ��擾
		if(wp>>16) active=0;								//�ŏ������Ă�ꍇ�͔�A�N�e�B�u
		break;
	}
	int ret;
	std::list<IWindowEvent *>::iterator i=wevent.begin();
	while( i!=wevent.end() ){								//end�łȂ����
		if( ret=(*i)->Event( this,msg,wp,lp ) ) return ret;
		i++;												//�߂�l������΂����ŏI��
	}
	return 0;
}
//���b�Z�[�W���[�v
bool Window::doEvents()
{
	MSG msg;
	while(1)
	{
		if( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )		//�L���[�Ƀ��b�Z�[�W�����邩���ׂ���΃��b�Z�[�W���i�[����
		{
			if( msg.message == WM_QUIT ) return false;
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}else break;
	}
	return true;
}

void Window::EventClear()
{
	std::list<IWindowEvent *>::iterator i=wevent.begin();
	while( i!=wevent.end() ) { delete (*i); ++i; }
	wevent.clear();
}

//DestroyEvent�̏������@������
int DestroyEvent::Event( Window *w,UINT msg,WPARAM wp,LPARAM lp )
{
	if( msg==WM_DESTROY ){ PostQuitMessage(0); return 1; }
	return 0;
}

int PaintEvent::Event( Window *w, UINT msg, WPARAM wp, LPARAM lp )
{
	if( msg==WM_PAINT )
	{
		PAINTSTRUCT ps;
		BeginPaint( w->getHwnd(),&ps );
		if( pevent ) pevent->Update( ps.hdc );
		EndPaint( w->getHwnd(),&ps );
		return 1;
	}
	return 0;
}

int MenuEvent::Event( Window *w, UINT msg, WPARAM wp, LPARAM lp )
{
	if( msg==WM_COMMAND )
	{
		if( mevent ) mevent->Menu( wp );
		return 1;
	}
	return 0;
}

void Window::setStyle( WINDOW_STYLE s )
{
	RECT rc;
	GetClientRect( hwnd,&rc );
	if(IsWindowVisible(hwnd)) SetWindowLong(hwnd,GWL_STYLE, s|WS_VISIBLE);
	else					  SetWindowLong(hwnd,GWL_STYLE, s );

	setSize( rc.right - rc.left,rc.bottom - rc.top );
}

void Window::setSize( int w,int h )
{
	RECT rc={ 0,0,w,h };
	AdjustWindowRectEx( &rc,getStyle(),GetMenu(hwnd)?1:0,0);		//�̈�쐬
	SetWindowPos( hwnd,NULL,0,0,rc.right - rc.left, rc.bottom - rc.top,
				  SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED );
}

void Window::getSize( int *w,int *h )
{
	RECT r;
	GetClientRect( hwnd, &r );
	*w = r.right - r.left;
	*h = r.bottom - r.top;
}

void Window::setPosition( int x,int y )
{
	RECT rc;
	GetWindowRect( hwnd,&rc );
	if( x==WSP_CENTER )
	{
		x = (GetSystemMetrics( SM_CXSCREEN )>>1 ) - (( rc.right -rc.left )>> 1 );
	}
	if( y==WSP_CENTER )
	{
		y = (GetSystemMetrics( SM_CYSCREEN )>>1 ) - (( rc.bottom-rc.top )>> 1 );
	}
	SetWindowPos( hwnd,NULL,x,y,0,0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED );
}

void Window::setIcon( char* res_name )
{
	HICON hicon = LoadIcon( GetModuleHandle( NULL ),res_name );
	SendMessage( hwnd,WM_SETICON,0,(LPARAM)hicon );
}

int Window::Create( const char* t,int w,int h,WINDOW_STYLE s )
{
	int ret;
	ret = Create();
	if( ret ) throw Exception( "�E�B���h�E�܂��̓N���X�o�^�@�쐬���s" );
	//�����ݒ�
	setTitle( t );
	setSize( w,h );
	setStyle( s );
	//�����ʒu
	setPosition();

	return  0;
}

void Window::setMenu( const char* res_name )
{
	//���݂̃T�C�Y�擾
	int w,h;
	getSize( &w,&h );
	//���j���[�̃��[�h
	HMENU m=NULL;//���j���[��񏉊���
	if( res_name ) m = LoadMenu(GetModuleHandle(NULL), res_name );//�V�������j���[��������
	//���j���[�̃Z�b�g
	SetMenu( getHwnd(),m );
	//�T�C�Y�C��
	setSize( w,h );	
}