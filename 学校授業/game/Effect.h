/** @file
 * �G�t�F�N�g�N���X�̒�`
 * @date 2009/06/19
 * @author ����
 **/
#pragma once


#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../graphics/IRender.h"
#include "Object2D.h"

namespace sugino{
namespace game{

	/** �G�t�F�N�g���N���X */
	class EffectBase
	{
	private:

	protected:
		/** ���W */
		math::Vector2 pos;
		
		/** �T�C�Y */  
		math::Vector2 scale;		//x��y�ʁX�g��
		
		/** ���{�T�C�Y */
		float size;
		/** ��] */
		float rot;

		/** �J�E���g */
		int cnt;
	public:
		virtual ~EffectBase(){}											//�f�X�g���N�^
		/** �R���X�g���N�^
		 * @param [in] Vector2 &p	:�������W
		 * @param [in] int	   count:�����J�E���g
		 * @param [in] float	r	:�p�x
		 **/
		EffectBase( math::Vector2 &p,int count,float r ) : pos(p),scale(1,1),size(0),rot(r),cnt(count){}	//�R���X�g���N�^
		/** ���z�\�� */
		virtual bool Anim();
		/** �\��
		 * @param [in] IRender		*render	:�\�����摜
		 * @param [in] Model2D		&model	:���f��
		 * @param [in] matrix3x3	&m		:�J�����s��
		 **/
		virtual void Draw( graphics::IRender *render,Model2D &model,math::Matrix3x3 &m );
		/** �Z�b�^�[�֐� */
		void setScale( math::Vector2 &scal ){ scale	=scal; }
		void setSize( float s ){ size = s; }
		void setRotate( float r ){ rot = r; }
		void setCnt( int count ){ cnt = count; }
		/** �Q�b�^�[�֐� */
		math::Vector2 getPos(){ return pos; }
		math::Vector2 getScale(){ return scale; }
		float getSize(){ return size; }
		float getRotate(){ return rot; }
		int getCount(){ return cnt; }
	};
	
	/**�@�G�t�F�N�g���Ǘ�����N���X */
	class EffectManager
	{
	private:
		/** ���X�g���|�C���^�^�ŊǗ� */
		std::list< EffectBase* > effect;
		/** �G�t�F�N�g�Ɏ����� */
		Model2D model;
		/** �e�N�X�`���������� */
		system::AutoPtr<graphics::IImage>	tex;//�C���[�W�i�e�N�X�`���j���O���������������R
	public:
		~EffectManager() { Dispose(); }
		/** ������
		 * @param [in] Vector2 &p	:���������W
		 * @param [in] int	count	:�����J�E���g
		 * @param [in] float	r	:�p�x
		 **/
		void Init( math::Vector2 &p,int count,float r );
		/** ���f���ƃe�N�X�`���̓ǂݍ���
		 * �ǂݍ��ޑO��"ZD"��Face�Ƀe�N�X�`�����\���Ă��邩�m�F
		 * @param [in] char *model_name		: ���f���t�@�C����
		 * @param [in] const char *tex_name	: �e�N�X�`���t�@�C����
		 * @param [in] IRender *render		: �����_�[
		 **/
		void Load( char *model_name,const char *tex_name,graphics::IRender *render );
		void Anim();
		/** �\��
		 * @param [in] IRender	*render		:�\�����������摜��
		 * @param [in] Matrix3x3	&m		:�J�����s��
		 **/
		void Draw( graphics::IRender *render,math::Matrix3x3 &m );					//���f����Manager
		/** ��� */
		void Dispose();
	};
}}