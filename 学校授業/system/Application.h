/** @file
 * アプリケーションクラスの定義
 * @date 2009/05/07
 * @author 杉野
 **/

#pragma once

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <stdlib.h>

#if _DEBUG || DEBUG						//デバッグモードかマクロがあれば
 #define _CRTDBG_MAP_ALLOC
 #define new_s new( _NORMAL_BLOCK,__FILE__,__LINE__ )
#else
 #define new_s new
#endif

namespace sugino{
namespace system{

		/** アプリケーションクラス */
		class Application
		{
		private:
			/** インスタンス */
			static Application *app;	//実体を1つだけにする

		protected:
			/** コンストラクタ
			 *	一番最初に実体化されたインスタンスを保持
			 **/
			Application(){ if(!app) app = this; }
		public:
			/** メイン関数
			 *  派生したクラス関数へ飛ぶ
			 **/
			virtual int Main() = 0;

			/** アプリケーションインスタンスを取得 
			 * @return アプリケーションのインスタンス
			 **/
			inline static Application *getApplication() { return app; }
		};
	}
	
	#if _DEBUG || DEBUG					//デバッグ用Print定義
	
	/** デバッグプリント */
	inline void __dprintf( const char *f,... )
	{
		char tmp[512];
		vsprintf_s(tmp,512,f,va_list(&f+1) );
		OutputDebugStringA(tmp);
	}

	/** デバッグ文字列の出力 */
	inline void __dputs( const char *f ){
		__dprintf( "%s\n",f );
	}
		#define dprintf __dprintf
		#define dputs	__dputs
	#else
		#define dprintf __noop
		#define dputs	__noop
	#endif
	/** 配列個数を取得するマクロ　ただし配列以外は指定しない */
	#ifndef countof
	#define countof(x) (sizeof((x)) /sizeof(* (x)))
	#endif

}
