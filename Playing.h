/** @file
 * �Q�[���̗V�ԕ����𓝊�����N���X�̒�`
 * @date 2009/08/27
 * @author ����
 **/
#pragma once

#include	"../game/Camera3D.hpp"		//�J�����N���X
#include	"../game/Scene.h"			//�V�[�����N���X
#include	"../math/Matrix.h"			//�J�����s��
#include	"../graphics/IRender.h"		//�����_�����O
#include	"../input/Input.h"			//����
#include	"../game/Object3D.h"		//�R�cModel�N���X
#include	"Stadium.h"					//�X�^�W�A��
#include	"Player.h"					//�I��i�����i�[�A���
#include	"Bat.h"						//�o�b�g
#include	"Ball.h"					//�{�[��
#include	"Rule.h"					//���[��
#include	"../game/BoundingObject.h"	//����
#include	"../editer/Edit.h"			//����G�f�B�^�[
#include	"../thread/Thread.h"		//�X���b�h
#include	"CameraAngle.h"				//�J�����̃A���O��

namespace sugino{
/** �싅�Ղ̗̈� */
namespace baseball{
	/** �싅�ՃQ�[���̃N���X */
	class BaseBall : public game::SceneBase,thread::IThreadRunnable
	{
	private:
			/** �w�i��ێ� */
			system::AutoPtr<graphics::IImage>	bg;				//�w�i�e�N�X�`��
			system::AutoPtr<graphics::IImage>	b;				//���z��ʃe�X�g

			//���[�h���ɕK�v�Ȃ���
			system::AutoPtr<graphics::IImage>	load_bg;		//���[�f�B���O���w�i�e�N�X�`��
			system::AutoPtr<graphics::IImage>	load_f;			//���[�f�B���O�������e�N�X�`��
			system::AutoPtr<graphics::IImage>	img_Helpkey;			//�L�[�w���v�e�N�X�`��
			
			system::AutoPtr<graphics::IImage>	img_attacker;



			DWORD								load_al;		
			int									load_wait;		//100���ɂȂ����Ƃ������]�T��������
			void CalAlpha();
			int									load_cnt;
			int									draw_cnt;		//�\�����Ă���J�E���g	
			
			/** �X���b�h */
			thread::Thread	thre;

			/** �J���� */
			game::Camera3D	camera;

			/** �O���E���h�N���X�ŊǗ� */
			Ground			ground;
			
			/** ���[���N���X�ŊǗ� */
			Rule			rule;

			/** �{�[���N���X�ŊǗ� */
			Ball			ball;

			/** �v���C���[�i�I��j�N���X�ŊǗ� */
			PlayerManager	playerM;
			
			/**�@����̃J�S�N���X�ŊǗ� */
			BasketManager	basketM;

			/** �X�^�W�A���i����j�N���X�ŊǗ� */
			StadiumManager	stadiumM;
						
			/** �X�J�C�h�[���N���X�ŊǗ� */
			SkyDome			sky;

			/** �o�b�g�N���X�ŊǗ� */
			Bat				bat;
			
			/** ����G�f�B�^�[�N���X�ŊǗ� */
			editer::Editer	edit;

			/** �J�����̎��_ */
			CamAngle		angle;

			/** ���[�h
			 * @note	false:�ʏ탂�[�h	true:�G�f�B�b�g��
			 **/
			bool			edit_mode;
			
			/** �X���b�h�I���t���O
			 * @note	false:�V�[�����̂��f�X�g���N�^�ɓ����ĂȂ� true:�����Ă���
			 **/
			bool			exit;

			/** �G�f�B�b�g���ɃJ�����̌��Ă�� */
			float			at_pos_x;
			float			at_pos_z;
			
			/** ��U��U��ێ� */
			Rule::SIDE		player1;
			Rule::SIDE		player2;

			/** �U��������������̏�Ԃ�ێ� */
			Rule::STATE		p1_state;
			Rule::STATE		p2_state;

			/** �V�[���Ǘ��̏��
			 * @note Rule�N���X��GameSet�֐��߂�l����V�[�����Ǘ�����
			 *
			 **/
			 int			scene_flag;

