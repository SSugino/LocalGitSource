/** @file
 * WinMain
 * @date 2009/05/07
 * @author 杉野
 **/
#include "Application.h"
#include "Exception.h"
#include <mmsystem.h>
#pragma comment ( lib,"winmm.lib" )

using namespace sugino;
using namespace system;

Application *Application::app = NULL;

/** Windows エントリーポイント */
int WINAPI WinMain( HINSTANCE hinst,HINSTANCE fprev,
				   char *cmdline,int hshow )
{
	int ret = 0;

	timeBeginPeriod(1);													//timeGetTimeの精度向上
	try{
		Application *app = Application::getApplication();				//自分自身の実体を持つ
		
		if( !app )
		{ 
			throw Exception( "アプリケーションの実体が作成されていません" );
		}
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF );	//メモリリーク出力設定
		ret = app->Main();												//実体が存在していればMainを呼ぶ
	
	}catch( const Exception &e ){
		dputs( "\n[例外が発生しました]" );
		dputs( e.getError() );
		MessageBox( NULL,e.getError(),"例外による強制終了",MB_OK );
		ret = -1;
	}

	timeEndPeriod(1);													//精度戻す
	return ret;
}