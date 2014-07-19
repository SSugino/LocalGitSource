/** @file
 * DirectSound�֐��C���^�[�t�F�[�X�N���X�̒�`
 * @date 2009/08/20
 * @author ����
 **/

#pragma once

#include "Sound.h"
#include "../system/Window.h"
#include "../system/Exception.h"
#pragma comment( lib, "dsound.lib")
#include < mmsystem.h >

namespace sugino{
/** �T�E���h�̗̈� */
namespace sound{
		/**�@�_�C���N�g�T�E���h�N���X : IRender����̔h��
		 * �@DirectMusic���g�����Đ����s��
		 **/
		class DS9Sound : public ISound
		{
		private:
			/** DirectSound �������������� */
			int check;

			/**  �_�C���N�g�T�E���h�I�u�W�F�N�g */
			LPDIRECTSOUND lpds;

			/** �v���C�}���o�b�t�@ */
			LPDIRECTSOUNDBUFFER dsprim;	
			
		public:
			/** �R���X�g���N�^ */
			DS9Sound(){ check=false;lpds = NULL;dsprim=NULL; }

			/** �f�X�g���N�^ */
			virtual ~DS9Sound(){ Uninitialize(); }

			virtual void Initialize( HWND hw );

			/** DirectSound�̉�� */
			virtual void Uninit() = 0;

			/** �h���̓ǂݍ��� */
			ISoundBuffer *createSound( const char *name );
			
			/** �h���̌��ʉ��Z�b�g */
			void setSound( ISoundBuffer *src );//���̐ݒ�

			void Play( bool loop );

			void Stop( int end  );
	
			void Uninitialize(){
				if(lpds){ lpds->Release();lpds=NULL; }
				if( dsprim ){ dsprim->Release();dsprim=NULL; }
			}
		};

	}
}