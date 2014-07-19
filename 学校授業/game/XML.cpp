/** @file
 * XMLクラスの実装
 * @date 2009/06/15
 * @author 杉野
 **/

#include "XML.h"
#include "../system/Exception.h"

using namespace sugino;
using namespace game;
using namespace system;

XMLElement::XMLElement(const char *tag, char *text)
{
	std::string stag="<";				//"<"←タグの始まり
	stag+=tag;							//"<タグ名 に
	/** 今どの文字を見ているかのポインタ */
	char *p;
	/** 目的タグ検索 */
	p = strstr( text,stag.data() );
	//開始タグ検索
	if( p==NULL ) { throw Exception("開始タグが見つかりません"); }
	/** 属性名 */
	std::string att;
	/** 属性値 */
	std::string val;
	
	char *start = p;					//開始位置の保存
	p += stag.length();					//タグ文字分自分すすめる
	
	/** テキストデータの分別処理わけ
	 * a_mode
	 * 0:属性名取得 1:["]検索/先に進める 2:属性値取得
	 */
	int a_mode=0;
	
	/** 一文字ずつ見ていく */
	while(1)
	{
		if( *p== '>' ) { break; }				//閉じカッコで抜ける	
		if( *p== ' ' ) { p++;continue; }		//スペースなら参照先を進めてループの先頭に戻る(p++してもこのまま進んでしまうのでスペースが連続であると困る)
		
		// 属性名抽出
		if( a_mode==0 )
		{
			if( *p== '=' )
			{
				p++;
				a_mode=1;
				continue;
			}
			att += *p;				//属性名をぶち込む
			p++;					//次参照
		}
		
		//「属性名」と「属性値」の間の判定「"」の処理は
		if( a_mode==1 )
		{
			if( *p== '"' ) 
			{
				p++;
				a_mode=2;		
				continue;
			}
		}

		// 属性値を抽出
		if( a_mode==2 )
		{
			if( *p== '"' ) 
			{
				attribute[att]=val;
				att="";
				val="";
				p++;
				a_mode=0;
				continue;
			}
			val += *p;						//属性値をぶっこむ
			p++;
		}
	}
	char *p2=p;								//今の場所を保存
	//終了タグ検索
	//if( *(p - 2)!='/')						//1行タグでなければ 現在参照している場所 '>'の一つ前
	if( *(p - 1)!='/')
	{
		std::string etag="</";				// </←終了タグの始まり
		etag+=tag;							// </タグ名
		p2 = strstr( p,etag.data() );	//目的タグ検索  </タグ名 が付くもの検索
		if( p2==NULL ) { throw Exception("終了タグが見つかりません"); }	
		int size=p2-start;						//文字数の差取得
		data.resize( size+1 );				//　">"を含めるために+1
		strncpy( data,p,size );				//取得した文字数コピー
		data[size]=0;						//最後にNULL文字を入れておく
		
		p2+=etag.length()+1;				//"</タグ名"　で検索しているので ">"を含めるために+1
	}else{									//1行タグでも
		data.resize(1);						//初期化しておく
		data[0]=0;
	}
	length = p2-start;						//全体の大きさ
}
