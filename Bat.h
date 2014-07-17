/** @file
 * �o�b�g�N���X�̒�`
 * @date 2009/08/30
 * @author ����
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../input/Input.h"
#include "Rule.h"

namespace sugino{
namespace baseball{

	/** ����` */
	class Ball;

	/** �o�b�g/�o�b�^�[�i�Ő�)�N���X */
	class Bat
	{
	public:
		
		/** �ŐȂ̗񋓑� */
		enum BATTER_TYPE{
			LEFT	 = 1,				//�L���b�`���[����݂č��o�b�^�[
			RIGHT	 = 2,				//�L���b�`���[����݂ĉE�o�b�^�[
		};

	private:
		/** �o�b�g�̃e�N�X�`���C���[�W��ێ� */
		system::AutoPtr<graphics::IImage>	img; 

		/** ���f����ێ� */
		game::Model3D						model; 
		
		/** �ŐȂ�ێ� */
		BATTER_TYPE							type;

		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_bat,mat_r,mat_sc,mat_t,mat_init;
		
		/** Se */
		int se_swing;
		int se_hitting;
		bool se_swing_f;

		/** �X���ő�p�x */
		float								lean_rot;

		/** �X���X�s�[�h */
		float								lean_speed;

		/** �X���J�E���g
		* @note	lean_speed�ƌv�Z����̂�float�^�ŗp��
		**/
		float								lean_cnt;

		/** �X�������t���O
		 * @note	0:�ҋ@	1:�X����	2:�߂蒆
		 **/
		int									lean_flag;
		
		/** ��]�p�x */
		float								rot;
		
		/** ��]�� */
		float								rot_val;

		/** �p���[
		 * @note	��]���߂�Ƃ��ɑ�����
		 **/
		float								pow;

		/** �z�u���W */
		math::Vector3						pos;

		/** �����蔻��̍��W
		 * @note ���f���̃{�[���ʒu���[�J�����W����ʍ��W�ɕϊ�����
		 **/
		math::Vector3						col_pos;			//�X�V�O
		math::Vector3						col_pos2;			//�X�V��

		/** ��������� */
		game::BoundingLine3D				line;

		/** �s��v�Z�p */
		void calMatrix();
		
		/**
		 * @param [in]	Key			key			:�L�[����̂��߂ɃV�[���Ŏ󂯎����Key��n��
		 * @param [out] Ball		_ball		:�{�[���Ƃ̔���̂��߂Ƀ{�[���N���X��n��
		 * @param [out] Rule		rule		:���[���N���X
		 * @param [out] STATE		p1state		:�v���C���[�P�̍U����
		 **/
		void		Swing( input::Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state  );			
	public:
		
		/** ������
		 * @param [in] name  : �ǂݍ��ރ��f���t�@�C����
		 * @param [in] _img  : �����_�[��createImage�ō쐬�����f�[�^
		 **/
		void Init( char *model_name, graphics::IImage *_img );

		/** �o�b�g�Ɋւ��郁�C��
		 * @param [in]	Key			key			:�L�[����̂��߂ɃV�[���Ŏ󂯎����Key��n��
		 * @param [out] Ball		ball		:�{�[���Ƃ̔���̂��߂Ƀ{�[���N���X��n��
		 * @param [out] Rule		rule		:���[���N���X
		 * @param [out] STATE		p1state		:�v���C���[�P�̍U����
		 **/
		void Main( input::Key *key,Ball *ball,Rule *_rule,Rule::STATE &p1state );
	
		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** �z�u���W�擾 */
		math::Vector3 &getPos(){ return pos; }

		/** �z�u���W�̃Z�b�g */
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** �ŐȂ̃Z�b�g
		 * @param [in] BATTER_TYPE	:�o�b�^�[�̑Őȗ񋓑�
		 **/
		void setBatter( BATTER_TYPE type );	
		
		BATTER_TYPE getBatter(){ return type; }

		/** �o�b�g���g���{�[���Ƃ̔����������
		 * @param [out]	Ball		*ball	:�{�[���N���X
		 **/
		void Collision( Ball *ball);
		
		/** �o�b�g�̐�̍��W�����[�J���Ȃ̂ŃO���[�o���ւ̐�p�̕ϊ� */
		void TransformVector();
		
		//----- set/get -------------------
		/** �X���ő�p�x�̎擾
		 * @return	_lean_rot					:�X���ő�p�x
		 **/
		float getLeanRot(){ return lean_rot; }

		/** �X���X�s�[�h�̃Z�b�g
		 * @param [in]	float	_lean_speed		:�X���X�s�[�h
		 **/
		void setLeanSpeed( float _lean_speed ){	 lean_speed = _lean_speed; }

		/** �X���X�s�[�h�̎擾
		 * @return	lean_speed					:�X���X�s�[�h
		 **/
		float getLeanSpeed(){ return lean_speed; }

		/** �X���t���O�̃Z�b�g
		 * @param [in]	int	state				:��ԃt���O�̔ԍ�
		 * @note 0:�ҋ@	1:�X����	2:�߂蒆
		 **/
		void setLeanFlag( int state ){ lean_flag=state; }
		
		/** �X���t���O�̎擾
		 * @return lean_flag					:��ԃt���O
		 **/
		int getLeanFlag(){ return lean_flag; }
	
		/** �X���J�E���g�̎擾
		 * @return lean_cnt						:�X���̃J�E���g
		 * @note �J�E���g�������"�U���Ă���"
		 **/
		float	getLeanCnt(){ return lean_cnt; }
		
		/** �X���J�E���g�̃Z�b�g
		 * @param [in] int						:�X���̃J�E���g
		 **/
		void setLeanCnt( float _lean_cnt ){ lean_cnt = _lean_cnt; }

	};
}}