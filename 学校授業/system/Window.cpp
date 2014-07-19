/** @file
 * ウィンドウズオブジェクトクラスの実装
 * @date 2009/05/08
 * @author 杉野
 **/
#include "Window.h"
#include "Exception.h"

using namespace sugino;
using namespace system;

//コンストラクタ
Window::Window(){
	hwnd = NULL;
	active = FALSE;
}

//ウィンドウ作成
int Window::Create()
{
	HINSTANCE inst	= GetModuleHandle(NULL);
	char	 *c_name = "_WINDOWS_CLASS_";
	WNDCLASS	wc;								//クラスのスタイル指定

	if( !GetClassInfo( inst,c_name,&wc ) )		//未登録
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
		if( !RegisterClass( &wc ) ) throw Exception( "クラス登録失敗" );
	}

	//拡張スタイルを持つウィンドウの作成
	hwnd = CreateWindowEx( 0,c_name,"TITLE",WS_OVERLAPPEDWINDOW,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  CW_USEDEFAULT,CW_USEDEFAULT,
						  NULL,NULL,inst,NULL );
	if( hwnd == NULL ) throw Exception( "ウィンドウ作成失敗" );;
	
	//自分自身のアドレスを保持（メッセージ保存用)
	SetWindowLong( hwnd,GWL_USERDATA,(LONG)this );		//今現在のウィンドウの情報取得
	return 0;
}
//ウィンドウプロシージャ
LRESULT CALLBACK Window::WindowProc( HWND hw, UINT msg, WPARAM wp, LPARAM lp ) //メッセージ処理
{
	int ret = 0;
	//thisポインタ取得
	Window *win = ( Window * )GetWindowLong( hw,GWL_USERDATA );
	if( win ) ret = win->Evant( msg,wp,lp );				//自分のProcへ
	if( !ret ) return DefWindowProc( hw,msg,wp,lp );		//戻り値がない,×以外ならウィンドウに任せる
	return 0;												//デフォルト
}

int Window::Evant( UINT msg,WPARAM wp,LPARAM lp )
{
	switch( msg )
	{
	case WM_ACTIVATE:
		active = (wp&0xFFFF);								//アクティブ状態を取得
		if(wp>>16) active=0;								//最小化してる場合は非アクティブ
		break;
	}
	int ret;
	std::list<IWindowEvent *>::iterator i=wevent.begin();
	while( i!=wevent.end() ){								//endでなければ
		if( ret=(*i)->Event( this,msg,wp,lp ) ) return ret;
		i++;												//戻り値があればそこで終了
	}
	return 0;
}
//メッセージループ
bool Window::doEvents()
{
	MSG msg;
	while(1)
	{
		if( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )		//キューにメッセージがあるか調べあればメッセージを格納する
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

//DestroyEventの処理方法を実装
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
	AdjustWindowRectEx( &rc,getStyle(),GetMenu(hwnd)?1:0,0);		//領域作成
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
	if( ret ) throw Exception( "ウィンドウまたはクラス登録　作成失敗" );
	//初期設定
	setTitle( t );
	setSize( w,h );
	setStyle( s );
	//中央位置
	setPosition();

	return  0;
}

void Window::setMenu( const char* res_name )
{
	//現在のサイズ取得
	int w,h;
	getSize( &w,&h );
	//メニューのロード
	HMENU m=NULL;//メニュー情報初期化
	if( res_name ) m = LoadMenu(GetModuleHandle(NULL), res_name );//新しいメニュー情報を入れる
	//メニューのセット
	SetMenu( getHwnd(),m );
	//サイズ修正
	setSize( w,h );	
}