			/** BGM */
			int play_bgm;
			int play_bgm2;
			bool play_bgm_f;

	public:
			/** �R���X�g���N�^
			 * @param [out] IRender *r		:main�ŃC���X�^���X�����ꂽ�����_�[
			 * @param [out] Key		*k		:main�ŃC���X�^���X�����ꂽ�L�[
			 * @param [out] Mouse	*m		:main�ŃC���X�^���X�����ꂽ�}�E�X
			 * @param [in]	int		wid		:��ʕ�
			 * @param [in]	int		hei		:��ʍ���
			 **/
			BaseBall( graphics::IRender *r, input::Key *k, input::Mouse *m,int wid,int hei ) : SceneBase( r,k,m )
			{
				//�J�����̏�����
				camera.Initialize( math::Vector3( 0,1,2 ),math::Vector3(0,0,1),wid,hei,(float)wid,(float)hei,2,10000 );
				edit_mode	= false;
				at_pos_x	= 0.0000001f;
				at_pos_z	= 0.0000001f;
				//��U��U�̏�����
				
				//�{���̕�
				player1 = Rule::IN_SIDE;			//�\�A��U
				p1_state= Rule::DEFFENCE;				//player1���U����Ԃ�
				player2 = Rule::OUT_SIDE;			//���A��U
				p2_state= Rule::DEFFENCE;			//player2�������Ԃ�
				/*
				player1 = Rule::OUT_SIDE;			//�\�A��U
				p1_state= Rule::ATTACK;				//player1���U����Ԃ�
				player2 = Rule::IN_SIDE;			//���A��U
				p2_state= Rule::DEFFENCE;			//player2�������Ԃ�
				*/

				scene_flag = 0;

				b =NULL;
				exit		= false;
				load_cnt = draw_cnt = 0;
				load_wait=50;
				bg	= r->createImage( "data/graphic/sky2.png" );
				
			}

			//�f�X�g���N�^
			~BaseBall()
			{
				exit = true;
				if( thre.isActive() ) thre.Wait();
				void Dispose();
			}

			/** ������ */
			void Init();

			/** ���C������ */
			void Main();
			
			/** ��ʕ\�� */
			void Draw();
			
			/** �\���i��U��U�j�̎擾 */
			Rule::SIDE getSidePlayer1(){ return player1; }
			Rule::SIDE getSidePlayer2(){ return player2; }

			/** �\���i��U��U�j�̃Z�b�g */
			void setSidePlayer1( Rule::SIDE _side ){ player1 = _side; }
			void setSidePlayer2( Rule::SIDE _side ){ player2 = _side; }

			/** �U���Ԃ̎擾 */
			Rule::STATE getStatePlayer1(){ return p1_state; }
			Rule::STATE getStatePlayer2(){ return p2_state; }

			/** �U���Ԃ̃Z�b�g */
			void setStatePlayer1( Rule::STATE _state ){ p1_state = _state; }
			void setStatePlayer2( Rule::STATE _state ){ p2_state = _state; }

			/** �U���Ԃ̑S�Z�b�g
			 * @param [in]	STATE	p1state	:player1�̍U��̏��
			 * @param [in]	STATE	p2state	:player2�̍U��̏��
			 * @note p1���U���Ȃ�� p2�͎���łȂ���΂Ȃ�Ȃ�
			 **/
			void setPlayerAll( Rule::STATE p1state,Rule::STATE p2state )
			{
				if( p1state == p2state ) throw system::Exception( "�U��̏�Ԃ������ɂȂ��Ă܂�" );
				setStatePlayer1( p1state );
				setStatePlayer2( p2state );
			}

			/** �X���b�h���C�� */
			virtual DWORD Run();
			
			/** ���
			 * @note �X���b�h�Ń��[�h���ꂽ���̂̉��
			 **/
			void Dispose();

			//�J��������Ŏg�p
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
			
			/** �����I�� */
			void GameSet();

			/** �w���v�̕\�� */
			void DrawHelp( graphics::IRender *render,Rule::STATE p1state,Ball *ball,Bat *bat );
	};

}}