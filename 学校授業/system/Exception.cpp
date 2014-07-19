/** @file
 * 例外処理の関数
 * @date 2009/05/07
 * @author 杉野
 **/
#include <windows.h>
#include "Exception.h"

using namespace sugino;
using namespace system;

//コンストラクタ:例外発生時に必ず呼ばれる
Exception::Exception(const char *s) : error(s)
{
	//デバッグ実行中なら一時停止して呼び出し履歴を見てトレースする
	if( IsDebuggerPresent() ) __debugbreak();
}