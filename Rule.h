/** @file
 * �싅�Ճ��[���N���X�̒�`
 * @date 2009/10/15
 * @author ����
 **/

#pragma once
#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../game/Camera3D.hpp"
#include "../input/Input.h"

namespace sugino{
/** �싅�ł̗̈� */
namespace baseball{
		
	/** ����`�@*/
	class PlayerManager;
	class BasketManager;
	class Ground;
	class Ball;
	class Basket;
	class Bat;

	/** �싅�Ń��[���N���X */
	class Rule
	{
	public:
		/** �\���̗񋓑� */
		enum SIDE
		{
			IN_SIDE		= 1,		//�\:��U
			OUT_SIDE	= 2,		//��:��U
		};

		/** �U���A����̗񋓑� */
		enum STATE 
		{
			ATTACK		= 1,		//�U����
			DEFFENCE	= 2,		//�����
		};
		

	private:
		
		/** �e�N�X�`���C���[�W��ێ� */
		system::AutoPtr<graphics::IImage>	img_count;		//�J�E���g�̃e�N�X�`��
		system::AutoPtr<graphics::IImage>	img_result;		//�A�E�g��q�b�g�Ȃǂ̌��ʃe�N�X�`��
		system::AutoPtr<graphics::IImage>	img_score;		//���_
		system::AutoPtr<graphics::IImage>	img_cntside;	//��,�\��
		system::AutoPtr<graphics::IImage>	img_change;		//�����
		
		/** SE */
		int se_strike;
		int se_ball;
		int se_hit;
		int se_out;
		int se_foreball;
		int se_change;
		int se_homerun;
		
		
		/** ��̃J�E���g */
		int  cnt;

		/** �J�E���g�ł���z��̗v�f�ԍ��ł����� */
		int strike;			//�X�g���C�N

		int ball;			//�{�[��

		int out;			//�A�E�g
		
		/** �����_ */
		int	p1score;		//�v���C���[�P�̓��_

		int p2score;		//�v���C���[�Q�̓��_
		
		/** ���_�̃p�^�[���ԍ� */
		int p1pat;			//1��
		int p2pat;
		int p1pat1;			//10��
		int p2pat1;

		/** ���ʂ̔ԍ�
		 * @note 0:�\�����Ȃ� 1:�q�b�g 2:�A�E�g 3:�t�H�A�{�[�� 4:�t�@�[�� 5:�z�[������
		 * @note �摜�̃p�^�[���ɂ��g��
		 **/		
		int result;

		/** �_�@*/
		int point;			//���Z����_ p1score+=point�ƂȂ�

		/** �J�E���g�z�� */
		int strikeArray[3];	
		int ballArray[4];
		int outArray[3];

		/** true�̎����Z����OK */
		bool strike_add_flag;
		bool ball_add_flag;

		/** ture�̎��A�E�g�̏��������� */
		bool is_out_flag;

		/** �t�H�A�{�[���̃t���O
		 * @note Runner�N���X�Ői�ۏ������s��
		 **/
		bool foreball_flag;

		/** �V���O���q�b�g�̃t���O
		 * @note �q�b�g�̌��ʂ�������ǂɓ�����������ł͂Ȃ�
		 *		 �����܂ŋ��̐����������Ȃ������̃q�b�g
		 **/
		bool normal_hit_flag;

		/** �t�F���X�ɂ��q�b�g�̃t���O
		 * @note �t�F���X�ɓ����������ʂ̃t���O
		 *�@	 �A�E�g�̃t�F���X���܂�
		 *		 �����������e�̒萔���t���O�����Ĉ��� 2�Ȃ�c�[�x�[�X�q�b�g�Ȃ�
		 **/
		int fence_hit_flag;
		
		/** ���_������t���O
		 * @note �����𖞂������痧��
		 **/
		bool add_score_flag;

