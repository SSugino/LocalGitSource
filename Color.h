/** @file
 * オブジェクトカラー関数クラスの定義
 * @date 2009/06/03
 * @author 杉野
 **/
#pragma once
#include "../system/Template.h"
#include "Float.h"

namespace sugino{
namespace math{
	
	/** カラークラス(ベクトルと似ているのでnamespace mathで定義) */
	class Color{
	public:
		float a;								//アルファ値
		float r;								//赤要素
		float g;								//緑要素
		float b;								//青要素

		Color(){}								//デフォルトコンストラクタ
		/** 初期化
		 * @param [in] float rr : メンバ変数 r への初期化
		 * @param [in] float gg : メンバ変数 g への初期化
		 * @param [in] float bb : メンバ変数 b への初期化
		 * @param [in] float aa : メンバ変数 a への初期化
		 **/
		Color( float rr,float gg,float bb,float aa=1.0 ) :  r(rr) ,g(gg) ,b(bb),a(aa)  {};
		
		/** 色の取り出し 各色要素のfloat変換
		 * @param [in] unsigned long col	:色(0x00000000～0xffFFFFFF)
		 **/
		Color( unsigned long col )
		{
			int ia = (col>>24)&0xFF;					//各色取り出し
			int ir = (col>>16)&0xFF;
			int ig = (col>>8)&0xFF;
			int ib = col&0xFF;

			this->a = ia /255.0f;						//floatに直す
			this->r = ir /255.0f;
			this->g = ig /255.0f;
			this->b = ib /255.0f;
		};
		//---- 四則演算 -----
		/** 加算 */
		inline Color operator + ( const Color &c ) const{ return Color( r+c.r, g+c.g, b+c.b, a+c.a ); }	
		/** 減算 */
		inline Color operator - ( const Color &c ) const{ return Color( r-c.r, g-c.g, b-c.b, a-c.a ); }
		/** 乗算 */
		inline Color operator * ( const float s ) const{ return Color( r*s, g*s, b*s, a*s ); }		
		/** 除算 */
		inline Color operator / ( const float s ) const{ return *this * Float::inv(s); }
		
		/** 色計算用 */
		inline Color operator * ( Color &c ) { return Color( r*c.r, g*c.g, b*c.b, a*c.a ); }
		
		/** 代入演算 */
		inline Color &operator += ( const Color &c ) { a+=c.a; r+=c.r; g+=c.g; b+=c.b; return *this; }
		inline Color &operator -= ( const Color &c ) { a-=c.a; r-=c.r; g-=c.g; b-=c.b; return *this; }
		inline Color &operator *= ( const float s ) { a*=s; r*=s; g*=s; b*=s; return *this; }
		inline Color &operator /= ( const float s ) { return *this *=Float::inv(s); }		//逆数かける
		
		/* 符号 */
		Color operator + () const { return *this; }
		
		/** 符号反転 */
		Color operator - () const { return Color( -r,-g,-b,-a ) ; }

		/** キャスト */
		inline operator unsigned long() const 
		{
			//0～255までの範囲に
			int ia = (int)(a*255);
			if(ia<0)ia=0;
			if(ia>255)ia=255;
			
			int ir = (int)(r*255);
			if(ir<0)ir=0;
			if(ir>255)ir=255;
			
			int ig = (int)(g*255);
			if(ig<0)ig=0;
			if(ig>255)ig=255;
			
			int ib = (int)(b*255);
			if(ib<0)ib=0;
			if(ib>255)ib=255;
			
			return(  (ia<<24)|(ir<<16)|(ig<<8)|(ib) );			//DWORD値(32bit)の合成
		}
	};
}}