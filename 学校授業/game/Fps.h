/** @file
 * フレームレートクラスの定義
 * @date 2009/05/18
 * @author 杉野
 **/

#pragma once

#include <Windows.h>
#include <stdio.h>
#include "../system/Window.h"

namespace sugino{
namespace game{

	/** 時間管理クラス */
	class Fps{
	private:

		/** 固定するフレームレート */
		DWORD wfps; 

		/**  フレームレート */
		DWORD fps;

		DWORD count;//カウント

		DWORD time;
	public:
		/* コンストラクタ **/
		Fps(){ 
			time =timeGetTime(); 
		}
		/* デストラクタ **/
		~Fps(){}
		
		void setWait( int f ){ wfps = (1000<<8)/f; }
		void Wait();				 //固定
		void Count();				 //カウント計算
		/** フレームレート取得 */
		int  getFps() { Count();return fps; }
		
	};
}}