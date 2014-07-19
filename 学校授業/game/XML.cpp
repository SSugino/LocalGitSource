/** @file
 * XML�N���X�̎���
 * @date 2009/06/15
 * @author ����
 **/

#include "XML.h"
#include "../system/Exception.h"

using namespace sugino;
using namespace game;
using namespace system;

XMLElement::XMLElement(const char *tag, char *text)
{
	std::string stag="<";				//"<"���^�O�̎n�܂�
	stag+=tag;							//"<�^�O�� ��
	/** ���ǂ̕��������Ă��邩�̃|�C���^ */
	char *p;
	/** �ړI�^�O���� */
	p = strstr( text,stag.data() );
	//�J�n�^�O����
	if( p==NULL ) { throw Exception("�J�n�^�O��������܂���"); }
	/** ������ */
	std::string att;
	/** �����l */
	std::string val;
	
	char *start = p;					//�J�n�ʒu�̕ۑ�
	p += stag.length();					//�^�O���������������߂�
	
	/** �e�L�X�g�f�[�^�̕��ʏ����킯
	 * a_mode
	 * 0:�������擾 1:["]����/��ɐi�߂� 2:�����l�擾
	 */
	int a_mode=0;
	
	/** �ꕶ�������Ă��� */
	while(1)
	{
		if( *p== '>' ) { break; }				//���J�b�R�Ŕ�����	
		if( *p== ' ' ) { p++;continue; }		//�X�y�[�X�Ȃ�Q�Ɛ��i�߂ă��[�v�̐擪�ɖ߂�(p++���Ă����̂܂ܐi��ł��܂��̂ŃX�y�[�X���A���ł���ƍ���)
		
		// ���������o
		if( a_mode==0 )
		{
			if( *p== '=' )
			{
				p++;
				a_mode=1;
				continue;
			}
			att += *p;				//���������Ԃ�����
			p++;					//���Q��
		}
		
		//�u�������v�Ɓu�����l�v�̊Ԃ̔���u"�v�̏�����
		if( a_mode==1 )
		{
			if( *p== '"' ) 
			{
				p++;
				a_mode=2;		
				continue;
			}
		}

		// �����l�𒊏o
		if( a_mode==2 )
		{
			if( *p== '"' ) 
			{
				attribute[att]=val;
				att="";
				val="";
				p++;
				a_mode=0;
				continue;
			}
			val += *p;						//�����l���Ԃ�����
			p++;
		}
	}
	char *p2=p;								//���̏ꏊ��ۑ�
	//�I���^�O����
	//if( *(p - 2)!='/')						//1�s�^�O�łȂ���� ���ݎQ�Ƃ��Ă���ꏊ '>'�̈�O
	if( *(p - 1)!='/')
	{
		std::string etag="</";				// </���I���^�O�̎n�܂�
		etag+=tag;							// </�^�O��
		p2 = strstr( p,etag.data() );	//�ړI�^�O����  </�^�O�� ���t�����̌���
		if( p2==NULL ) { throw Exception("�I���^�O��������܂���"); }	
		int size=p2-start;						//�������̍��擾
		data.resize( size+1 );				//�@">"���܂߂邽�߂�+1
		strncpy( data,p,size );				//�擾�����������R�s�[
		data[size]=0;						//�Ō��NULL���������Ă���
		
		p2+=etag.length()+1;				//"</�^�O��"�@�Ō������Ă���̂� ">"���܂߂邽�߂�+1
	}else{									//1�s�^�O�ł�
		data.resize(1);						//���������Ă���
		data[0]=0;
	}
	length = p2-start;						//�S�̂̑傫��
}
