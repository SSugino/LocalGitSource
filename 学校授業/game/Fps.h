/** @file
 * �t���[�����[�g�N���X�̒�`
 * @date 2009/05/18
 * @author ����
 **/

#pragma once

#include <Windows.h>
#include <stdio.h>
#include "../system/Window.h"

namespace sugino{
namespace game{

	/** ���ԊǗ��N���X */
	class Fps{
	private:

		/** �Œ肷��t���[�����[�g */
		DWORD wfps; 

		/**  �t���[�����[�g */
		DWORD fps;

		DWORD count;//�J�E���g

		DWORD time;
	public:
		/* �R���X�g���N�^ **/
		Fps(){ 
			time =timeGetTime(); 
		}
		/* �f�X�g���N�^ **/
		~Fps(){}
		
		void setWait( int f ){ wfps = (1000<<8)/f; }
		void Wait();				 //�Œ�
		void Count();				 //�J�E���g�v�Z
		/** �t���[�����[�g�擾 */
		int  getFps() { Count();return fps; }
		
	};
}}