/** @file
 * �X�^�W�A���N���X�̒�`
 * @date 2009/08/28
 * @author ����
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../game/Camera3D.hpp"
#include "Player.h"
#include "Ball.h"
#include "Rule.h"
#include "Bat.h"

namespace sugino{
/** �싅�ł̗̈� */
namespace baseball{
	
	//����`
	class Ground;
	class Rule;
	class PlayerBase;
	class BallBase;
	class Bat;

	/** �X�^�W�A���̊��N���X
	 * @note	�x�[�X��ϋq�Ȃ�
	 **/
	class StadiumBase
	{
	protected:
		/** ���W */
		math::Vector3		pos;
		
		/** �X�P�[�����O */
		math::Vector3		scale;

		/** ��] */
		float				rot;

		/** ���f�� */
		game::Model3D		*model;

		/** �}�l�[�W���[�Ǘ��̑I��̃e�N�X�`���C���[�W���Q�� */
		graphics::IImage	*img;

		
	public:
		/** �R���X�g���N�^
		 * @pragma [in] _pos		: ���W
		 * @pragma [in] _scale		: �k��
		 * @pragma [in] _rot		: �p�x
		 * @pragma [in] _model		: ���f���f�[�^
		 * @pragma [in] _img		: �e�N�X�`���f�[�^
		 **/
		StadiumBase( math::Vector3 &_pos, math::Vector3 &_scale, float _rot, game::Model3D *_model, graphics::IImage *_img )
		{
			pos		= _pos;
			scale	= _scale;
			rot		= _rot;
			model	= _model;
			img		= _img;
		}

		/** �f�X�g���N�^ */
		virtual ~StadiumBase(){}

		/** ������ */
		virtual void Init()=0;			 

		/** ���C������ */
		virtual void Main()=0;

		/** �\��
		 * IRender	*r				:�����_�[
		 * Matrix4x4	&vps		:�|�����킹���J�����s��
		 **/
		virtual void Draw( graphics::IRender *r,math::Matrix4x4 &vps )=0;

		//--------- Set ------------
		/** ���W�̃Z�b�g
		 * @param [in]	Vector3	_pos	:3�������W
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }

		/** �X�P�[���̃Z�b�g
		 * @param [in]	Vector3	_scale	:�X�P�[���T�C�Y
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** ��]�ʂ̃Z�b�g
		 * @param [in]	float _rot		:��]��
		 **/
		void setRot( float _rot ){ rot = _rot; }

		//--------- Get ------------
		/** 3�������W�̎擾
		 * @return pos					:3�������W
		 **/
		math::Vector3	getPos(){ return pos; }
		
		/** �X�P�[���T�C�Y�̎擾
		 * @return scale				:�X�P�[�����O�T�C�Y
		 **/
		math::Vector3	getScale(){ return scale; }

		/** ��]�ʂ̎擾
		 * @return rot					:��]��
		 **/
		float			getRot(){ return rot; }


		/** �ڐG����
		 * @param [out] BallBase *bbase	:�{�[���N���X
		 */
		virtual void Collision( Ball *bbase ) = 0;
	};
	//----------------------------------------------------------------------------------------------------
	
	/** �X�^�W�A��(����j�Ǘ��N���X */
	class StadiumManager{
	private:
		/** ���N���X��ێ� */
		std::list<StadiumBase *>			stadium;			

		/** �x�[�X�̃��f����ێ� */
		game::Model3D						mdl_base;
		
		/** �ϋq�Ȃ̃��f����ێ� */
		game::Model3D						mdl_audi;

		/** �h����̉摜�������ŊǗ� */
		system::AutoPtr<graphics::IImage>	img_base;
		system::AutoPtr<graphics::IImage>	img_audi;

	public:
		/** �R���X�g���N�^ */
		StadiumManager(){
			img_base	= NULL;
			img_audi	= NULL;
		}
		
		/** �f�X�g���N�^/��� */
		~StadiumManager(){ Dispose(); }
		
		/** Base��K�v��������
		 * @note new-s��StadiumBase�h���^�����������Ă���
		 **/
		void Init();

		/** ���C��
		 * @param [out]	Rule		*rule	:���[��
		 * @param [out] Ground		*g		:�O���E���h�N���X
		 * @param [out] Ball		*ball	:�{�[���N���X�̃N���X
		 **/
		void Main( Rule *rule,Ground *g,Ball *_ball );
		
		/** �\��
		 * @param [out]	IRender		*render	:�����_�[
		 * @param [out]	Matrix4x4	&vps	:�r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��@
	 	 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** ���X�g�̉�� */
		void Dispose();

