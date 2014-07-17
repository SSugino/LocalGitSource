/** @file
 * �o�b�g�N���X�̎���
 * @date 2009/08/30
 * @author ����
 **/

#include "Bat.h"
#include "../game/BoundingObject.h"
#include "../game/Collision.h"
#include "../game/Rebound.h"

#include "../DxLib.h"
using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;
using namespace input;

//������
/*
char	*model_name	:���f���̃p�X
IImage	*_img		:�e�N�X�`��
*/
void Bat::Init( char* model_name,IImage *_img )
{
	model.Load( model_name );			//���f���̃��[�h
	img = _img;							//�e�N�X�`���������o�ɃZ�b�g

	Vector rr;
	rot = 86.3f;						//������]�l
	rr.Rotate( Vector(0,1,0),rot );		//���̉�]�l�ŏ�����

	pos = math::Vector3(-10,0,-103);			//�}�b�v�ւ̔z�u���W������
	mat_sc.Scale( Vector(5.5f,5.5f,5.5f) );		//�e5�{
	mat_t.Translate( pos );
	mat_r.RotateY( rot );
	mat_init.Initialize( rr,Vector( 0,1,0 ),pos );	//�s�񏉊���
	mat_bat = mat_init;

	//����T�C�Y�̏�����
	col_pos = model.getBoneArray(1).local;		//�{�[��1�Ԗڂ̍��W�i�o�b�g�̐�)��ۑ�
	line.s = this->pos;							//�o�b�g�̌��_���X�^�[�g�ʒu�Ƃ���
	line.width = 1.1f;							//�R���W�����̒����������L�΂��B�i�{�[�����ɂ��ꉞ���Ă邽��
	
	//�ŐȂ̏�����
	type = LEFT;								//�ŐȂ������͉E�Ŏ҂ŃZ�b�g

	//�X���̏�����
	lean_rot = 3.8f;							//�X�����E�p�x
	lean_speed = 0.08f/4;						//�X������
	lean_flag = 0;
	lean_cnt  = 0;								//�X���J�E���g
	pow =0;										//�ł��Ԃ���
	rot_val = 0;

	se_swing = LoadSoundMem("data/se/swing.mp3");		//�X�C���O��
	se_hitting = LoadSoundMem("data/se/hitting.wav");	//�ł����Ƃ��̉�
	se_swing_f = false;							//�X�C���O���
}

//�s��v�Z
void Bat::calMatrix()
{		
	rot_val = rot + (lean_rot*lean_cnt);			//�X�C���O�̉�]�ʎ擾	

	mat_r.RotateY( rot_val );						//Y����]
	mat_sc.Scale( Vector(11.5f,10,11.5f) );			//���f���̃T�C�Y���ǂ�ȃ{�[�����������Ă��܂��悤�Ɍ�����̂ŏk��
	mat_t.Translate( pos );
	mat_bat = mat_sc*mat_r*mat_t;					//�s��X�V
}

//�o�b�g�N���X�e�֐����Ăԃ��C��
void Bat::Main( Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state )
{	
	calMatrix();													
	if( _rule->ColAlpha() )											//����ʂ��o���ĂȂ����̂�
	{
		for( int i=0;i<4;i++ )										//���[�v�ł܂킵�đ��x���グ��
		{
			calMatrix();											
			//C�L�[�܂���.�L�[�o�b�g��]
			Swing( key,_ball,_rule,p1state );						//�X�C���O�֐�
			col_pos2 = mat_bat.TransformVector( col_pos );			//�O���[�o�����W��
			line.e = col_pos2;										//�o�b�g��̍��W�𔻒�Ƃ��Ċ֘A�t��
			line.s = pos;
			Collision( _ball );										//�o�b�g�ƃ{�[���̔���
		}

		//�o�b�^�[�ŐȕύX
		//Z�L�[����.�ŉE�o�b�^�[��
		if( p1state==Rule::ATTACK )									//�v���C���[1P���U���̎�
		{
			if( key->isOn( Key::KEY_X ) )	setBatter( RIGHT );		//�E�ŐȂ�
			else if( key->isOn( Key::KEY_Z ) )setBatter( LEFT );	//���ŐȂ�
		}else{														//�v���C���[2P���U���̎�
			if( key->isOn( Key::SLASH ) ) setBatter( RIGHT );
			else if( key->isOn( Key::DOT ) )setBatter( LEFT );
		}
	}
}

