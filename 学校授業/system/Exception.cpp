/** @file
 * ��O�����̊֐�
 * @date 2009/05/07
 * @author ����
 **/
#include <windows.h>
#include "Exception.h"

using namespace sugino;
using namespace system;

//�R���X�g���N�^:��O�������ɕK���Ă΂��
Exception::Exception(const char *s) : error(s)
{
	//�f�o�b�O���s���Ȃ�ꎞ��~���ČĂяo�����������ăg���[�X����
	if( IsDebuggerPresent() ) __debugbreak();
}