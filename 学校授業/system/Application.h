/** @file
 * �A�v���P�[�V�����N���X�̒�`
 * @date 2009/05/07
 * @author ����
 **/

#pragma once

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <stdlib.h>

#if _DEBUG || DEBUG						//�f�o�b�O���[�h���}�N���������
 #define _CRTDBG_MAP_ALLOC
 #define new_s new( _NORMAL_BLOCK,__FILE__,__LINE__ )
#else
 #define new_s new
#endif

namespace sugino{
namespace system{

		/** �A�v���P�[�V�����N���X */
		class Application
		{
		private:
			/** �C���X�^���X */
			static Application *app;	//���̂�1�����ɂ���

		protected:
			/** �R���X�g���N�^
			 *	��ԍŏ��Ɏ��̉����ꂽ�C���X�^���X��ێ�
			 **/
			Application(){ if(!app) app = this; }
		public:
			/** ���C���֐�
			 *  �h�������N���X�֐��֔��
			 **/
			virtual int Main() = 0;

			/** �A�v���P�[�V�����C���X�^���X���擾 
			 * @return �A�v���P�[�V�����̃C���X�^���X
			 **/
			inline static Application *getApplication() { return app; }
		};
	}
	
	#if _DEBUG || DEBUG					//�f�o�b�O�pPrint��`
	
	/** �f�o�b�O�v�����g */
	inline void __dprintf( const char *f,... )
	{
		char tmp[512];
		vsprintf_s(tmp,512,f,va_list(&f+1) );
		OutputDebugStringA(tmp);
	}

	/** �f�o�b�O������̏o�� */
	inline void __dputs( const char *f ){
		__dprintf( "%s\n",f );
	}
		#define dprintf __dprintf
		#define dputs	__dputs
	#else
		#define dprintf __noop
		#define dputs	__noop
	#endif
	/** �z������擾����}�N���@�������z��ȊO�͎w�肵�Ȃ� */
	#ifndef countof
	#define countof(x) (sizeof((x)) /sizeof(* (x)))
	#endif

}
