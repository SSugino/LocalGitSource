/** @file
 * �I�u�W�F�N�g�J���[�֐��N���X�̒�`
 * @date 2009/06/03
 * @author ����
 **/
#pragma once
#include "../system/Template.h"
#include "Float.h"

namespace sugino{
namespace math{
	
	/** �J���[�N���X(�x�N�g���Ǝ��Ă���̂�namespace math�Œ�`) */
	class Color{
	public:
		float a;								//�A���t�@�l
		float r;								//�ԗv�f
		float g;								//�Ηv�f
		float b;								//�v�f

		Color(){}								//�f�t�H���g�R���X�g���N�^
		/** ������
		 * @param [in] float rr : �����o�ϐ� r �ւ̏�����
		 * @param [in] float gg : �����o�ϐ� g �ւ̏�����
		 * @param [in] float bb : �����o�ϐ� b �ւ̏�����
		 * @param [in] float aa : �����o�ϐ� a �ւ̏�����
		 **/
		Color( float rr,float gg,float bb,float aa=1.0 ) :  r(rr) ,g(gg) ,b(bb),a(aa)  {};
		
		/** �F�̎��o�� �e�F�v�f��float�ϊ�
		 * @param [in] unsigned long col	:�F(0x00000000�`0xffFFFFFF)
		 **/
		Color( unsigned long col )
		{
			int ia = (col>>24)&0xFF;					//�e�F���o��
			int ir = (col>>16)&0xFF;
			int ig = (col>>8)&0xFF;
			int ib = col&0xFF;

			this->a = ia /255.0f;						//float�ɒ���
			this->r = ir /255.0f;
			this->g = ig /255.0f;
			this->b = ib /255.0f;
		};
		//---- �l�����Z -----
		/** ���Z */
		inline Color operator + ( const Color &c ) const{ return Color( r+c.r, g+c.g, b+c.b, a+c.a ); }	
		/** ���Z */
		inline Color operator - ( const Color &c ) const{ return Color( r-c.r, g-c.g, b-c.b, a-c.a ); }
		/** ��Z */
		inline Color operator * ( const float s ) const{ return Color( r*s, g*s, b*s, a*s ); }		
		/** ���Z */
		inline Color operator / ( const float s ) const{ return *this * Float::inv(s); }
		
		/** �F�v�Z�p */
		inline Color operator * ( Color &c ) { return Color( r*c.r, g*c.g, b*c.b, a*c.a ); }
		
		/** ������Z */
		inline Color &operator += ( const Color &c ) { a+=c.a; r+=c.r; g+=c.g; b+=c.b; return *this; }
		inline Color &operator -= ( const Color &c ) { a-=c.a; r-=c.r; g-=c.g; b-=c.b; return *this; }
		inline Color &operator *= ( const float s ) { a*=s; r*=s; g*=s; b*=s; return *this; }
		inline Color &operator /= ( const float s ) { return *this *=Float::inv(s); }		//�t��������
		
		/* ���� */
		Color operator + () const { return *this; }
		
		/** �������] */
		Color operator - () const { return Color( -r,-g,-b,-a ) ; }

		/** �L���X�g */
		inline operator unsigned long() const 
		{
			//0�`255�܂ł͈̔͂�
			int ia = (int)(a*255);
			if(ia<0)ia=0;
			if(ia>255)ia=255;
			
			int ir = (int)(r*255);
			if(ir<0)ir=0;
			if(ir>255)ir=255;
			
			int ig = (int)(g*255);
			if(ig<0)ig=0;
			if(ig>255)ig=255;
			
			int ib = (int)(b*255);
			if(ib<0)ib=0;
			if(ib>255)ib=255;
			
			return(  (ia<<24)|(ir<<16)|(ig<<8)|(ib) );			//DWORD�l(32bit)�̍���
		}
	};
}}