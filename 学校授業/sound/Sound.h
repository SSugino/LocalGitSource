/** @file
 * DirectSoundクラスの定義
 * @date 2009/08/20
 * @author 杉野
 **/

#pragma once

#define DIRECTSOUND_VERSION 0x900	//バージョン９を使用可能にする
#include < dsound.h >
#include "../system/Window.h"
typedef LPDIRECTSOUNDBUFFER DSBUFFER;

namespace sugino{
	namespace sound{
		/** ISoundBuffer インターフェイス */
		class ISoundBuffer
		{
		public:
			/** 音量変更
			 * @param  [in] :	
			 */
			virtual void Volume( int vol ) = 0;

			/** 再生
			 * @param  [in] loop : ループするかしないか	
			 */
			virtual void Play( bool loop ) = 0;

			/** 停止 */
			virtual void Stop( int end ) = 0;

			virtual inline LPDIRECTSOUNDBUFFER getDSbuf() = 0;
			
			/** ID取得 */
			virtual int getID() = 0;
		};
		
		/** 効果音を扱うインターフェース */
		class ISound{
		public:
			virtual ~ISound(){};
			/** DirectSoundの初期化 */
			virtual void Init( HWND hw ) = 0;
			
			/** DirectSoundの解放 */
			virtual void Uninit() = 0;

			/** 読み込み */
			virtual ISoundBuffer *createSound( const char *name ) = 0;
			
			/** 効果音セット */
			virtual void setSound( ISoundBuffer *src ) = 0;//音の設定
		};
	}
}