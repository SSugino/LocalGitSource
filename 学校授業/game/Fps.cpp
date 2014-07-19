/** @file
 * フレームレートクラス実装
 * @date 2009/05/18
 * @author 杉野
 **/

#include "Fps.h"
#include <Windows.h>

using namespace sugino;
using namespace game;
using namespace system;


void Fps::Wait()
{
	const DWORD st = (timeGetTime()&0xFFFF);
	static DWORD fs;
	
	if((fs>>8)-st<0x8000)fs=st<<8;
	fs=(fs+wfps)&0xFFFFFF;
	if((fs>>8)-st>=0x8000)fs=st<<8;
	while((fs>>8)-(timeGetTime()&0xFFFF)<0x8000)Sleep(1);
}

void Fps::Count()
{	
	if(time+1000<=timeGetTime())//現在の時間がtimeより1秒経過したら
	{
		time+=1000;
		fps=count;
		count=0;
	}
	count++;
}