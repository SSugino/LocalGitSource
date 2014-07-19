/** @file
 * XML���N���X�̒�`
 * @date 2009/06/15
 * @author ����
 **/

#pragma once

#include "../system/Exception.h"
#include "../system/Template.h"
#include <map>
#include <string>


namespace sugino{
namespace game{
	
	using namespace std;

	/** XML���v�f�̃N���X */
	class XMLElement
	{
	private:
		/** ���� */
		map< string,string > attribute;

		/** �e�L�X�g�f�[�^ */

		system::AutoArray<char>		data;
		/** �S�̃T�C�Y */
		int length;
	public:
		/** XML�ǂݍ��� �^�O�Ƒ����̌���
		 * @param[in] const char *tag	: �^�O��
		 * @param[in] const char *text	: ������
		 **/
		XMLElement( const char *tag, char *text );
		
		/** �Q�b�^�[�֐� ������擪�A�h���X�擾
		 * @return &text[0]	:�e�L�X�g�̐擪�A�h���X
		 **/
		
		char *getText(){ return &data[0]; }
		/** �Q�b�^�[�֐� �S�̃T�C�Y�擾
		 * @return length	: �S�̃T�C�Y
		 **/
		
		int getLength(){ return length; }

		/** �z��I�y���[�^�[
		 * @param [in] const char *key :�L�[
		 * @return �����̃f�[�^
		 **/
		const char * operator[]( const char *key )
		{
			return attribute[key].data();
		}

		/** �z��I�y���[�^�[
		 * @param [in] const char *key :�L�[
		 * @return �����̍Ō�̃f�[�^
		 **/
		bool isAttribute( const char *key )
		{
			return attribute.find(key)!=attribute.end(); 
		}
	};
}
}