/** @file
 * �t�H���g�摜�\���N���X�̒�`
 * @date 2009/05/19
 * @author ����
 **/

#pragma once

#include <Windows.h>
#include "../graphics/IRender.h"
#include "../system/Template.h"


namespace sugino{
namespace game{

	/** ���img=createImage(�`)���Ă���̕\���i�摜�̋��L�N���X�j */
	class FontImageOutput
	{
	private:
		graphics::IRender *render;
		graphics::IImage *img;
	public:
		FontImageOutput( graphics::IRender *r,graphics::IImage *i)
		{
			render =r;
			img =i;
		}
		/** ������\��
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		: 
		 * @param [in] int add_w	: �����Ԋu��
		 * @param [in] int add_h	: �����Ԋu��(���s)
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������
		 * @param [in] int pw		: �p�^�[����
		 * @param [in] int ph		: �p�^�[����
		 * @param [in] char *str	: ������
		 * @param [in] ...			: ���̑�
		 **/
		void Print( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *str, ... );
		
		/** ������\��
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		: 
		 * @param [in] int add_w	: �����Ԋu��
		 * @param [in] int add_h	: �����Ԋu��(���s)
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������
		 * @param [in] int pw		: �p�^�[����
		 * @param [in] int ph		: �p�^�[����
		 * @param [in] char *m		: ������
		 **/
		void Puts( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *m );
		
		/** �P�����\��
		 *
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		:
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������	
		 * @param [in] int w		: �p�^�[����
		 * @param [in] int h		: �p�^�[����
		 * @param [in] int str_pat	: ������p�^�[���ԍ�
		 **/
		void Putchar(int x,int y,int mod,int mul,int w,int h,int str_pat );
	};
	/** �摜�̓ǂݍ��݂��s���N���X���摜���Ǘ� */
	/** �����Ńf�[�^������ */
	class FontImageOutput2
	{
	private:
		graphics::IRender *render;
		/** �P�̂ŊǗ�����̂�AutoPtr���Ă��� */
		system::AutoPtr<graphics::IImage> img;
	public:
		/** �摜�̓ǂݍ��� */
		FontImageOutput2( graphics::IRender* r,char* name )
		{
			render = r; //�����_�����O�\���������
			img = r->createImage( name );//�ǂݍ���
		}
		
		/** ������\��
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		: 
		 * @param [in] int add_w	: �����Ԋu��
		 * @param [in] int add_h	: �����Ԋu��(���s)
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������
		 * @param [in] int pw		: �p�^�[����
		 * @param [in] int ph		: �p�^�[����
		 * @param [in] char *str	: ������
		 * @param [in] ...			: ���̑�
		 **/
		void Print( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *str, ... );
		
		/** ������\��
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		: 
		 * @param [in] int add_w	: �����Ԋu��
		 * @param [in] int add_h	: �����Ԋu��(���s)
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������
		 * @param [in] int pw		: �p�^�[����
		 * @param [in] int ph		: �p�^�[����
		 * @param [in] char *m		: ������
		 **/
		void Puts( int x,int y,int add_w,int add_h,int mod,int mul,int pw,int ph,char *m );
		
		/** �P�����\��
		 *
		 * @param [in] int x		: �\�����W
		 * @param [in] int y		:
		 * @param [in] int mod		: %�Ŋ������
		 * @param [in] int mul		: /�Ŋ������	
		 * @param [in] int w		: �p�^�[����
		 * @param [in] int h		: �p�^�[����
		 * @param [in] int str_pat	: ������p�^�[���ԍ�
		 **/
		void Putchar(int x,int y,int mod,int mul,int w,int h,int str_pat );
	};
}}