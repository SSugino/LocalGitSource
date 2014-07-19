/** @file
 * DirectSound�֐��C���^�[�t�F�[�X�N���X�̎���
 * @date 2009/08/20
 * @author ����
 **/

#pragma once
#pragma comment( lib, "dsound.lib")
#include "DS9Sound.h"
#include "../system/Exception.h"

using namespace sugino;
using namespace sound;
using namespace system;

/** DirectSound 
 *  ISound����̔h��
 **/
void DS9Sound::Initialize(HWND hw)
{
	if(check) system::Exception( "2��ڂ�DirectSoundInit�Ăяo��" );
	//================================= �I�u�W�F�N�g�̍쐬 ===========================================
	
	int ret = DirectSoundCreate( NULL, &lpds, NULL );
	if( ret != DS_OK ) throw system::Exception(" DirectSound ���������s(�I�u�W�F�N�g�̍쐬)");

//================================== �������x���̐ݒ� ============================================ 
	
	ret = lpds->SetCooperativeLevel( hw, DSSCL_EXCLUSIVE );
	if( ret != DS_OK )throw system::Exception(" DirectSound ���������s(�������x���̐ݒ�)");

//================================== �v���C�}���̍쐬 ============================================

	DSBUFFERDESC dsbd;

	ZeroMemory( &dsbd, sizeof( dsbd ) );
	dsbd.dwSize  = sizeof( dsbd );
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	
	ret = lpds->CreateSoundBuffer( &dsbd, &dsprim, NULL );
	if( ret != DS_OK )throw system::Exception(" DirectSound ���������s(�v���C�}���̍쐬)");

//================================= �t�H�[�}�b�g�̍쐬 ===========================================

	WAVEFORMATEX wfx;

	ZeroMemory( &wfx, sizeof( WAVEFORMATEX ) );
	wfx.wFormatTag	    = WAVE_FORMAT_PCM;
	wfx.nChannels	    =	 2;				// 1,2
	wfx.nSamplesPerSec  = 44100;				// 22050, 11025 ��
	wfx.wBitsPerSample   =    16;
	wfx.nBlockAlign     = wfx.wBitsPerSample/8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec  * wfx.nBlockAlign;

	ret = dsprim->SetFormat( &wfx );
	if( ret != DS_OK )throw system::Exception(" DirectSound ���������s(�t�H�[�}�b�g�̍쐬)");

	check = true;
}

