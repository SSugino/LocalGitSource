/** @file
 * フォント画像クラス実装
 * @date 2009/05/19
 * @author 杉野
 **/

#include "Font.h"
#include <stdio.h>

using namespace sugino;
using namespace game;
using namespace system;

void FontImageOutput2::Print( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *str, ... )
{
	char m[512];
	vsprintf( m,str,(char *)(&str+1) );
	Puts( x, y, add_w, add_h, mod, mul, pw, ph, m );
}

void FontImageOutput2::Puts( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *m )
{
	int xx=x;
	while(*m)
	{
		if( *m=='\n' ){ y+=add_h;x=xx;m++;continue;}
		Putchar( x, y, mod, mul, pw, ph, *m - ' ' );
		x+=add_w;
		m++;
	}
}

/** 一文字 */
void FontImageOutput2::Putchar(int x,int y,int mod,int mul,int w,int h,int str_pat )
{
	int px;
	int py;

	px=( str_pat % mod ) * w;
	py=( str_pat / mul ) * h;

	render->drawImage( x,y,w,h,img,px,py );
}
