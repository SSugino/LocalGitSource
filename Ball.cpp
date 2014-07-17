/** @file
 * �{�[���N���X�̎���
 * @date 2009/09/14
 * @author ����
 **/

#include "Ball.h"
#include "../game/BoundingObject.h"

using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;
using namespace input;

//�{�[���̈ړ����Z
/*
const Vector	&dir		:�ړ������鎲
const float		_velosity	:�ړ���
*/
void Ball::addMove( const Vector &dir, const float _velosity )
{
	if( _velosity>0 && _velosity<0 )				//���x0�ȊO�A���x������Ȃ�
	{
		//�ړ����x�̖O�a����
		if( _velosity >= max_velosity ) velosity = max_velosity;//�����̑��x�����̍ő呬�x�𒴂��Ă����ꍇ�A�K�葬�x���ő��
		if( _velosity <= min_velosity ) velosity = min_velosity;//�����̑��x�����̍Œᑬ�x�𒴂��Ă����ꍇ�A�K�葬�x���Œ��

		this->pos += (dir*_velosity);							//�ړ��ʉ��Z
		sphere.p = pos;											//�R���W�������̍��W�����̍��W�Ɗ֘A�t��
		sphere.s = size;										//�R���W�������̃T�C�Y�����̃T�C�Y�Ɗ֘A�t��
	}else velosity = 0;
}

//�{�[���̉�]���Z
/*
float add_rot		:��]��
*/
void Ball::addRotate( float add_rot )
{
	if( add_rot>1.0f ) add_rot = 1;					//��]�ʂP�ȏ�Ȃ�1��
	rotate += (add_rot/((size+size)*Math::PI));		//��]�v�Z
}

//�e�֐����Ăԃ��C��
/*
Ground			*g			:�O���E���h�N���X
Rule *rule, Key *key		:�L�[�N���X
Rule::STATE		&p1state	:�v���C���[�PP�̏�ԎQ��
*/
void Ball::Main( Ground *g, Rule *rule, Key *key, Rule::STATE &p1state )
{
	g->IntersectFunction( rule,NULL,NULL,this );		//�O���E���h�̓����蔻��
	Move( *key,p1state );								//�s�b�`���[�̈ړ�����
}

//������
/*
char			*model_name:���f���̃p�X����
IImage			*ball_img  :�{�[���̃e�N�X�`��
*/
void Ball::Init( char* model_name,IImage *ball_img )
{
	sphere=BoundingSphere3D(pos,size);					//�R���W�����̋��Ƀ����o�̍��W�ƃT�C�Y��������
	model.Load( model_name );							//3D���f���̃��[�h
	img	= ball_img;										//�e�N�X�`�����������珉����

	pos		= Vector(0.1444f,1,-40.48f);				//�������W�����l������
	gravity = 0.098f;									//�d�͎��l������
	mass	= 1.6f;										//���ʎ��l������							
	size	= 1.5f;										//���̃T�C�Y���l������
}

//�{�[���̈ړ�
/*
Key				&key		:�L�[�N���X�Q��
Rule::STATE		&p1state	:�v���C���[1P�̏�ԎQ��
*/
void Ball::Move( Key &key,Rule::STATE &p1state )
{
	if( direct.Length() )this->direct.Normalize();		//����������Ȃ�������͏�ɒP�ʃx�N�g���ɂ���
	sphere.p = pos;										//������W�����̍��W�Ɗ֘A�t��
	sphere.s = size;				

	calMatrix();										//�s��v�Z
	if( state_flag )									//��Ԃɒl������Ƃ��̂ݏ���
	{
		if( velosity>0 ) rotate_val = ( max_velosity/velosity );//��]�ʂ��ő�X�s�[�h/���̃X�s�[�h�ŋ��߂�	

		//�ω��� D�A���@A�A���@�L�[ 
		if( p1state==Rule::DEFFENCE )					//�v���C���[�P�����������
		{
			if( key.isDown( Key::KEY_D ))	target.x +=0.35f;	//�ʂ̍��W�����炷
			if( key.isDown( Key::KEY_A ))	target.x -=0.35f;
		}
		else if( p1state == Rule::ATTACK )  //�v���C���[2�����������
		{
			if( key.isDown( Key::SEMICORON )) target.x -=0.35f;	//�ʂ̍��W�����炷
			if( key.isDown( Key::BRACE )) target.x +=0.35f;
		}

		//�������͖ڕW�Ɍ������Ĕ�΂�
		if( this->state_flag==1 )
		{
			Vector3 direct = (this->target - this->pos).Normalize();	//�ړI�n-�{�[��
			setDirectValue( direct );									//���������߂�
		}
		//�ł�����̑ŋ��Ɋւ���
		if( 1<=state_flag && state_flag<=3 )
		{
		//���̈ړ��ʂɂ���č��W�̍X�V�͂����̂�
		direct.y = 0;												//���˂�������Y�l�����邩��	
		this->pos.y += -( gravity*mass );							//�d�́�����
		//�ړ�
		this->addMove( direct,velosity );							//���̕����ɐi�܂���
		//���̉�]
		this->addRotate( rotate_val*direct.z );
		}
	}else															//state���O�@�܂蓊����O
	{
		pos = Vector(0.1444f,1,-40.48f );							//�Œ�l�ɍ��W���Z�b�g
		rotate = 0;													//��]�Ȃ�
		rotate_val = 0;												//��]�ʂȂ�
		velosity = 0;												//�ړ��ʂȂ�
	}
}

//�s��v�Z
void Ball::calMatrix()
{
	mat_sc.Scale(size,size,size);
	mat_t.Translate( pos );
	mat_r.RotateX( rotate );
	mat_ball = mat_r*mat_sc*mat_t;					//�s��v�Z
}

//�I���_�ɕt��������
bool Ball::BackNetCollision()
{	
	if( state_flag!=1 )return false;							//�������݂̂̔���
	//�o�b�N�l�b�g�ɓ��B
	if( Collision3D::Collision( sphere,bbox1 ) )				//�u�L���b�`���[�ɓ͂�����v���R���W��������̏���
	{
		return true;
	}
	return false;
}

//�`��
/*
IRender *render					:Direct9�����_�����O
sugino::math::Matrix4x4 &vps	:�v�Z���ꂽ�s��(view.pos.scale)
*/
void Ball::Draw( IRender *render, sugino::math::Matrix4x4 &vps )
{

	if( state_flag==0 )return;						//�{�[���o����
	
	//�e
	Matrix shadow,mm;
	shadow.Shadow( 1,1,sphere.s+0.5f );				//�e���o��
	mm.Translate( pos.x,pos.y-0.5f,pos.z );
	//�e��p�̕\��
	render->setTexture( NULL );						//�e�N�X�`�����͂�Ȃ�
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform(mm*shadow, vps);			//�e�ƃI�u�W�F�N�g�̊|�����킹
		model[i].setTColor( 0x45000000 );			//�e�̐F
		model[i].Draw( render );					//�e�̕\��
	}
	
	//�{�[���̕\��
	render->setTexture( img );
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform(mat_ball, vps);			//�擾���ĕϊ�
		if( !makyu_flag )model[i].Draw( render );	//������ԂłȂ���Ε\��
		render->setTexture( NULL );					//
	}
}