		/** �q�b�g��ł����Ƃ����t���O�B�x�[�X���낤���z�[���������낤���q�b�g�Ƃ��Ĉ���
		 *  �q�b�g��ł�����A�X�g���C�N��{�[���J�E���g�����������Ȃ��Ƃ����Ȃ��̂ł��̎��Ɏg�p����
		 **/
		bool hit_flag;

		/** ���t���O */
		bool change_flag;

		/** �Q�[���Z�b�g�t���O */
		bool end_flag;

		/** �A���t�@�l�t���O */
		bool alpha_flag;
		int	 alpha_cnt;

		/** �J�E���g�̃p�^�[���v�Z */
		void CalPattern();

		/** �J�E���g��������
		 * @param [out]	Ballbase	*bbase	:�{�[���N���X
		 * @param [out]	Ground		*ground	:�O���E���h�N���X
		 * @param [out] Bat			&_bat
		 **/
		void addCount( baseball::Ball *bbase, Ground *ground,Bat &_bat );
		
		/** �J�S���{�[������������ǂ����̔�����}�l�[�W���[�Ŏ󂯎�肳��Ɍʂɏ��������邽�߂̏���
		 * @param [out]	Ballbase	*bbase	:�{�[���N���X
		 * @param [out]	Ground		*ground	:�O���E���h�N���X
		 * @param [out]	BasketManager	*bm	:�J�S�̃}�l�[�W���[
		 **/
		void Result( baseball::Ball *bbase, Ground *ground, BasketManager &bm,PlayerManager &pm );
		
		/** Result�ŏ����������ꂽ�e�J�S�ł̃{�[������������ǂ����̔���
		 * @param [out] BallBase	*bbase	:�{�[���N���X
		 * @param [out]	Ground		*ground	:�O���E���h�N���X
		 **/
		void BasketResult( baseball::Ball *bbase, Ground *ground, Basket *bask );

		/** Result�ŏ����������ꂽ�r���Ń{�[������~�����Ƃ��̃q�b�g����
		 * @param [out] BallBase	*bbase	:�{�[���N���X
		 * @param [out]	Ground		*ground	:�O���E���h�N���X
		 * @param [out]	PlayerManager	&pm	:�v���C���[�̃}�l�[�W���[�N���X
		 **/
		void HitResult( baseball::Ball *bbase, Ground *ground,Basket *bask, PlayerManager &pm );

		/** �_���Z
		 * @param STATE p1:	���C���̃v���C���[�P�̍U��̏��
		 **/
		void AddScore( STATE *p1 );

		/** ��㏈��
		 * @param [out]	BasketManager	&bm:	�J�S�̃}�l�[�W���[
		 * @param [out] PlayerManager	&pm:	�I��̃}�l�[�W���[
		 * @param [in]	ball		  *ball:	�{�[���N���X
		 * @param [out] STATE			*p1:	���C���̃v���C���[�P�̍U��̏��
		 * @param [out] STATE			*p2:	���C���̃v���C���[�Q�̍U��̏��
		 **/
		void Change( BasketManager &bm,PlayerManager &pm,Ball *bbase,STATE *p1,STATE *p2 );
		
			
		/** �񂪐i�ނ��Ƃ̏�����
		 * 
		 * @note ��ȊO�̃J�E���g��S�ď�����
		 **/
		void NextInit( baseball::Ball *bbase, Bat &_bat );

		/** �Ŏ҂��Ƃ̏�����
		 * @note �{�[���A�X�g���C�N�J�E���g��������
		 **/
		void BatterInit();
		
		/** �A�E�g�̏���
		 * @param [in]	Ballbase *bbase	:�{�[���̊��N���X
		 * @param [in]	Bat		 *_bat	:�o�b�g�N���X
		 * @note �O�U�A�t�F���X�A�L���b�`�������̃A�E�g����
		 **/
		void Out( Ball *bbase,Bat *_bat );
		
		/** �U��̏�Ԃ�ێ� */
		STATE	state;

		/** �A���t�@�l */
		DWORD	alpha;
		DWORD	r_alpha;				//���ʕ\���p�̃A���t�@�l

