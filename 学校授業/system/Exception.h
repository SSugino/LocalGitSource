/** @file
 * 例外処理クラスの定義
 * @date 2009/05/07
 * @author 杉野
 **/
#pragma once

#include <string>

namespace sugino{
namespace system{
	/** 例外処理用クラス */
	class Exception
	{
	private:
		/** エラー文字を格納 */
		std::string error;
	public:
		/** 例外発生文字表示
		 * @param [in]	char*	*s:例外発生文
		 **/
		Exception( const char *s );
		
		/** エラー内容を取得 
		 * @return エラー内容の文字列
		 **/
		const char *getError() const { return error.data(); }
	};
	//例外クラス定義用のマクロ
	#define CreateException( NAME,MSG ) class NAME : public Exception \
	{ public: ##NAME##()	:	Exception(##MSG##) {} }
}}