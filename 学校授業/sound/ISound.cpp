/** @file
 * DirectSoundインターフェースクラス実装
 * @date 2009/08/20
 * @author 杉野
 **/

#include "ISound.h"

using namespace sugino;
using namespace sound;

void ISound::PlaySE( ISoundBuffer *se )
{
	this->setSound( se );

	DSBUFFER buf = se->getDSbuf();//バッファ取得
		
	this->Play(true);
	this->setSound( NULL );
	buf->Release();
}

void ISound::StopSE( ISoundBuffer *se )
{
	DSBUFFER buf = se->getDSbuf();
	buf->Stop();
}