		/** �摜�̈ړ� */
		int home_x;						//�z�[�������摜�̍��W
		int home_mov;					//�z�[�������摜�̈ړ���
	
					
		/** ����͈͂̔��� */
		game::BoundingSphere3D	b_sphere;
	
	public:
		/** �R���X�g���N�^ */
		Rule() : cnt(1),out(0),ball(0),strike(0)
		{
			strike_add_flag =false;
			ball_add_flag	=false;		
			foreball_flag	=false;
			normal_hit_flag =false;
			hit_flag		=false;		
			change_flag		=false;
			fence_hit_flag	=false;
			add_score_flag	=false;
			is_out_flag		=false;
			end_flag		=false;
			alpha_flag		=false; 
			alpha_cnt		=0;

			img_count		= NULL;
			img_result		= NULL;
			img_score		= NULL;
			img_cntside		= NULL;
			img_change		= NULL;

			int i;
			for(i=0;i<sizeof(outArray)>>2;i++) outArray[i]	=0;
			for(i=0;i<sizeof(ballArray)>>2;i++) ballArray[i]	=0;
			for(i=0;i<sizeof(strikeArray)>>2;i++) strikeArray[i]	=0;

			p1pat	=0;
			p2pat	=0;
			p1pat1	=0;
			p2pat1	=0;

			result = 0;

			p1score = 0;
			p2score = 0;
			point	= 0;

			//�͈͏�����
			b_sphere.p = math::Vector3( 1,1,-75 );	//�z�[���x�[�X��菭���O
			b_sphere.s = 28;

		}

		/** �{�[����Ԃ�0�̂Ƃ��̏����� */
		void isReturn();


		/** ������
		 * @param [in]	IImage	*count_img	:�����_�[��createImage�ō쐬�����f�[�^
		 * @param [in]	IImage	*result_img	:�����_�[��createImage�ō쐬�����f�[�^
		 * @param [in]	IImage	*score_img	:�����_�[��createImage�ō쐬�����f�[�^
		 * @param [in]	IImage	*side_img	:�����_�[��createImage�ō쐬�����f�[�^
		 * @param [in]	IImage	*side_img	:�����_�[��createImage�ō쐬�����f�[�^
		 **/
		void Init( graphics::IImage *count_img,graphics::IImage *result_img,
				   graphics::IImage *score_img,graphics::IImage *side_img,
				   graphics::IImage *change_img);

		/** ���C��
		 * @param [in]	BallBase		*bbase	:�{�[���N���X
		 * @param [in]	Ground			*ground	:�O���E���h�N���X
		 * @param [in]	BasketManager	*bm		:�J�S�̃}�l�[�W���[�N���X
		 * @param [in]	PlayerManager	&pm		:�J�S�̃}�l�[�W���[�N���X
		 * pparam [in]	Bat				&_bat	:�o�b�g�N���X
		 * @param [in]	STATE			*p1		:�v���C���[�P�̏�ԗ񋓑�
		 * @param [in]	STATE			*p2		:�v���C���[�Q�̏�ԗ񋓑�
		 * @param [in]	int				*scene_f:���C���̃V�[���t���O
		 **/
		void Main( baseball::Ball *bbase, Ground *ground,BasketManager &bm,PlayerManager &pm,Bat &_bat,STATE *p1,STATE *p2,int *scene_f );

		/** �`��
		 * @param [out]	IRender		*render	:�����_�[
		 * @param [out]	Matrix4x4	&vps	:�|�����킵���J�����s��
		 * @param [out]	Bat			*bat	:�o�b�g�N���X
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps,Bat &_bat );

		/** �A���t�@�l�����A�v�Z�A ����
		 * @return alpha >= 1.0f:true		����ȊO:false 
		 **/
		bool ColAlpha();

		//--------- Set ------------
		/** ��̃Z�b�g
		 * @param [in]	int _cnt	:��
		 **/
		void setCnt( const int _cnt ){ cnt = _cnt; }		//��̃Z�b�g

