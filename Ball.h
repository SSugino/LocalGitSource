/** @file
 * �{�[���N���X�̒�`
 * @date 2009/09/14
 * @author ����
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Object2D.h"
#include "../game/Collision.h"
#include "../math/Vector.h"

#include "Stadium.h"
#include "Player.h"
#include "Rule.h"

namespace sugino{
namespace baseball{
	
	//����`
	class Ground;
	class Rule;

	/** �{�[���̃N���X */
	class Ball
	{
	private:
		/** �{�[���̔������ێ� */
		game::BoundingSphere3D	sphere;
		/** �I���_(�o�b�N�l�b�g)�̔������ێ� */
		game::BoundingBox3D		bbox1;

		/** �s�� */
		math::Matrix4x4			mat_ball,mat_r,mat_sc,mat_t,mat_init;

		/** �s��v�Z */
		void calMatrix();

		/** ���̔��a�T�C�Y */
		float					size;
		
		/** ���̖ړI�n�i��������)��ێ� */
		math::Vector3			target;				//�I���_���W	
		
		/** �ʒu���W��ێ� */
		math::Vector3			pos;

		/** ������ێ� */
		math::Vector3			direct;

		/** ���̑��x */
		float					velosity;
			
		/** �ő呬�x��ێ� */
		float					max_velosity;

		/** �ŏ����x��ێ� */
		float					min_velosity;

		/** �d�͒l��ێ� */
		float					gravity;

		/** ���ʂ�ێ� */
		float					mass;

		/** ��] */
		float					rotate;

		/** ��]�x�� */
		float					rotate_val;

		/** ���f����ێ� */
		game::Model3D			model;

		/** �{�[���̃e�N�X�`���C���[�W��ێ� */
		system::AutoPtr<graphics::IImage>	img;

		/** ��ԃt���O��ێ�
		 * @note	0:�o�����Ă��Ȃ�	1:������	2:�ł��Ԃ����� 3:���ʎ�
		 **/
		int state_flag;
		
		/** �����閂���t���O
		 * true:���������� false:���ĂȂ�
		 **/
		bool makyu_flag;

	public:
		//�f�t�H���g
		Ball()
		{
			direct =math::Vector3(0,0,0);
			velosity=0;
			max_velosity=2.45f;
			min_velosity=0;
			rotate =0;
			rotate_val=0.2f;
			state_flag = false;											//��Ԃ𕚂���
	
			//����̏�����
			bbox1.p = math::Vector3( -0.587f,0,-110 );			//���S�̓^�[�Q�b�g��菭������
			bbox1.s	= math::Vector3( 35,15,0.8f);						//�^�[�Q�b�g�T�C�Y�ȉ��̔��蕝
			
			target	= math::Vector3( bbox1.p.x,1.40f,bbox1.p.z );			//�ړI�n�̍��W		
			img = NULL;
			makyu_flag = false;
		}

		//������
		void Init( char* model_name,graphics::IImage *ball_img );


		//�N���X�̃��C��
		void Main( baseball::Ground *g, baseball::Rule *rule,input::Key *key,Rule::STATE &p1state );
		
		/** ����̎擾
		 * @reutrn sphere	:���̔���
		 **/
		game::BoundingSphere3D	&getBoundingObj(){ return sphere; }

		/** �ړ� */
		void Move( input::Key &key,Rule::STATE &p1state );
		
		/** �\��
		 * @param [out]IRender	*render	:�����_�[
		 * @param [out]Matrix4x4 &vps	:�J�����s��
		 **/
		void Draw( graphics::IRender *render,math::Matrix4x4 &vps );

		/** �{�[���̏I���_����
		 * @return	false:	�����B	true:�o�b�N�l�b�g�̏I���_�܂Œ�����
		 **/
		bool BackNetCollision();
		
		//-------�@Set	--------
		/** �ڕW�n�_�̃Z�b�g
		 * @param [in] Vector3	&_target	;�ڕW�n�_
		 **/
		void setTarget( math::Vector3 &_target ){ target = _target; }

		//-------�@Get	--------
		/** �ڕW�n�_�̎擾
		 * @return	target	�ڕW�n�_
		 **/
		math::Vector3 &getTarget(){ return target; }
	
				//--------- Set ------------
		/** �ʒu�擾
		 * @return	pos			:���[�J���ʒu���W
		 **/
		math::Vector3 &getPos(){ return pos; }
		
		/** �ő呬�x�̎擾
		 * @return  max_speed	:�ō����x�̃x�N�g��
		 **/
		const float	getMaxVelosity(){ return max_velosity; }

		/** �Œᑬ�x�̎擾
		 * @return	min_speed	:�Œᑬ�x�̃x�N�g��
		 **/
		const float getMinVelosity(){ return min_velosity; }

		/** �����̎擾
		 * @return	direct		:�����̒P�ʃx�N�g��
		 **/
		const math::Vector3	&getDirectValue()
		{
			if( !direct.Length() ) system::Exception( "�������擾�ł��܂���ł���" ); 
			return direct.Normalize();
		}
		
		/** �{�[���̑��x�̎擾
		 * @return velosity		:����
		 **/
		float getVelosity(){ return velosity; } 

		/** �d�͒l�̎擾 
		 * @return	gravity		:�d�͒l�̃x�N�g��
		 **/
		float getGravity(){ return gravity; }
		
		/** ���ʒl�̎擾 
		 * @return	mass		:���ʒl�̃x�N�g��
		 **/
		float getMass(){ return mass; }

		/** ��]�̎擾 
		 * @return	rotate_val	:��]
		 **/
		float getRotateValue(){ return rotate_val; }


		/** ��ԃt���O�̎擾
		 * @return	state_floag	:���̏��
		 **/
		int getStateFlag(){ return state_flag; }

		//-------�@Set	--------
		/** �ʒu���W�̃Z�b�g
		 * @param [in]	Vector3	 _pos	:�Z�b�g������W
		 **/
		void setPos( const math::Vector3 &_pos ){ pos = _pos; }

		/** ���x��"�ő�l"���Z�b�g
		 * @param [in]	Vector3	 _max	:�Z�b�g����ő呬�x
		 **/
		void setMaxVelosity( const float &_max ){ max_velosity = _max; }

		/** ���x��"�Œ�l"���Z�b�g
		 * @param [in]	Vector3	_min	:�Z�b�g����Œᑬ�x
		 **/
		void setMinVelosity( const float &_min ){ min_velosity = _min; }
		
		/** ���x�̃Z�b�g
		 * @param [in]	float _velosity	:���x
		 **/
		void setVelosity( const float &_velosity ){ velosity = _velosity; }

		/** �����̃Z�b�g
		 * @param [in] Vector3	_direct	:�Z�b�g�������
		 **/
		void setDirectValue( const math::Vector3 &_direct ){ direct = _direct; }

		/** �d�͒l�̃Z�b�g
		 * @param [in] float	_gravity:�Z�b�g����d�͒l
		 **/
		void setGravity( float _gravity ){ gravity = _gravity; }
		
		/** ���ʒl�̃Z�b�g
		 * @param [in] float	_mass	:�Z�b�g���鎿�ʒl
		 **/
		void setMass( float _mass ){ mass = _mass; }

		/** ��]�ʂ̃Z�b�g
         * @param [in] float	rot_val	:�Z�b�g�����]��
		 **/
		void setRotValue( float rot_val ){ rotate_val = rot_val; }

		/** ��ԃt���O�̃Z�b�g
		 * @param [in] bool	_state		:�Z�b�g������
		 * @note	0:�o�����ĂȂ� 1:�s�b�`���O�� 2:�ł�����
		 **/
		void setStateFlag( int state ){ state_flag = state; }

		//------- Move�l�ARotate�l�A�␳ -----------
		/** ���W�X�V
		 * @param [in] Vector3	dir			:�s����������
		 * @param [in] float	_velosity	:���x
		 * @note �����������Ȃ��Ă����Ȃǂ̕ω������鎞�Ȃ�
		 **/
		void addMove( const math::Vector3 &dir, const float _velosity );

		/** ��]�ʂ�������
         * @param [in] float add_rot		:�������]��
		 **/
		void addRotate( float add_rot );

		/** ���W�̕␳
		 * @param [in] Vector3	_pos		:������␳�� 
		 **/
		void revisePos( const math::Vector3 &_pos ){ pos += _pos; }

		/** �����t���O�̃Z�b�g */
		void setMakyuFlag( bool flag ){ makyu_flag = flag; }

		/** �o�b�N�l�b�g���W�̎擾
		 * @return Vector3 �o�b�N�l�b�g���W
		 **/
		math::Vector3 &getBackNetPos(){ return bbox1.p; }
	};
}}