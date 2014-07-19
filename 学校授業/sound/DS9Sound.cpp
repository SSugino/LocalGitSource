/** @file
 * DirectSound関数インターフェースクラスの実装
 * @date 2009/08/20
 * @author 杉野
 **/

#pragma once
#pragma comment( lib, "dsound.lib")
#include "DS9Sound.h"
#include "../system/Exception.h"

using namespace sugino;
using namespace sound;
using namespace system;

/** DirectSound 
 *  ISoundからの派生
 **/
void DS9Sound::Initialize(HWND hw)
{
	if(check) system::Exception( "2回目のDirectSoundInit呼び出し" );
	//================================= オブジェクトの作成 ===========================================
	
	int ret = DirectSoundCreate( NULL, &lpds, NULL );
	if( ret != DS_OK ) throw system::Exception(" DirectSound 初期化失敗(オブジェクトの作成)");

//================================== 協調レベルの設定 ============================================ 
	
	ret = lpds->SetCooperativeLevel( hw, DSSCL_EXCLUSIVE );
	if( ret != DS_OK )throw system::Exception(" DirectSound 初期化失敗(協調レベルの設定)");

//================================== プライマリの作成 ============================================

	DSBUFFERDESC dsbd;

	ZeroMemory( &dsbd, sizeof( dsbd ) );
	dsbd.dwSize  = sizeof( dsbd );
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	
	ret = lpds->CreateSoundBuffer( &dsbd, &dsprim, NULL );
	if( ret != DS_OK )throw system::Exception(" DirectSound 初期化失敗(プライマリの作成)");

//================================= フォーマットの作成 ===========================================

	WAVEFORMATEX wfx;

	ZeroMemory( &wfx, sizeof( WAVEFORMATEX ) );
	wfx.wFormatTag	    = WAVE_FORMAT_PCM;
	wfx.nChannels	    =	 2;				// 1,2
	wfx.nSamplesPerSec  = 44100;				// 22050, 11025 等
	wfx.wBitsPerSample   =    16;
	wfx.nBlockAlign     = wfx.wBitsPerSample/8 * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec  * wfx.nBlockAlign;

	ret = dsprim->SetFormat( &wfx );
	if( ret != DS_OK )throw system::Exception(" DirectSound 初期化失敗(フォーマットの作成)");

	check = true;
}