		/** �A�E�g�J�E���g���Z�b�g
		 * @param [in]	int _out	:�A�E�g�J�E���g
		 **/
		void setOut( const int _out ){ out = _out; }	
		
		/** �{�[���J�E���g�̃Z�b�g
		 * @param [in]	int _ball	:�{�[���J�E���g
		 **/
		void setBall( const int _ball ){ ball = _ball; }	

		/** �X�g���C�N�J�E���g�̃Z�b�g
		 * @param [in]	int _strike	:�X�g���C�N�J�E���g
		 **/
		void setStrike( int _strike ){ strike = _strike; }
		

		/** ���Z�����t���O�̃Z�b�g
		 * @note �ʂ̃N���X�ŉ��Z����������(�X�g���C�N��t�@�[��)������Z�b�g����
		 **/
		void setStrikeAddFlag( bool flag ){ strike_add_flag = flag; }
		
		/** ���Z�����t���O�̃Z�b�g
		 * @note �ʂ̃N���X�ŉ��Z����������������Z�b�g����
		 **/
		void setBallAddFlag( bool flag ){ ball_add_flag = flag; }

		/** �ʏ�q�b�g�̃t���O�̃Z�b�g
		 * @param [in] bool flag			:��ԃt���O
		 **/
		void setNormalHitFlag( bool flag ){ normal_hit_flag = flag; }
		
		/** �q�b�g��ł����t���O
		 * @param [in] bool	flag			:��ԃt���O
		 * @note �t�F���X���܂�
		 **/
		void setHitFlag( bool flag ){ hit_flag = flag; }

		/** �t�F���X�ɓ��������q�b�g�t���O
		 * @param [in]	bool flag			:��ԃt���O
		 * @note 0:�A�E�g�t�F���X 1:�V���O���q�b�g 2:�c�[�x�[�X�q�b�g
		 *		 3:�X���[�x�[�X�q�b�g 4:�z�[������
		 **/
		void setFenceHitFlag( int flag ){ fence_hit_flag = flag; }
		
		/** ���_���Z�t���O
		 * @param [in]	bool flag			:��ԃt���O
		 **/
		void setAddScoreFlag( bool flag ) { add_score_flag = flag; }

		/** �t�H�A�{�[���̃t���O�̃Z�b�g
		 * @param [in] bool	flag			:��ԃt���O
		 **/
		void setForeballFlag( bool flag ){ foreball_flag = flag; }

		/** ���t���O�̃Z�b�g
		 * @param [in]	bool flag			:��ԃt���O 
		 **/
		void setChangeFlag( bool flag ){ change_flag = flag; }
		
		/** �_�̃Z�b�g
		 * @param [in]	int point			:�_
		 * @note addScoreFlag=true�̎��ɉ��Z�����l�Bfalse�̎��͏��1������
		 **/
		void setPoint( int _point ){ point = _point; }
		
		/** �A�E�g�t���O�̃Z�b�g
		 * @param [in]	bool flag			:�t���O
		 * @note true�ɂȂ�ƃA�E�g����������
		 **/
		void setIsOutFlag( bool flag ){ is_out_flag= flag; }

		//--------- Get ------------
		/** ��̎擾
		 * @return	cnt			:��
		 **/
		const int &getCnt(){ return cnt; }			//��̎擾
		
		/** �A�E�g�J�E���g�擾
		 * @return	out				:�A�E�g�J�E���g
		 **/
		const int &getOut(){ return out; }
		
		/** �{�[���J�E���g�̎擾
		 * @return	ball			:�{�[���J�E���g
		 **/
		const int &getBall(){ return ball; }
		
		/** �X�g���C�N�J�E���g�̎擾
		 * @return strike			:�X�g���C�N�J�E���g
		 **/
		const int &getStrike(){ return strike; }
	
