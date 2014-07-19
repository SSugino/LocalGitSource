/** @file
 * WinMain
 * @date 2009/05/07
 * @author ����
 **/
#include "Application.h"
#include "Exception.h"
#include <mmsystem.h>
#pragma comment ( lib,"winmm.lib" )

using namespace sugino;
using namespace system;

Application *Application::app = NULL;

/** Windows �G���g���[�|�C���g */
int WINAPI WinMain( HINSTANCE hinst,HINSTANCE fprev,
				   char *cmdline,int hshow )
{
	int ret = 0;

	timeBeginPeriod(1);													//timeGetTime�̐��x����
	try{
		Application *app = Application::getApplication();				//�������g�̎��̂�����
		
		if( !app )
		{ 
			throw Exception( "�A�v���P�[�V�����̎��̂��쐬����Ă��܂���" );
		}
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF );	//���������[�N�o�͐ݒ�
		ret = app->Main();												//���̂����݂��Ă����Main���Ă�
	
	}catch( const Exception &e ){
		dputs( "\n[��O���������܂���]" );
		dputs( e.getError() );
		MessageBox( NULL,e.getError(),"��O�ɂ�鋭���I��",MB_OK );
		ret = -1;
	}

	timeEndPeriod(1);													//���x�߂�
	return ret;
}