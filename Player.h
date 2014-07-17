/** @file
 * �v���C���[�i�I��j�N���X�̒�`
 * @date 2009/09/04
 * @author ����
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Object2D.h"
#include "../game/Collision.h"
#include "../input/Input.h"

#include "Rule.h"

namespace sugino{
namespace baseball{

	/**  ����` */
	class Rule;
	class Ball;
	class Ground;

	/** �v���C���[(�I��)�̊��N���X */
	class PlayerBase
	{
	protected:
	
		/** ���W */
		math::Vector3		pos;
		
		/** �X�P�[�����O */
		math::Vector3		scale;

		/** ��] */
		float				rot;

		/** ��]�� */
		float				rot_val;

		/** ���f�� */
		game::Model3D		*model;

		/** �}�l�[�W���[�Ǘ��̃e�N�X�`���C���[�W���Q�� */
		graphics::IImage	*img;
				
		/** �o�����
		 * @note true:�o����	false:�o�����ĂȂ�
		 **/
		bool state;	

	public:
		/** �R���X�g���N�^
		 * @pragma [in] _pos		: ���W
		 * @pragma [in] _scale		: �k��
		 * @pragma [in] _rot		: ��]
		 * @param�@[in] _rot_val	: ��]�� 
		 * @pragma [in] _model		: ���f���f�[�^
		 * @pragma [in] _img		: �e�N�X�`���f�[�^
		 **/
		PlayerBase( math::Vector3 &_pos, math::Vector3 &_scale, float _rot,float _rot_val, game::Model3D *_model, graphics::IImage *_img, bool _state )
		{
			pos		= _pos;
			scale	= _scale;
			rot		= _rot;
			rot_val	= _rot_val;
			model	= _model;
			img		= _img;
			state	= _state;
		}

		/** �f�X�g���N�^ */
		virtual ~PlayerBase(){}

		/** ������ */
		virtual void Init(){};			 

		/** ���C������
		 * @param [out] Key			&key	:���C���̃L�[�N���X
		 * @param [out] Ball		*_ball	:�{�[���N���X
		 **/
		virtual void Main( input::Key &key,Ball *_ball,
							Rule *rule,Rule::STATE &p1state ){};		//�h����ŏ������������̂ŉ��z�ŗp��
		
		/** �\��
		 * @param [out] IRender		*r	:�����_�[
		 * @param [out]	Matrix4x4	&vps:�J�����s��
		 **/
		virtual void Draw( graphics::IRender *r,math::Matrix &vps )=0;
		
		//--------- Get ------------
		/** ���W�̎擾
		 * @return	pos					:3�������W
		 **/
		math::Vector3	getPos(){ return pos; }
		
		/** �X�P�[���̎擾
		 * @return scale				:3�����X�P�[��
		 **/
		math::Vector3	getScale(){ return scale; }
		
		/** ��Ԃ̎擾 
		 * @return	state				:�o�����
		 **/
		bool			getState(){ return state; }

		/** ��]�̎擾
		 * @return	rot					:�p�x
		 **/
		float			getRot(){ return rot; }

		/** ��]�ʂ̎擾
		 * @return rot_val				:��]��
		 **/
		float			getRotVal(){ return rot_val; }

		//--------- Set ------------
		/** ���W���Z�b�g����
		 * @param [in] Vector3	_pos	:3�������W
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** �X�P�[�����O���Z�b�g����
		 * @param [in] Vector3	_scale	:3�����X�P�[��
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** ��]���Z�b�g����
		 * @param [in] float	_rot	:��]�x��
		 **/
		void setRot( float _rot){ rot = _rot; }
		
		/** ��]�ʂ��Z�b�g����
		 * @param [in] float	_rot_val:��]��
		 **/
		void setRotaVal( float _rot_val ){ rot = _rot_val; }
		
		/** �o����Ԃ̃Z�b�g
		 * @param	bool	state		:�o�����
		 **/
		void setState( bool _state ){ state = _state; }

		/** �����������
		 * @param [out] Ball *bbase		:�{�[���N���X 
		 **/
		virtual void Collision( Ball *bbase ){};

		/** ���x�̕ύX */
		virtual void setSpeed( float sp ) =0;

	};	

	/** �v���C���[(�I��j�Ǘ��N���X */
	class PlayerManager{
	private:
		/** ���N���X��ێ� */
		std::list<PlayerBase *>	player;			

		/** �����i�[�̃��f�� */
		game::Model3D			mdl_runner;

		/** �s�b�`���[�̃��f�� */
		game::Model3D			mdl_picher;

		/** �h����̉摜�������ŊǗ� */
		system::AutoPtr<graphics::IImage>	img_runner1;
		system::AutoPtr<graphics::IImage>	img_runner2;
		system::AutoPtr<graphics::IImage>	img_runner3;
		system::AutoPtr<graphics::IImage>	img_pich;
		system::AutoPtr<graphics::IImage>	img_minirun;
		
		/** Se */
		int se_home_run;
		int se_hakushu;

		/** ��ԃt���O�̗����Ă����
		 * @note	�����i�[�l��
		 **/
		int						runner_cnt;

		/** �V���O���q�b�g�������i�[��i�߂鏈��
		 * @param [out]	Ball	*_ball	:�{�[���N���X
		 **/
		void					SingleHitGoRunner( Ball *_ball,Rule *rule );
		
		/** �c�[�x�[�X�q�b�g�������i�[��i�߂鏈��
		 * @param [out]	Ball	*_ball	:�{�[���N���X
		 **/
		void					TwoBaseHitGoRunner( Ball *_ball,Rule *rule );
		
		/** �X���[�x�[�X�q�b�g�������i�[��i�߂鏈��
		 * @param [out]	Ball	*ball	:�{�[���N���X
		 **/
		void					ThreeBaseHitGoRunner( Ball *_ball,Rule *rule );	
		
		/** �z�[���������̃����i�[��i�߂鏈��
		 * @param [out]	Ball	*_ball	:�{�[���N���X
		 **/
		void					HomeRunGoRunner( Ball *_ball,Rule *rule );	
		
		/** �~�j�����i�[�̃p�^�[���ԍ� */
		int						mini_pat;
		
		/** �����i�[���̃p�^�[�� */
		void					CalPattern();

		/** �F�̍��������A�v�Z */
		void					CalColor();
		
		/** �A���t�@�l */
		DWORD					alpha;
		
		/* �A���t�@�l���Z���Z�t���O */
		bool					alpha_flag;
		
		/** �����o�� */
		void					PushRunner();

	public:
		/** �R���X�g���N�^ */
		PlayerManager(){
			img_runner1	= NULL;
			img_runner2 = NULL;
			img_runner3 = NULL;
			img_pich	= NULL;
			img_minirun	= NULL;
			
			runner_cnt	= 0;
			mini_pat	= 0;
			alpha		= 0xBBFFFFFF; 
			alpha_flag	= false;
		}
		
		/** �f�X�g���N�^/��� */
		~PlayerManager(){ Dispose(); }
		
		/** Base��K�v��������
		 * @note new-s��PlayerBase�h���^�����������Ă���
		 **/
		void Init();
		
		/** ���C��
		 * @param [out]	Key				*key	:���C���̃L�[�N���X
		 * @param [in]	Ground			*g		:�O���E���h�N���X
		 * @param [out]	Ball			*_ball	:�{�[���̃N���X
		 * @param [out]	BasketManager	*baskM	:�J�S�̃}�l�[�W���[�N���X
		 * @param [out] Rule			*rule	:���[���N���X
		 **/
		void Main( input::Key *key,baseball::Ground *g,Ball *_ball,BasketManager *baskM,
			 baseball::Rule *rule,Rule::STATE &p1state );		//�n�ʂ�ǂƂ̔�����������邽�߂Ɉ����ɓ����

		/** �\��
		 * @param [out] IRender		*render	:�����_�[
		 * @param [out] Matrix4x4	&vps	:�J�����s��
		 **/
		void Draw( graphics::IRender *render, game::Camera3D &cam,Bat &_bat );
	
		/** �ړ� */
		void Move();
		
		/** ���X�g�̉������ 
		 * @note ���X�g�ŕ���PlayerBase�̉��
		 **/
		void Dispose();

		//--------- Get ------------
		/** �v���C���[���X�g�̎擾
		 * @return player		:���N���X�̃��X�g
		 **/
		std::list< PlayerBase *> &getPlayer(){ return player; }

		/** �h����̃f�[�^���擾
		 * @param [in]	int here	:�~�������X�g�f�[�^�܂ł̃��[�v��
		 * @return	�v���C���[�̊��N���X
		 **/
		PlayerBase *getPlayerBase( int here ); 

		/** �h����̏�ԃf�[�^���擾 */
		void SarchPlayerState();

		/** �w��̔h����f�[�^�̏�Ԃ��Z�b�g����
		 * @param [in]	int		num		:�~�������X�g�f�[�^�܂ł̃��[�v��
		 * @param [in]	bool	_state	:�V���ɃZ�b�g����t���O
		 *	@return	true:�Z�b�g����	false:���s
		 **/
		bool setPlayerState( unsigned int num, bool _state ); 

		//--------- Set ------------
		/** �v���C���[���N���X�����X�g�֒ǉ�
		 * @note "��̕�"�����X�g�֒ǉ�
		 **/
		void setPlayer( PlayerBase *play ){ player.push_back(play); }
		
		/** ���f���̃Z�b�g�i�O���ǂݍ���)
		 * @param [in] char		*model_name		:���f���̖��O
		 * @note "PlayerManager�����o"�umdl_runner�v��Load�֐����Ă�
		 **/
		void setRunnerModel( char *model_name ){ mdl_runner.Load(model_name); }

		/** �e�N�X�`���̈ꊇ�Z�b�g
		 * @param [out] IRender	*render		:�����_�[
		 * @param [in]	char *runner_img1	:�����i�[�̃e�N�X�`��1
		 * @param [in]	char *runner_img1	:�����i�[�̃e�N�X�`��2
		 * @param [in]	char *pich_img	:�s�b�`���[�̃e�N�X�`��1
		 **/
		void setImage( graphics::IRender *render,char *runner_img1,char *runner_img2,char *runner_img3,char *pich_img,char *mini_img  )
		{
			img_runner1	= render->createImage( runner_img1 );
			img_runner2 = render->createImage( runner_img2 );
			img_runner3 = render->createImage( runner_img3 );
			img_pich	= render->createImage( pich_img );
			img_minirun = render->createImage( mini_img );
		}

		/** ���f���̈ꊇ�Z�b�g */
		void setModel( char *runner_model,char *picher_model )
		{
			mdl_runner.Load( runner_model );
			mdl_picher.Load( picher_model );
		} 
		
		/** �����i�[�̐l���̎擾
		 * @return state_cnt	:����state�t���O�̗����Ă鐔
		 **/
		int		getRunnerCnt(){ return runner_cnt; }

		/** 3�A�E�g�̎��Ƀ����i�[��S�ď������� */
		void					DeleteRunner();


	};
	//---------------------------------------------------------------------
	
	/** �����i�[�p�N���X(PlayerBase�h��) */
	class Runner : public PlayerBase 
	{
	private:
		
		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_run,mat_r,mat_sc,mat_t,mat_init;
		
		/** �d�͒l��ێ� */
		float								gravity;

		//�s��̌v�Z
		void calMatrix();

		/** �����ێ� */
		game::BoundingOBox3D				bobox;

	public:

		/** ������ **/
		Runner( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _rot_val,game::Model3D *_model, graphics::IImage *_img,float _gravity,  bool state ) :
		PlayerBase( _pos,_scale,_rot,_rot_val,_model,_img,state )
		{
			gravity = _gravity;
			//�e���݂�ς��A�����Ŏ󂯎����rot�̌X���Ŕ����OBox��ς���
			mat_r.RotateY( _rot );
			bobox=game::BoundingOBox3D( _pos,math::Vector(1.5f,_scale.y*2.5f,0.25f),mat_r );
	   };
		
		void Init()
		{
			calMatrix();
		}

		/** �����i�[�Ɋւ��郁�C��
		 * @param [out]	Key			&key	: ���C���̃L�[�N���X
		 * @param [out] Ball		*_base	: �{�[���N���X
		 * @param [out] Rule		*rule	:���[���N���X
		 * @param [out]	STATE		*p1state:�v���C���[1�̍U����
		 **/
		void Main( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );

		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** �����蔻��
		 * @param [out]	Ball		*_ball	:�{�[���N���X
		 * @param [out]	Rule		*rule	:���[���N���X
		 **/
		virtual void Collision( Ball *_ball,Rule *rule );

		/** �o����Ԃ̎擾 */
		bool getState(){ return state; }

		/** �o����Ԃ̃Z�b�g
		 * @param [in]	bool	state		:�o�����
		 * @note	true:�o����		false:�o�������Ȃ�
		 **/
		void setState( bool _state ){ state = _state; }

		/** ���x�ύX */
		void setSpeed( float sp ){}
	};
	
	/** �s�b�`���[�̒e���o��p�N���X */
	class Picher : public PlayerBase
	{
	private:
		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_pich,mat_r,mat_sc,mat_t,mat_init;
	
		/** ��]�ʂ�ێ� */
		float								rot_val;		
		/** ��] */
		float								rotate;
		/** ��]�J�E���g */
		int									rot_cnt;
		/** �e����J�E���g */
		int									rot_save;	
		/** ��]�X�^�[�g�t���O */
		bool								rot_flag;
		/** �t��]������t���O */
		bool								rot_reverse;

		/** �d�͒l��ێ� */
		float								gravity;
	
		/** �s��̌v�Z */
		void calMatrix();

		/** �ڐG�����ێ� */
		game::BoundingBox3D					bobox;

		/** ���o���̃X�s�[�h */
		float								speed;
		
		/** ���̔���
		 * @param [out]	Key			&key	: ���C���̃L�[�N���X
		 * @param [out] Ball		*_ball	: �{�[���̃N���X
		 * @param [out] Rule		*rule	: ���[���N���X		 
		 **/
		void	Shot( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );
		
		/** �����郂�[�V�����̉�]
		 * @param [in] ball *_ball	:�{�[���N���X
		 **/
		void HandMotion( Ball *_ball );

	public:
		/** ������ **/
		Picher( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _rot_val,game::Model3D *_model, graphics::IImage *_img,float _gravity, bool state ) :
		PlayerBase( _pos,_scale,_rot,_rot_val,_model,_img,state )
		{
			gravity = _gravity;
			speed = 1.2f;
			//����̏�����
			bobox.p	= _pos;
			bobox.s  = math::Vector3( _scale.x*2.5f,_scale.y*5,_scale.z*2.5f );
		
			rot_val=0;		
			rotate=0;
			rot_cnt=59;
			rot_save = rot_cnt/2;			
			rot_flag=false;
			rot_reverse=false;

		};
		
		/** ������ */
		void Init()
		{
			calMatrix();
		}

		/** �s�b�`���[�Ɋւ��郁�C��
		 * @param [out]	Key			&key	: ���C���̃L�[�N���X
		 * @param [out] Ball		*_ball	: �{�[���N���X
		 * @param [out] Rule		*rule	:���[���N���X
		 **/
		void Main( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );
	
		/** �\��
		 * @param [in] IRender *render : IRender�N���X��h�������N���X
		 * @param [in] Matrix4x4 &vps  : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		//----- set get -------
		/** ���W�̎擾
		 * @return	pos					:3�������W
		 **/
		math::Vector3 getPos(){ return pos; }
		
		/** ���W�̃Z�b�g 
		 * @param [in]	Vector3	_pos	:3�������W
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** ��]�ʂ̎擾
		 * @return	rot					:��]��
		 **/
		float getRot(){ return rot; }

		/** ��]�ʂ̃Z�b�g
		 * @param [in]	float _rot		:��]��
		 **/
		void setRot( float _rot ){ rot = _rot; }

		/** �ڐG����
		 * @param [out]	Ball	*bbase	:�{�[���N���X
		 **/
		virtual void Collision( Ball *bbase );

		/** ���x�ύX */
		void setSpeed( float sp ){ speed =  sp; }

	};


	//----------------------------------------------------------------------------------------------------
	/** ����̃J�S���Ǘ�����N���X */
	class BasketManager
	{
	private:
		/** ���N���X��ێ� */
		std::list<Basket *>	basket;			

		/** "����̃J�S"�̃��f�� */
		game::Model3D			mdl_basket;

		/** �h����̉摜�������ŊǗ� */
		system::AutoPtr<graphics::IImage>	img_basket;
		
		/** ���̂����ǂꂩ���L���b�`���Ă� */
		bool								catch_flag;
	public:
		/** �R���X�g���N�^ */
		BasketManager(){
			img_basket	= NULL;
			catch_flag	= false; 
		}
		
		/** �f�X�g���N�^/��� */
		~BasketManager(){ Dispose(); }
		
		/** Base��K�v��������
		 * @note new-s��BasketBase�h���^�����������Ă���
		 **/
		void Init();
		
		/** ���C��
		 * @param [out]	Key			*key	:���C���̃L�[�N���X
		 * @param [out] Rule		*rule	:���[���N���X
		 * @param [in]	Ground		*g		:�O���E���h�N���X
		 * @param [out]	Ball		*ball	:�{�[���̃}�l�[�W���[�N���X
		 **/
		void Main(  input::Key *key,baseball::Rule *rule,baseball::Ground *g,Ball *_ball );

		/** �\��
		 * @param [out] IRender		*render	:�����_�[
		 * @param [out] Matrix4x4	&vps	:�J�����s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** ���X�g�̉������ 
		 * @note ���X�g�ŕ���BasketBase�̉��
		 **/
		void Dispose();

		//--------- Get ------------
		/** �J�S���X�g�̎擾
		 * @return Basket		:���N���X�̃��X�g
		 **/
		std::list< Basket *> &getBasket(){ return basket; }
		
		//--------- Set ------------
		/** �v���C���[���N���X�����X�g�֒ǉ�
		 * @note "��̕�"�����X�g�֒ǉ�
		 **/
		void setBasket( Basket *bask ){ basket.push_back(bask); }		

		/** ���f���̃Z�b�g
		 * @param [in] char		*model_name		:���f���̖��O
		 * @note "BasketManager�����o"�umdl_basket�v��Load�֐����Ă�
		 **/
		void setModel( char *model_name ){ mdl_basket.Load(model_name); }

		/** �e�N�X�`���̈ꊇ�Z�b�g
		 * @param [out] IRender	*render		:�����_�[
		 * @param [in]	char *basket_img	:�J�S�̃e�N�X�`��
		 **/
		void setImage( graphics::IRender *render,char *basket_img )
		{
			img_basket	= render->createImage( basket_img );
		}
		
		/** ��ꂪ�L���b�`�������ǂ����̃t���O�̎擾 */
		bool getCatchFlag(){ return catch_flag; }

		/**  ��ꂪ�L���b�`�������ǂ����̃t���O�̃Z�b�g */
		void setCatchFlag( bool flag ) { catch_flag = flag; }

	};


	/** ������̃J�S�p�N���X */
	class Basket
	{		
	private:
		/** ���W */
		math::Vector3		pos;
		
		/** �f�t�H���g���W */
		math::Vector3		default_pos;

		/** �f�t�H���g�s��*/
		math::Matrix4x4		mat_default;

		/** ����͈� */
		float				def_range;

		/** �X�P�[�����O */
		math::Vector3		scale;

		/** ��] */
		float				rot;

		/** ��]�ʂ�ێ� */
		float				rot_val;

		/** ���f�� */
		game::Model3D		*model;

		/** �}�l�[�W���[�Ǘ��̑I��̃e�N�X�`���C���[�W���Q�� */
		graphics::IImage	*img;
		
		/** �G�f�B�b�g���[�h��"�I��"���ꂽ���ǂ����̏�ԃt���O */
		bool				select;

		/** �{�[������������̏�� */
		bool				catch_flag;

		/** ����͈͂̔��� */
		game::BoundingSphere3D		b_sphere;

		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4				mat_basket,mat_r,mat_r2,mat_sc,mat_t,mat_init;
		
		/** �J�S�̗��T�C�h�̕Ǎ��W Base�ɂ͌��̍��W�݂̂����� */
		math::Vector3		left_pos1;			//�X�V�O
		math::Vector3		left_pos2;			//�X�V��

		math::Vector3		right_pos1;
		math::Vector3		right_pos2;
		
		/** �J�S�̒��S�B�G�f�B�b�g���̍��W */
		math::Vector3		center_pos1;
		math::Vector3		center_pos2;

		math::Vector3		forword_pos1;
		math::Vector3		forword_pos2;


		/** �d�͒l��ێ� */
		float				gravity;

		/** �s��̌v�Z */
		void calMatrix();

		/** �ڐG�����ێ�
		 * @note	�J�S�̌`�Ȃ̂ŗ��T�C�h�Ɖ���3�ɕ����܂�
		 **/
		game::BoundingOBox3D	bobox1;			//��
		game::BoundingOBox3D	bobox2;			//���T�C�h
		game::BoundingOBox3D	bobox3;			//�E�T�C�h
		game::BoundingOBox3D	b_catch;

		//�G�f�B�b�g�p�̃T�C�Y���傫�߂̔���
		game::BoundingOBox3D	bobox4;
		
		//�����ێ�
		game::BoundingSphere3D sphere;
	public:
		/** ������ **/
		Basket( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _def_range,game::Model3D *_model, graphics::IImage *_img,float _gravity )
		{
			pos			= _pos;
			default_pos = _pos;
			scale		= _scale;
			rot			= _rot;
			def_range	= _def_range;
			model		= _model;
			img			= _img;
			gravity		= _gravity;	
			//����͈͂̏�����
			b_sphere.p	= _pos;
			b_sphere.s	= def_range+3;								//����͈�			
		};
		
		/** ������
		 * @note �R���X�g���N�^�ň����ő�����Ȃ����̂�����������
		 **/
		void Init()
		{
			rot_val	= 0;
			
			//�f�t�H���g�s��̏�����
			const float range = def_range;
			math::Matrix4x4 r,t,s;
			r.RotateY( rot );
			t.Translate( pos );
			s.Scale( math::Vector3(range,0.2f,range)  );			//�͈͂̃T�C�Y
			mat_default = r*s*t;
			select		= false; 
			
			mat_r.RotateY( rot );
			mat_t.Translate( pos );
			mat_sc.Scale( scale );
			mat_basket = mat_r*mat_sc*mat_t;
			//�{�[������擾
			left_pos1	= model->getBoneArray(1).local;				//���̕�
			right_pos1	= model->getBoneArray(2).local;
			center_pos1 = model->getBoneArray(3).local;
			forword_pos1 = model->getBoneArray(4).local;
			this->TransformVector();								//���[�J�����W��ϊ����Ă���		
			//�e���݂�ς��A�����Ŏ󂯎����rot�̌X���Ŕ����OBox��ς���		
			bobox1 = game::BoundingOBox3D( pos,math::Vector(scale.x*1.2f,scale.y*0.6f,0.25f),mat_r );
			//���T�C�h
			mat_r2.RotateY( rot+ 90*math::Math::PI/180 );			//rot�p�x�ɑ΂��Ă�90�x
			bobox2 = game::BoundingOBox3D( left_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
			bobox3 = game::BoundingOBox3D( right_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
			//�G�f�B�b�g�p����
			bobox4 = game::BoundingOBox3D( center_pos2,math::Vector(scale.x*1.2f,scale.y*0.6f,1.8f),mat_r );
	
			b_catch= game::BoundingOBox3D( forword_pos2,math::Vector(scale.x*0.8f,scale.y*0.8f,0.8f),mat_r );

			catch_flag = false;
		}

		/** �J�S�Ɋւ��郁�C��
		 * @param [out]	Key			&key	:�L�[�N���X
		 * @param [out] Ball		*_ball	:�{�[���N���X
		 * @param [out] Rule		*rule	:���[���N���X
		 **/
		void Main( input::Key &key,Ball *_ball,Rule *rule );
	
		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		//----- set get -------
		/** ���W�̎擾
		 * @return pos					:3�������W
		 **/
		math::Vector3 getPos(){ return pos; }
		
		/** ���W�̃Z�b�g
		 * @param [in]�@Vector	&pos	:3�������W
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** ��]�x���̎擾
		 * @return	rot					:��]
		 **/
		float getRot(){ return rot; }

		/** OBB����̎擾
		 * @return	bobox4				:�J�S�̐^�񒆂̔�����擾
		 **/
		game::BoundingOBox3D	getBoundingOBox(){ return bobox4; }
		
		/** ��]�̃Z�b�g
		 * @param	float _rot			:��]
		 **/
		void setRot( float _rot ){ rot = _rot; }
	
		/** �ڐG����
		 * @param [out]	Ball		*_ball	:�{�[���N���X
		 * @param [out] Rule		*rule	:���[���N���X
		 **/
		void Collision( Ball *_ball,Rule *rule );

		/** �T�C�h�̕ǂ̍��W�����[�J���Ȃ̂ŃO���[�o���ւ̐�p�̕ϊ� */
		void TransformVector();

		/** ���̎擾�t���O�̎擾 */
		bool getCatchFlag(){ return catch_flag; }

		/** �{�[������������ǂ����̃t���O���Z�b�g */
		void setCatchFlag( bool flag ){ catch_flag = flag; }
	
		/** �f�t�H���g�i���������j�̍��W�擾
		 * @return	default_pos			:����������3�������W
		 **/
		math::Vector3	getDefaultPos(){ return default_pos; }

		/** �X�P�[���̎擾
		 * @return scale				:3�����X�P�[��
		 **/
		math::Vector3	getScale(){ return scale; }
				
		/** �f�t�H���g�s��̎擾
		 * @return	���������ɍ쐬�����s��
		 **/
		math::Matrix4x4	getDefaultMatrix(){ return mat_default; }

		/** ����͈͂̔���̎擾
		 * @return		b_sphere	����͈͂̋�
		 **/
		game::BoundingSphere3D	getBoundingSphere(){ return b_sphere; }

		/** �G�f�B�b�g���̏�Ԃ̎擾
		 * @return	�I�����ꂽ���ǂ����̏��
		 **/
		bool			getSelect(){ return select; }
		 
		//--------- Set ------------
		
		/** �X�P�[�����O���Z�b�g����
		 * @param [in] Vector3	_scale	:3�����X�P�[��
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** ��]�l���Z�b�g����
		 * @param [in]
		 **/
		void setRotVal( float _rot_val ){ rot_val = _rot_val; }
		
		/** �G�f�B�b�g���̏�Ԃ̃Z�b�g
		 * @return	�I�����ꂽ���ǂ����̏��
		 **/
		void setSelect( bool _select ){ select =_select; }


		/** ��]�l�̎擾
		 * @return	rot_val		:��]��
		 **/
		float getRotVal(){ return rot_val; }		
	};

}}