		/** �t�H�A�{�[���t���O�̎擾
		 * @return true:�t�H�A�{�[��	false:�t�H�A�{�[���łȂ�
		 **/
		bool getForeballFlag(){ return  foreball_flag; }		

		/** �ʏ�q�b�g�t���O�̎擾
		 * @return true:�q�b�g			false:�q�b�g�ł͂Ȃ�
		 **/
		bool getNormalHitFlag(){ return normal_hit_flag; }

		/** �q�b�g�t���O�̎擾
		 * @return	true:�A�E�g��t�@�[���ł͂Ȃ����ʂ�����ꂽ false:����ȊO 
		 **/
		bool getHitFlag(){ return hit_flag; }
		
		/** ���t���O�̎擾
		 * @return	true:���				false:��シ�ׂ��łȂ�
		 **/
		bool getChangeFlag(){ return change_flag; }
		
		/** �t�F���X�ɓ��������q�b�g�t���O�̎擾
		 * @return	0:�������ĂȂ��@1:�V���O���q�b�g�@2:�c�[�x�[�X�q�b�g 
		 *			3:�X���[�x�[�X�q�b�g 4:�z�[������
		 **/
		int getFenceHitFlag(){ return fence_hit_flag; }
		
		/** ���_���Z�t���O�擾
		 * @return	true:���_���Z			false:���Z���Ȃ�
		 **/
		bool getAddScoreFlag() { return add_score_flag; }

		/** �v���C���[1�̓��_�̎擾 */
		int getP1Score(){ return p1score; }
		/** �v���C���[2�̓��_�̎擾 */
		int getP2Score(){ return p2score; }
				
		/** ���_�̃p�^�[���ԍ��̎擾
		 * @param [in]	int *pat1	:1���̃p�^�[���ԍ�
		 * @param [in]	int *pat2	:10���̃p�^�[���ԍ�
		 **/
		void getP1Pat( int *pat1, int *pat2 )
		{
			*pat1 = p1pat;
			*pat2 = p1pat1;
		};

		void getP2Pat( int *pat1, int *pat2 )
		{
			*pat1 = p2pat;
			*pat2 = p2pat1;			
		};

		/** ���ʃp�^�[���̃Z�b�g 
		 * @param [in]	pat	:�p�^�[���ԍ�
		 * @note 0:�\�����Ȃ� 1:�q�b�g 2:�A�E�g 3:�t�H�A�{�[�� 4:�t�@�[�� 5:�z�[������
		 **/
		//0:�\�����Ȃ� 1:�q�b�g 2:�A�E�g 3:�t�H�A�{�[�� 4:�t�@�[�� 5:�z�[������
		
		void setResult( int res  ){ result = res; } 
		/** ���ʂ̎擾
		 * @return	result_pat			:����
		 * @note ���ʂ𓾂���̂Ńp�^�[�������łȂ��A�u���̌��ʁv�����炱���̏������g����
		 **/
		int getResult(){ return result; }

		/** ���ʕ\���̃A���t�@�l�v�Z
		 * @param [in]	float	add	:�A���t�@���Z�l
		 * @return true:�����I�� false:������
		 * @note false�̊Ԃ͑��̏����������Ȃ�
		 **/
		bool ColResultAlpha( float add );
		
		/** �z�[���������̉摜�̈ړ� */
		bool DrawMove();

		/** �Q�[���Z�b�g�̏���
		 * @param [out] STATE *p1	:�v���C���[1�̏��
		 * @return 0:�I�������ɖ������ĂȂ�	6:�v���C���[1�̏����i�p�^�[���ԍ�) 7:�v���C���[2�̏����i�p�^�[���ԍ�) 8:���������̃p�^�[���ԍ�
		 **/
		int GameSet( STATE *p1 );	
	
		/** ����̃J�S����΂Ɏ擾�ł��Ȃ��A�s�b�`���[�����O�̃L���b�`���[��蕔���̔���
		 * @param [in]	Ball	*_ball	:�{�[���N���X
		 **/
		bool InField( Ball *_ball );
	};
}}