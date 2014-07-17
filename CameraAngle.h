/** @file
 * �Q�[����ł̃J�����A���O���N���X�̒�`
 * @date 2009/11/13
 * @author ����
 **/

#include "../game/Camera3D.hpp"
#include "../input/Input.h"
#include "Rule.h"


namespace sugino{
namespace baseball{
	
	/** ����` */
	class Rule;
	class Camera;
	class Ball;
	
	/** �J�����̃A���O���N���X */
	class CamAngle
	{
	private:
		/** �G�f�B�b�g���ɃJ�����̌��Ă�� */

		math::Vector3	edit_cpos;
		/** ���o���̌��Ă��v�Z�p */
		math::Vector3	at;

		/** �����i�[�p�̃J������p�� */
		game::Camera3D		cam1st;				//1�ۂ�����p�̃J������ێ�
		game::Camera3D		cam3rd;				//3�ۂ�����p�̃J������ێ�
		
		/** �����i�[�̉��z��� */
		system::Rect		right;				//�t�@�[�X�g�p�̉��z���
		
		system::Rect		left;				//�T�[�h�p�̉��z���

		/** �J�����A���O���̃��C��1
		 * @param [out] Camera3D	&cam	:�J�����N���X
		 * @param [out]	Ball		&_ball	:�{�[���N���X
		 * @param [out] Rule		&_rule	:���[���N���X
		 * @param [out] Key			&key	:�L�[�N���X
		 * @param [in]	bool		mode	:�G�f�B�b�g���[�h
		 * @param [out]	STATE		&p1		:�v���C���[�P�̍U��̏��
		 **/
		void Aspect( game::Camera3D &cam, Ball &_ball, Rule &_rule, input::Key &key,bool mode,Rule::STATE &p1 );
				
		/** �J�����E�F�C�g�J�E���g */	
		int wait;

	public:
		/** �R���X�g���N�^ */
		CamAngle()
		{
			edit_cpos = math::Vector3(0,1,0);
			wait	=0;			
		};
		
		/** �����i�[�p�J�����̏����� */
		void Init();
		
		/** ���������p�̃��C�� */
		void Main(  game::Camera3D &cam, Ball &_ball, Rule &_rule,PlayerManager &pm,input::Key &key,bool mode,Rule::STATE &p1 );
		
		/** ViewPort�ŕ`��͈͂���� */
		void Draw( graphics::IRender *render,PlayerManager &pm,Ground &g,Ball &_ball );

		/** �{�[���̈ʒu����
		 * @return true:�E���� :false:������
		 **/
		bool isRight( math::Vector3 &pos ); 

		/** �{�[���͈͓̔�����
		 * @param [in] Vector3	pos		:���W
		 * @param [in] float	length	:�͈́i�x���w��j
		 * @return
		 **/
		bool Inside( math::Vector3 &pos,float length );
			
		//���ʂɂ��J�������o�֐�
		void isFoul( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isHit( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isOut( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isHomeRun( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		
		/** 1�ۃ����i�[�̃J�����s��̎擾 */
		math::Matrix4x4 get1stCamMatrix(){ return cam1st.getMatrix(); }		//1�ۂ̃J�����̊|�����킹���J�����s��̎擾
		
		/** 3�ۃ����i�[�̃J�����s��̎擾 */
		math::Matrix4x4 get3rdCamMatrix(){ return cam3rd.getMatrix(); }		//3�ۂ̃J�����̊|�����킹���J�����s��̎擾
		
	};
}}