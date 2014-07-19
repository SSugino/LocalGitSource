/** @file
 * DirectSound�C���^�[�t�F�[�X�N���X����
 * @date 2009/08/20
 * @author ����
 **/

#include "ISound.h"

using namespace sugino;
using namespace sound;

void ISound::PlaySE( ISoundBuffer *se )
{
	this->setSound( se );

	DSBUFFER buf = se->getDSbuf();//�o�b�t�@�擾
		
	this->Play(true);
	this->setSound( NULL );
	buf->Release();
}

void ISound::StopSE( ISoundBuffer *se )
{
	DSBUFFER buf = se->getDSbuf();
	buf->Stop();
}