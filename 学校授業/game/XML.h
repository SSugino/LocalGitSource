/** @file
 * XML風クラスの定義
 * @date 2009/06/15
 * @author 杉野
 **/

#pragma once

#include "../system/Exception.h"
#include "../system/Template.h"
#include <map>
#include <string>


namespace sugino{
namespace game{
	
	using namespace std;

	/** XML風要素のクラス */
	class XMLElement
	{
	private:
		/** 属性 */
		map< string,string > attribute;

		/** テキストデータ */

		system::AutoArray<char>		data;
		/** 全体サイズ */
		int length;
	public:
		/** XML読み込み タグと属性の検索
		 * @param[in] const char *tag	: タグ名
		 * @param[in] const char *text	: 文字列
		 **/
		XMLElement( const char *tag, char *text );
		
		/** ゲッター関数 文字列先頭アドレス取得
		 * @return &text[0]	:テキストの先頭アドレス
		 **/
		
		char *getText(){ return &data[0]; }
		/** ゲッター関数 全体サイズ取得
		 * @return length	: 全体サイズ
		 **/
		
		int getLength(){ return length; }

		/** 配列オペレーター
		 * @param [in] const char *key :キー
		 * @return 属性のデータ
		 **/
		const char * operator[]( const char *key )
		{
			return attribute[key].data();
		}

		/** 配列オペレーター
		 * @param [in] const char *key :キー
		 * @return 属性の最後のデータ
		 **/
		bool isAttribute( const char *key )
		{
			return attribute.find(key)!=attribute.end(); 
		}
	};
}
}