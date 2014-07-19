/** @file
 * フォント画像表示クラスの定義
 * @date 2009/05/19
 * @author 杉野
 **/

#pragma once

#include <Windows.h>
#include "../graphics/IRender.h"
#include "../system/Template.h"


namespace sugino{
namespace game{

	/** 先にimg=createImage(～)してからの表示（画像の共有クラス） */
	class FontImageOutput
	{
	private:
		graphics::IRender *render;
		graphics::IImage *img;
	public:
		FontImageOutput( graphics::IRender *r,graphics::IImage *i)
		{
			render =r;
			img =i;
		}
		/** 文字列表示
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		: 
		 * @param [in] int add_w	: 文字間隔幅
		 * @param [in] int add_h	: 文字間隔高(改行)
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの
		 * @param [in] int pw		: パターン幅
		 * @param [in] int ph		: パターン高
		 * @param [in] char *str	: 文字列
		 * @param [in] ...			: その他
		 **/
		void Print( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *str, ... );
		
		/** 文字列表示
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		: 
		 * @param [in] int add_w	: 文字間隔幅
		 * @param [in] int add_h	: 文字間隔高(改行)
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの
		 * @param [in] int pw		: パターン幅
		 * @param [in] int ph		: パターン高
		 * @param [in] char *m		: 文字列
		 **/
		void Puts( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *m );
		
		/** １文字表示
		 *
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		:
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの	
		 * @param [in] int w		: パターン幅
		 * @param [in] int h		: パターン高
		 * @param [in] int str_pat	: 文字列パターン番号
		 **/
		void Putchar(int x,int y,int mod,int mul,int w,int h,int str_pat );
	};
	/** 画像の読み込みも行うクラス一個一個画像を管理 */
	/** 自分でデータを持つ */
	class FontImageOutput2
	{
	private:
		graphics::IRender *render;
		/** 単体で管理するのでAutoPtrつけておく */
		system::AutoPtr<graphics::IImage> img;
	public:
		/** 画像の読み込み */
		FontImageOutput2( graphics::IRender* r,char* name )
		{
			render = r; //レンダリング表示するもの
			img = r->createImage( name );//読み込み
		}
		
		/** 文字列表示
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		: 
		 * @param [in] int add_w	: 文字間隔幅
		 * @param [in] int add_h	: 文字間隔高(改行)
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの
		 * @param [in] int pw		: パターン幅
		 * @param [in] int ph		: パターン高
		 * @param [in] char *str	: 文字列
		 * @param [in] ...			: その他
		 **/
		void Print( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *str, ... );
		
		/** 文字列表示
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		: 
		 * @param [in] int add_w	: 文字間隔幅
		 * @param [in] int add_h	: 文字間隔高(改行)
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの
		 * @param [in] int pw		: パターン幅
		 * @param [in] int ph		: パターン高
		 * @param [in] char *m		: 文字列
		 **/
		void Puts( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *m );
		
		/** １文字表示
		 *
		 * @param [in] int x		: 表示座標
		 * @param [in] int y		:
		 * @param [in] int mod		: %で割るもの
		 * @param [in] int mul		: /で割るもの	
		 * @param [in] int w		: パターン幅
		 * @param [in] int h		: パターン高
		 * @param [in] int str_pat	: 文字列パターン番号
		 **/
		void Putchar(int x,int y,int mod,int mul,int w,int h,int str_pat );
	};
}}