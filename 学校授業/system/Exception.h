/** @file
 * ��O�����N���X�̒�`
 * @date 2009/05/07
 * @author ����
 **/
#pragma once

#include <string>

namespace sugino{
namespace system{
	/** ��O�����p�N���X */
	class Exception
	{
	private:
		/** �G���[�������i�[ */
		std::string error;
	public:
		/** ��O���������\��
		 * @param [in]	char*	*s:��O������
		 **/
		Exception( const char *s );
		
		/** �G���[���e���擾 
		 * @return �G���[���e�̕�����
		 **/
		const char *getError() const { return error.data(); }
	};
	//��O�N���X��`�p�̃}�N��
	#define CreateException( NAME,MSG ) class NAME : public Exception \
	{ public: ##NAME##()	:	Exception(##MSG##) {} }
}}