		//get-------------------------------------------------------------
		/** ���X�g�̎擾
		 * @return	���N���X�̃��X�g
		 **/
		std::list< StadiumBase *> &getStadium(){ return stadium; }
			
		//set-------------------------------------------------------------
		/** ���X�g�̒ǉ�
		 * @param [in]	Stadiumbase	*_stadium	:���N���X
		 * @note "��̕�"�����X�g�֒ǉ�
		 **/
		void setStadium( StadiumBase *_stadium ){ stadium.push_back(_stadium); }
		
		/** ���f���̃Z�b�g
		 * @param [in] char		*model_name		:���f���̖��O
		 **/
		void setBaseModel( char *model_name ){ mdl_base.Load(model_name); }

		/** �e�N�X�`���̈ꊇ�Z�b�g
		 * @param [out]	IRender *render			:�����_�[
		 * @param [in]	char	*base_img		:�ۂ̃e�N�X�`��
		 * @param [in]	char	*audi_img		:�ϋq�Ȃ̃e�N�X�`��
		 **/
		void setImage( graphics::IRender *render,char *base_img,char *audi_img )
		{
			img_base	= render->createImage( base_img );
			img_audi	= render->createImage( audi_img );
		}

		/** ���f���̈ꊇ�Z�b�g */
		void setModel( char *base_model,char *audi_model )
		{
			mdl_base.Load( base_model );
			mdl_audi.Load( audi_model );
		}
	};

	//---------------------------------------------------------------------------
	//		StadiumBase�h���N���X
	//---------------------------------------------------------------------------
	
	/** �x�[�X�i�ہj�p�N���X(StadiumBase�h��) */
	class Base : public StadiumBase
	{
	private:

		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_base,mat_r,mat_sc,mat_t,mat_init;
		
		/** ��]�ʂ�ێ� */
		float								rot;
		
		/** �d�͒l��ێ� */
		float gravity;

		/** �s��̌v�Z */
		void calMatrix();
		
		/** �����ێ� */
		game::BoundingOBox3D				bobox;
	public:
		/** ������ **/
		Base( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,game::Model3D *_model, graphics::IImage *_img, float _gravity) :
		StadiumBase( _pos,_scale,_rot,_model,_img )
		{
			rot		= _rot;
			gravity = _gravity;
			//����
			mat_r.RotateY( _rot );
			bobox	= game::BoundingOBox3D( _pos,math::Vector3(_scale.x*0.5f,_scale.y*0.15f,_scale.z*0.5f),mat_r );			
		};
		
		void Init()
		{
			calMatrix();
		}

		/** �����i�[�Ɋւ��郁�C�� */
		void Main();
	
		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		 /** �ڐG����
		  * @param [out]	Ball	*bbase	:�{�[���N���X
		  **/
		 virtual void Collision( Ball *bbase );
	};
	//-----------------------------------------------------------------
	/** �ϋq�ȗp�N���X(StadiumBase�h��) */
	class Auditorium : public StadiumBase
	{
	private:

		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_audi,mat_r,mat_sc,mat_t,mat_init;
		
		/** ��]�ʂ�ێ� */
		float								rot;
		
		/** �s��̌v�Z */
		void calMatrix();
		
	public:
		/** ������ **/
		Auditorium( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,game::Model3D *_model, graphics::IImage *_img) :
		StadiumBase( _pos,_scale,_rot,_model,_img )
		{
			rot = _rot;
		};
		
		/** ������ */
		void Init()
		{
			calMatrix();
		}