//�X�E�B���O
/*
Key *key,Ball	*_ball		:�{�[���N���X
Rule			*_rule		:���[���̃N���X
Rule::STATE		&p1state	:�PP�̏�Ԃ��Q��
*/
void Bat::Swing(  Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state )
{
	if( ( key->isDown( Key::KEY_C)&&p1state==Rule::ATTACK)		//C�L�[�����ĂȂ����v���C���[�P���U���̎�
		||  ( key->isDown( Key::R_SHIFT )&&p1state==Rule::DEFFENCE) )	// . �L�[�����ĂȂ����v���C���[�P������̎�
	{
		pow = 0.5f;													//�{�^����������p���[�l0.5�������l�Ƃ���
		float cnt;
		cnt	 = lean_cnt;											//�����݂̃J�E���g��Ⴄ
		cnt += lean_speed;											//�J�E���g�ƃX�s�[�h�����Z���Ă���
		if( cnt <= 1.0f ){											//�v�Z���ʃJ�E���g��1����
			 lean_cnt =  cnt;										//�o�b�g����ʎ�O�։�]���Ă���
		}else lean_flag= 2;											//�߂�t���O��
	}else{															//�{�^���������ĂȂ��Ƃ��A��]���߂�
		float cnt=0;
		cnt = lean_cnt;
		if( pow <=1.5f ) pow += cnt;								//��]�J�E���g����o�b�g�̃p���[���߂�
		else if( pow>1.5f ) pow = 1.5f;								//�p���[�L�a���� �ő�1.5
		cnt -= lean_speed;											//��]���x���猻�݂̃J�E���g������
		if( cnt > 0 ){												//�v�Z���ʂ�0�ȏ�Ȃ�
			lean_cnt =cnt;											//�o�b�g����ʉ��։�]����(�X�C���O��)
			PlaySoundMem(se_swing, DX_PLAYTYPE_BACK );
		}else{														//�X����0�ȉ�	
			lean_cnt = 0;
			lean_flag= 0;
		}
	}
}

//�{�[���Ƃ̐ڐG����
/*
Ball	*_ball		:�{�[���̃N���X
*/
void Bat::Collision( Ball *_ball )
{
	BoundingSphere3D sphere = _ball->getBoundingObj();		//�������瓖���蔻��̏����擾
	//�o�b�g�ɓ�����������
	if( _ball->getStateFlag()!=1 )return;					//�{�[�����s�b�`���O���̂�
	Vector dir = _ball->getDirectValue();				//���˕Ԃ�O�̈ړ��ʂ�ۑ�
	if( Rebound::Refrect( sphere,line,&dir ) )			//��������
	{
		_ball->setMakyuFlag(false);						//������Ԃ��Ȃ���
		PlaySoundMem(se_hitting, DX_PLAYTYPE_BACK );
		float ran = Math::rand()*2-1;					//�����ω���^����
		float rot = _ball->getRotateValue();
		rot = -rot;
		_ball->setRotValue(rot);						//��]�����]
		dir.x += ran;									//���˕Ԃ������Ƀ����_������������
		
		//�s�񔽎˕����Z�����V���Ȉړ���
		_ball->setDirectValue( dir );					//���ˎ�������Ƃ���
		float speed = _ball->getVelosity();
		_ball->setVelosity( speed*this->pow );			//�X�C���O�́{����
		_ball->setStateFlag( 2 );						//�{�[���̏�Ԃ�"�ł���"��	
	}
}

//�`��
/*
IRender		*render	:Direct3D�̃����_�����O
Matrix4x4	&vps	:view.pos.scale�s��
*/
void Bat::Draw( IRender *render, Matrix4x4 &vps )
{
	PointLight3D light( Vector(0,1,0),Color(0,1,0),100000 );	//���C�e�B���O
	Light3D *lights[] = { &light };								//���C�g�f�[�^

	render->setTexture( img );			
	for(int i=0;i<model.getObjectCount();i++)		//���f�������`��
	{
		model[i].Transform( mat_bat,vps );
		model[i].Lighting( lights,1, Color(1,1,1,1) );
		model[i].Draw( render );
	}
	render->setTexture( NULL );	
	
	//�e
	Matrix shadow,mm;
	Vector dir;						//�����p�x�N�g��
	dir = pos-light.getPos();		//��������  ��������I�u�W�F�N�g�ɑ΂��Ẵx�N�g��
	
	float t = (dir.x/dir.y);		//�e�̊��������߂�
	if( t<0 ) t = -t;				//��Βl
	float t2 =(dir.z/dir.y);		//Z������
	if( t2<0 ) t2=-t2;
	
	shadow.Scale( 1,0,1 );			//�o�b�g�p�̉e�̃T�C�Y
	mm.Translate( 1,-0.05f,0 );
	
	//�e��p�̕\��
	render->setTexture( NULL );
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].setColor( 0x4f000000 );			//�e�̐F
		model[i].Transform(shadow*mat_bat, vps);	//�e�ƃI�u�W�F�N�g�̊|�����킹
		model[i].Draw( render );					//�o�b�g�p�A�̕\��
	}
}

//�ŐȂ̕ύX
/*
BATTER_TYPE _type	:�V�K�ɓ��ꂽ���ŐȂ̏��
*/
void Bat::setBatter( BATTER_TYPE _type )
{
	switch(_type){
		case LEFT : 
			pos = Vector3(-10,0,-103);
			line.s = pos;						//������W���X�V
			rot = 86.3f;
			lean_rot = 3.8f;					//���E�p�x�ݒ肵����
			type = _type;
			break;
		case RIGHT :
			pos = Vector( 8,0,-103);
			line.s = pos;
			rot = 180.53f;
			lean_rot = -3.8f;
			type = _type;
			break;
		default : throw system::Exception("�w�肳�ꂽ�񋓑̂�����܂���(BATTER_TYPE)");
	}
}