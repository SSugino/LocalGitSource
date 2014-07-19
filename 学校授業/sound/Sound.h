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
			/** ���ʕύX
			 * @param  [in] :	
			 */
			virtual void Volume( int vol ) = 0;

			/** �Đ�
			 * @param  [in] loop : ���[�v���邩���Ȃ���	
			 */
			virtual void Play( bool loop ) = 0;

			/** ��~ */
			virtual void Stop( int end ) = 0;

			virtual inline LPDIRECTSOUNDBUFFER getDSbuf() = 0;
			
			/** ID�擾 */
			virtual int getID() = 0;
		};
		
		/** ���ʉ��������C���^�[�t�F�[�X */
		class ISound{
		public:
			virtual ~ISound(){};
			/** DirectSound�̏����� */
			virtual void Init( HWND hw ) = 0;
			
			/** DirectSound�̉�� */
			virtual void Uninit() = 0;

			/** �ǂݍ��� */
			virtual ISoundBuffer *createSound( const char *name ) = 0;
			
			/** ���ʉ��Z�b�g */
			virtual void setSound( ISoundBuffer *src ) = 0;//���̐ݒ�
		};
	}
}