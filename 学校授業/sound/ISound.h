/** @file
 * DirectSound�N���X�̒�`
 * @date 2009/08/20
 * @author ����
 **/

#pragma once

#define DIRECTSOUND_VERSION 0x900	//�o�[�W�����X���g�p�\�ɂ���
#include < dsound.h >
#include "../system/Window.h"
typedef LPDIRECTSOUNDBUFFER DSBUFFER;

namespace sugino{
	namespace sound{
		/** ISoundBuffer �C���^�[�t�F�C�X */
		class ISoundBuffer
		{
		public:
			~ISoundBuffer(){};
			/** ���ʕύX
			 * @param  [in] :	
			 */
			virtual void getVol() = 0;
		
			/** ID�擾 */
			virtual int getID() = 0;

			virtual inline LPDIRECTSOUNDBUFFER getDSbuf() = 0;
		};
		
		/** ���ʉ��������C���^�[�t�F�[�X */
		class ISound{
		public:
			virtual ~ISound(){};
			/** DirectSound�̏����� */
			virtual void Initialize( HWND hw ) = 0;
			
			/** DirectSound�̉�� */
			virtual void Uninit() = 0;

			/** �ǂݍ��� */
			virtual ISoundBuffer *createSound( const char *name ) = 0;
			
			/** ���ʉ��Z�b�g */
			virtual void setSound( ISoundBuffer *src ) = 0;//���̐ݒ�

			virtual void Play( bool loop ) = 0;

			virtual void Stop( int end  ) = 0;

			void PlaySE( ISoundBuffer *se );
			void StopSE( ISoundBuffer *se );
		};
	}
}