		/** ���C�� */
		void Main();
	
		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** �����蔻��
		 * @param [out]	Ball	*bbase	:�{�[���N���X
		 **/
		 virtual void Collision( Ball *bbase ){};
	};

	//-------------------------------------------------------------------------------------
	/** �O���E���h�i�n��)�N���X */
	class Ground
	{
	private:
		int se_foul;

		/** �w�i�̃e�N�X�`���C���[�W��ێ� */
		system::AutoPtr<graphics::IImage>	img_ground;		//�n��
		system::AutoPtr<graphics::IImage>	img_wall;		//��
		
		/** ���f����ێ� */
		game::Model3D						model;

		/** �X�g���C�N�]�[���̔������ێ� */
		game::BoundingBox3D					bbox;

		/** ���ʂ�������ǂ̔����ێ� */
		game::BoundingOBox3D				bobox1;			//����2BH
		game::BoundingOBox3D				bobox2;			//������1BH
		game::BoundingOBox3D				bobox3;			//������3BH
		game::BoundingOBox3D				bobox4;			//������OUT
		game::BoundingOBox3D				bobox5;			//���t�g���z�[������	
		game::BoundingOBox3D				bobox6;			//���C�g���z�[������
		game::BoundingOBox3D				bobox7;			//�E����OUT
		game::BoundingOBox3D				bobox8;			//�E����3BH
		game::BoundingOBox3D				bobox9;			//�E����1BH
		game::BoundingOBox3D				bobox10;		//�E��2BH
		/** */
		void ResultInit();									//�z�u�Ȃǂ̏�����
		/** ����p�̍s�� */
		math::Matrix4x4						mat_osc[10],mat_rot[10],mat_obox[10];

		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_sc,mat_ground;	//sc:����������scale�s�� mat:Draw���̈ړ��s��
		
		/** �}�b�v���W��ێ� */
		math::Vector3						pos;

		/** �n�ʂ̔��肷��ׂɕK�v�Ȃ��̂�ێ� */
		game::ModelFace						*mf;
		
		/**�@����p�̃��C��ێ� */
		math::Vector3						ray;
		
		/** ���ʂ̏�Ԃ�ێ�
		 * @note 0:�����Ȃ�(�ʏ�q�b�g���j	1:�t�@�[��	2:���ʂ̕�
		 **/
		int									result_state;

		/** �ڐG����
		 * @param [in]	Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g 
		 * @param [in]	Vector3		&pos	:���肳�������I�u�W�F�N�g�̍��W
		 * @param [in]	Vector3		&rey	:���肳���������C�̕���
		 * @param [in]	float		size	:���肳����ׂ̔��a�T�C�Y
		 * @param [in]	ModelFace   **f		:�}�b�v�̖ʏ��@�����o��mf
		 * @param [in]	Vector3		*ret	:���C�̌�_�̈ʒu
		 * @param [in]	Vector3		*norm	:�@���x�N�g���̈ʒu
		 * @return		true:��������	false:�������ĂȂ�
		 **/
		bool Intersect( game::Object3D &map, math::Vector3 &pos, math::Vector3 &ray,float size,game::ModelFace **f,math::Vector3 *ret, math::Vector3 *norm );
	
		/** ���ƕǂƂ̐ڐG����
		 * @param [in]	Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
		 * @param [in]	Vector3		&pos1	:���̒��S���W
		 * @param [in]  float		size	:���肳���邽�߂͈̔̓T�C�Y
		 * @param [out] Vector3		*pos	:�����̕␳���W
		 * @note	�֐����ō��W��␳
		 **/
		bool IntersectSideWall( game::Object3D &map,math::Vector3 &pos1,float size,math::Vector3 *pos,math::Vector3 *dir);
	
		/** ���ƖʂƂ̈ʒu�֌W����
		 * @param [in]	Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
		 * @param [in]	Vector3		&pos	:���̒��S���W
		 * *	return	true:�ʂ̗�		false:�ʂ̕\	
		 * @note	�����ʂɑ΂��Ăǂ̈ʒu�ɂ��邩�Ŕ�����s��
		 **/
		bool IntersectFoul( game::Object3D &map,math::Vector3 &pos );
	
		bool						one_hit;			//�X�g���C�N�����̂ݔ���t���O

		/** ���[�J�����W���O���[�o�����W�ɕϊ� */
		void TransformVector( int num,math::Vector3 local );
			
		/** ���ƃJ�x�Ƃ̔���
		 * @param [in]	Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
		 * @param [in]	Vector3		&pos	:���̒��S���W
		 * @param [in]	float		size	:����T�C�Y
		 * @param [out]	ModelFace	**f		:���f���̖�
		 * @param [out] Vector3		*r		:��_
		 * @param [out]	Vector3		*n		:�@�� 
		 **/
		bool IntersectWall( game::Object3D &map,math::Vector3 &pos,float size,game::ModelFace**f,math::Vector3 *r,math::Vector3 *n );

	public:
		Ground()
		{
			result_state = 0;
		}

		/** ������
		 * @param [in]	char*		model_name		: �ǂݍ��ރ��f���t�@�C����
		 * @param [in]	char*		ground_img		: �����_�[��createImage�ō쐬�����f�[�^
		 * @param [in]  char*		wall_img		: �����_�[��createImage�ō쐬�����f�[�^
		 **/
		void Init( char *model_name, graphics::IImage *ground_img, graphics::IImage *wall_img );

		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** ���C��
		 * @param [in]	BallBase	*bbase		:�{�[���h����
		 * @param [out]	Bat			*_bat		:�o�b�g�N���X
		 * @param [in]	Rule		*_rule		:���[���N���X
		 **/
		void Main( Ball *_ball,Bat *_bat, Rule *_rule );
		
		/** �X�g���C�N�]�[������
		 * @param [out]	BallBase	*bbase		:�{�[���h����	
		 * @return	true:	�X�g���C�N�]�[���ʉ�	false:�ʉ߂��ĂȂ����{�[��
		 **/
		bool StrikeJudge( Ball *bbase );
		
		/** �e���N���X�Ƃ��̔h����֐��̔��菈��
		 * @param [out]	Rule		*rule		:���[���N���X
		 * @param [out]	StadiumBase	*base1		:�X�^�W�A�����N���X
		 * @param [out]	PlayerBase	*base2		:�v���C���[���N���X
		 * @param [out] BallBase	*base3		:�{�[���N���X
		 * @note	�e�}�l�[�W���̃��C���ŌĂ΂��֐��Ȃ̂ň����͈�C�ɓn�����Ɋe���ȊO�͂m�t�k�k�œn��
		 **/
		void IntersectFunction( Rule *rule,StadiumBase *base1,PlayerBase *base2,Ball	*base3);

		/** �z�u���W�擾(�J�����̏��������Ɏg�p) */
		math::Vector3 &getPos(){ return pos; }
		
		//------- set	/	get ------------
		/** ��񏈗��t���O�̎擾 */
		bool getOnehitFlag(){ return one_hit; }
		
		/** ��񏈗��t���O�̃Z�b�g */
		void setOnehitFlag( bool _one_hit ){ one_hit = _one_hit; }
		
		/** �O���E���h�Ō��ʏ�Ԃ̃Z�b�g
		 * @param [in]	int		state	:���
		 * @note	0:�����Ȃ�	1:�t�@�[�����
		 **/
		void setResultState( int state ){ result_state = state; }
		
		/** �O���E���h�œ����錋�ʂ̎擾
		 * @return	result_state	:�O���E���h�œ�������	0:�Ȃɂ��Ȃ�	1:�t�@�[��
		 **/
		int getResultState(){ return result_state; }
			
		/** �t�F���X����̎擾
		 * @param [out]	int	num	:�ǂ̃t�F���X�̔��肩
		 * @note 0:�g�p���Ȃ�	1:���t�g2BH���� 2:���t�g1BH���� 3:���t�g3BH����
		 * 4:���t�g�A�E�g����	5:���t�g�z�[���������� 6:���C�g�z�[����������
		 * 7:���C�g�A�E�g����	8:���C�g3BH���� 9:���C�g1BH����
		 * 10:���C�g2BH
		 * @return	�t�F���X�̔���
		 **/
		game::BoundingOBox3D	getBoundingObj( int num );
		
		/** �t�F���X�̐ڐG����
		 * @param [out] Ball	*bbase	:�{�[���N���X
		 * @return	�i�ۏ��� 0:�A�E�g�ɓ������� 1:�V���O���q�b�g�ɓ������� 
		 * 2:�c�[�x�[�X�q�b�g�ɓ������� 3:�X���[�x�[�X�q�b�g�ɓ�������
		 * 4:�z�[�������ɓ�������
		 **/
		int Collision( Ball *bbase );

	};

	//-----------------------------------------------------------------------------
	/** �X�J�C�h�[���N���X */
	class SkyDome
	{
	private:
		/** �e�N�X�`����ێ� */
		system::AutoPtr<graphics::IImage>	img_sky;		//��
		
		/** ���f����ێ� */
		game::Model3D						model;
	
		/** �s��ϊ��f�[�^��ێ� */
		math::Matrix4x4						mat_sc,mat_rot;	//mat_sc:����������scale�s��/mat_rot��]
		
		/** �}�b�v���W��ێ� */
		math::Vector3						pos;

		/** ��]�l(����)��ێ� */
		float								rot;
		
		/** ��]�ʂ�ێ� */
		float								rot_val;

	public:
		/** ������
		 * @param [in] name			: �ǂݍ��ރ��f���t�@�C����
		 * @param [in] sky_img	: �����_�[��createImage�ō쐬�����f�[�^
		 * @param [in] sky_img2	: �����_�[��createImage�ō쐬�����f�[�^
		 **/
		void Init( char *model_name, graphics::IImage *sky_img );
		
		/** �v�Z�Ȃ� */
		void Main();
		/** �\��
		 * @param [in] render : IRender�N���X��h�������N���X
		 * @param [in] camera : �r���[*�v���W�F�N�V����*�X�N���[�����|�����킹���s��
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** ��]�x���ɃZ�b�g���Ȃ���
		 *	@param [in] float speed		:��]���� 0�`1 
		 **/
		void setRotate( float speed ){ rot_val = speed; }
	};
}}