/** @file
 * DirectSound関数インターフェースクラスの定義
 * @date 2009/08/20
 * @author 杉野
 **/

#pragma once

#include "Sound.h"
#include "../system/Window.h"
#include "../system/Exception.h"
#pragma comment( lib, "dsound.lib")
#include < mmsystem.h >

namespace sugino{
/** サウンドの領域 */
namespace sound{
		/**　ダイレクトサウンドクラス : IRenderからの派生
		 * 　DirectMusicを使った再生を行う
		 **/
		class DS9Sound : public ISound
		{
		private:
			/** DirectSound を初期化したか */
			int check;

			/**  ダイレクトサウンドオブジェクト */
			LPDIRECTSOUND lpds;

			/** プライマリバッファ */
			LPDIRECTSOUNDBUFFER dsprim;	
			
		public:
			/** コンストラクタ */
			DS9Sound(){ check=false;lpds = NULL;dsprim=NULL; }

			/** デストラクタ */
			virtual ~DS9Sound(){ Uninitialize(); }

			virtual void Initialize( HWND hw );

			/** DirectSoundの解放 */
			virtual void Uninit() = 0;

			/** 派生の読み込み */
			ISoundBuffer *createSound( const char *name );
			
			/** 派生の効果音セット */
			void setSound( ISoundBuffer *src );//音の設定

			void Play( bool loop );

			void Stop( int end  );
	
			void Uninitialize(){
				if(lpds){ lpds->Release();lpds=NULL; }
				if( dsprim ){ dsprim->Release();dsprim=NULL; }
			}
		};

	}
}