/** @file
 * �v���C���[(�I��j�N���X�̎���
 * @date 2009/09/04
 * @author ����
 **/

#include "Player.h"
#include "../game/BoundingObject.h"
#include "../game/Collision.h"

#include "Stadium.h"
#include "Ball.h"

#include "../DxLib.h"

using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;
using namespace input;
using namespace	system;



//�s�b�`���[�`�L���b�`���[�܂ł̃L�[�r�b�g�f�[�^�z��
int key_bit_array[30*60*60];
int pitch_data_buf[30*60*60];	//���������Ȃ炱��Ƀf�[�^���c���Ă���
int get_FC;						//�z��v�f�ԍ�
extern int g_cpuFC;

//�h����̊֐�
//�����i�[�̍s��v�Z
void Runner::calMatrix()
{
	rot += rot_val;
	mat_r.RotateY( rot );
	mat_sc.Scale( scale );
	mat_t.Translate( pos );
	mat_run = mat_r*mat_sc*mat_t;


}

//�����i�[�̊e�֐����Ăԃ��C��
/*
Key			&key	:�L�[�N���X
Ball		*_ball	:�{�[���N���X
Rule		*rule	:���[���N���X
Rule::STATE &p1state:�v���C���[�PP�̏��
*/
void Runner::Main( Key &key,Ball *_ball, Rule *rule, Rule::STATE &p1state )
{
	Collision( _ball,rule );		//�{�[���������i�[�ɓ��������̂Ȃ�A�E�g�ɂ��邽�߂̃R���W����
	calMatrix();
	pos.y-=gravity;
	//�����蔻��ւ̊֘A�t��
	bobox	= BoundingOBox3D( this->pos+Vector(0,0,-8),Vector(1.5f,scale.y*2.5f,0.25f),this->mat_r );
}

//�����i�[�̕`��
/*
IRender *render	:DirectX9�����_�����O
Matrix  &vps	:view.pos.scale�s��|�����킹
*/
void Runner::Draw( IRender *render,Matrix &vps )
{
	if( state )												//��ꂪ�o���\���
	{	
		render->setTexture( img );
		for( int i=0;i<model->getObjectCount();i++)
		{
			Object3D *obj = model->getObject(i);			//�Q�Ƃ������̂ŁA�󂯎��
		
			obj->Transform(this->mat_run, vps);				//�擾���ĕϊ�
			obj->Draw( render );							//�\��
			
		}
		render->setTexture( NULL );
	}
}

//�����i�[�̐ڐG����
/*
Ball *ball	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void Runner::Collision( Ball *ball,Rule *rule )
{
	//�h����̃{�[�����瓖���蔻��̏����擾
	BoundingSphere3D sphere = ball->getBoundingObj();
	if( this->getState() )										//�����i�[�o����
	{
		Vector	vret;
		if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
		{
			state = false;										//�A�E�g�ɂȂ�
			rule->setIsOutFlag(true );							//�A�E�g���o�̃t���O��ON��
		};
	}
}

//�s�b�`���[�̍s��v�Z
void Picher::calMatrix()
{
	mat_r.RotateX(rotate);
	mat_sc.Scale( 5,5,5 );
	mat_t.Translate( pos );
	mat_pich = mat_sc * mat_r * mat_t;
}

//�s�b�`���[�̊e�֐����Ăԃ��C��
/*
Key			&key		:�L�[�N���X�Q��
Ball		*_ball		:�{�[���N���X
Rule		*rule		:���[���N���X
Rule::STATE &p1state	:�PP�v���C���[�̏��
*/
void Picher::Main( Key &key,Ball *_ball,Rule *rule,Rule::STATE &p1state)
{
			//�L�[�f�[�^��z��
	key_bit_array[get_FC++] = key.getKeyData();			//�ω������Ȃǂ�z��ɕۑ����Ă���


	if( _ball->getStateFlag()==0 )									//�{�[�����o�����ĂȂ�(���̂ݏ������܂�)�i��㎞�̏��������܂ށj
	{
		key.gFc_Init();

		//_ball->setPos( Vector(0.1444f,1,-40.48f ));					//���ˍ��W���Z�b�g
		_ball->setPos( Vector(pos.x,1,pos.z -0.48f ));					//���ˍ��W���Z�b�g
		_ball->setTarget( Vector(pos.x,1.40f,-123.344f ) );			//���̖ړI�n���Z�b�g	
		_ball->setDirectValue( _ball->getTarget() - pos );			//���̕��o���������Z�b�g
		_ball->setMakyuFlag( false );
		

		if( p1state==Rule::DEFFENCE )									//�v���C���[1�����
		{
			if( key.isDown( Key::KEY_A))			//A�L�[�œ�����ʒu�ύX
			{							
				if( pos.x > -6.5f )  pos.x-=0.15f;
			}
			if( key.isDown( Key::KEY_D ))
			{
				 if( pos.x < 6.5f )pos.x+=0.15f;
			}

		}else if (p1state == Rule::ATTACK )
		{
			if( key.isDown( Key::SEMICORON))			//A�L�[�œ�����ʒu�ύX
			{							
				if( pos.x > -6.5f )  pos.x-=0.15f;
			}
			if( key.isDown( Key::BRACE ))
			{
				 if( pos.x < 6.5f )pos.x+=0.15f;
			}		
		}


		if( rule->ColAlpha() ) 
		{
			Shot( key,_ball,rule,p1state );		//����ʂ��o���ĂȂ����̂ݔ��ˊ֐�
		}

	}else if( _ball->getStateFlag()==1 )								//State�t���O�P:�o�����̏���(�����Ă�)
	{	

		_ball->setMakyuFlag( false );
		if( p1state==Rule::DEFFENCE )									//�v���C���[1�����
		{
			//S�L�[�Ŗ����n��
			if( key.isDown( Key::KEY_S) ) _ball->setMakyuFlag( true );		
		}else{
			// :�L�[�Ŗ���
			if( key.isDown( Key::CONMA ) ) _ball->setMakyuFlag( true );
		}

		//�L���b�`���[�ɓ�������
		if( _ball->BackNetCollision()==1 )
		{
			pos.x = 0;		//�s�b�`���[�̈ʒu�������ʒu��

			//���؃f�[�^�̕ۑ�///////////////////
			key_bit_array[get_FC] = -1;			//�f�[�^���[�ɏI�����Ƃ���-1�����
			//*pitch_data_buf = *key_bit_array;	//�ۑ�
			//�����f�[�^�������o���i�J���p�j
			FILE *fp=fopen("pitch_data.txt","w");
			//�t�@�C���ɏo��
			fprintf(fp,"{\n" );
			for(int i=0;i<60*30*30;i++)
			{
				if( key_bit_array[i] == -1 )break;		//���[�Ȃ甲����
				if(i%10 == 0) 			fprintf(fp,"\n" );	//10�������ނ��Ƃɉ��s
				fprintf(fp,"%d,",key_bit_array[i]);
					
			}
			fprintf(fp,"\n" );
			fprintf(fp,"}" );
			fclose(fp);	
			////////////////////////////////////////
			
			key.gFc_Init();
		}
	}

	HandMotion( _ball );							//�����郂�[�V������������
	calMatrix();
}

//��̉�]
/*
Ball *_ball	:�{�[���N���X
*/
void Picher::HandMotion( Ball *_ball )
{	
	if(rot_flag)
	{
		if(rot_reverse!=false)return;						//�����Ă��Ȃ���Ζ߂�
		rot_cnt--;											//��̉�]�J�E���g
		rot_val = -0.05f;
		rotate+= rot_val;

		if( rot_save+5 ==  rot_cnt ){
			_ball->setVelosity( speed );					//�������Z�b�g
			_ball->setStateFlag(1);							//�ʂ̏�Ԃ��u������v��
		}
		if( rot_cnt<0 ){ 									//��̉�]����]��������
			rot_reverse = true;								//��]���������t���O
			rot_flag = false;
		}
	}else{
		if(rot_reverse!=true)return;
		rot_cnt++;											//��̉�]�J�E���g��߂��Ă���
		rot_val = 0.05f;
		rotate+= rot_val;

		if( rot_cnt>59 )									//60�J�E���g����̃J�E���g����Ȃ̂�
		{
			rot_reverse = false;
			rotate = 0;
		}
	}
}

//�e�̔���
/*
Key			&key	:�L�[�N���X
Ball		*_ball	:�{�[���N���X
Rule		*rule	:���[���N���X
Rule::STATE &p1state:�v���C���[�̏��
*/
void Picher::Shot( Key &key,Ball *_ball,Rule *rule,Rule::STATE &p1state )
{
	

	if(rot_flag==false && rot_reverse==false )					//��]�J�n�t���O����]�߂�t���O���~��ĂȂ���΂Ȃ�Ȃ�
	{
		if( p1state==Rule::DEFFENCE )							//�v���C���[1������̎�
		{
			//���x�킯(�Œᑬ�`�ō����܂�)			
			if( key.isOn( Key::NUM_ONE )) speed = 0.75f;		    //�Œᑬ 1
			else if( key.isOn( Key::NUM_TWO )) speed = 0.85f;		//�ᑬ	2�L�[
			else if( key.isOn( Key::NUM_THREE )) speed = 1.12f;		//�����@3�L�[
			else if( key.isOn( Key::NUM_FOUR )) speed = 1.5f;		//�����@4�L�[
			else if( key.isOn( Key::NUM_FIVE )) speed = 1.85f;		//�ō���5�L�[
			
			if(speed)_ball->setVelosity(speed);

			if( key.isOn( Key::KEY_W)) rot_flag = true;		//�����]������
		}else if( p1state==Rule::ATTACK )						//�v���C���[2���f�B�t�F���X
		{
			if( key.isOn( Key::NUM_ZERO ))speed = 0.75f;		//�ᑬ	0�L�[
			else if( key.isOn( Key::ECORL ))speed = 0.85f;		//-
			else if( key.isOn( Key::CARET )) speed = 1.12f;		//^	
			else if( key.isOn( Key::YEN )) speed = 1.5f;		//\	
			else if( key.isOn( Key::BACKSPACE )) speed = 1.85f;		//�ᑬ	BS
			
			if(speed)_ball->setVelosity(speed);

			if( key.isOn( Key::AT_MARK))
			{
				rot_flag = true;
			}
		}
	}
}

//�s�b�`���[�̕`��
/*
IRender *render	:Direct3D9 �����_�����O
Matrix	&vps	:view.pos.scale
*/
void Picher::Draw( IRender *render, Matrix &vps )
{
	for( int i=0;i<model->getObjectCount();i++)
	{
		model->getObject(i)->Transform(this->mat_pich, vps);		//�擾���ĕϊ�
		render->setTexture(img);
		model->getObject(i)->Draw( render );						//�\��
		render->setTexture( NULL );
	}	
}

//�s�b�`���[�̐ڐG����
/*
Ball	*ball	:�{�[���N���X
*/
void Picher::Collision( Ball *ball )
{
	//�h����̃{�[�����瓖���蔻��̏����擾
	BoundingSphere3D sphere = ball->getBoundingObj();

	//������������
	Vector	vret;
	if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
	{
		ball->revisePos( vret );									//�␳
	};
}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̏�����
void PlayerManager::Init()
{
	//�����i�[��"3��"�ǉ�
	this->setPlayer( new_s Runner( Vector(39,1,-40),Vector(2.5f,2.5f,2.5f),(Math::PI*45)/180,0,&mdl_runner,img_runner1,0.098f,false ) );
	this->setPlayer( new_s Runner( Vector(-11,1,-10),Vector(2.5f,2.5f,2.5f),(Math::PI*-15)/180,0,&mdl_runner,img_runner2,0.098f,false) );
	this->setPlayer( new_s Runner( Vector(-44,1,-48),Vector(2.5f,2.5f,2.5f),(Math::PI*-115)/180,0,&mdl_runner,img_runner1,0.098f,false) );

	//�s�b�`���[�i���ˑ�)�̒ǉ�
	this->setPlayer( new_s Picher( Vector(0,0,-40),Vector(1,1,1),0,0,&mdl_picher,img_pich,0.098f,true ));
	
	//�����z��ۑ��p�t���[���J�E���g������
	get_FC = 0;
	g_cpuFC = 0;

	PlayerBase *three = this->getPlayerBase(1);
	std::list<PlayerBase *>::iterator i;
	i = player.begin();								
	while(i != player.end()){						
		PlayerBase *p = *i;
		p->Init();
		++i;
	}
	
	se_home_run = LoadSoundMem("data/se/gaya.mp3");			//�z�[�������̌��ʉ�
	se_hakushu = LoadSoundMem("data/se/hakusyu.mp3");		//����̌��ʉ�

}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̃��C��
/*
Key						*key		:�L�[�N���X
Ground					*g			:�O���E���h�N���X
Ball					*_ball		:�{�[���N���X
BasketManager			*baskM		:�G�f�B�b�g�̎���J�S�N���X
Rule *rule,Rule::STATE  &p1state	:�v���C���[�̏��
*/
void PlayerManager::Main( Key *key,Ground *g,Ball *_ball,BasketManager *baskM,Rule *rule,Rule::STATE &p1state )
{	
	CalColor();			//�A���t�@�l�̌v�Z
	CalPattern();
	PlayerBase *three = this->getPlayerBase(1);
	std::list<PlayerBase *>::iterator i;
	i = player.begin();								
	while(i != player.end()){						
		PlayerBase *p = *i;
		g->IntersectFunction( NULL,NULL,p,NULL );		//�O���E���h�Ƃ̔���
		
		if( rule->getForeballFlag() )						//�t�H�A�{�[�����̏���
		{	
			if( three->getState() )							//3�ۂ��璲�׃t���O�������Ă����̂Ȃ�
			{
				rule->setAddScoreFlag(true);				//�����o���_
			}
			if( setPlayerState(0,true) )					//�S�ے��ׂ���Ȃ玟�̗ۂ̃t���O�𗧂ĂĂĂ���
			{
				rule->setForeballFlag( false );
			}
			rule->setForeballFlag( false );
		}
		//�ʏ�q�b�g�̎��B�J�S������ĂȂ��A�Ȃ����O�����h��ŁA�~�܂����Ƃ�,�������J�S����΂Ɏ��Ȃ��z�[�����̓A�E�g�Ƃ���
		else if( rule->getNormalHitFlag() && g->getResultState()==0 && baskM->getCatchFlag()==false && !rule->InField(_ball)  )	
		{
			SingleHitGoRunner( _ball, rule );						//�ۂ�i�߂�
			rule->setNormalHitFlag( false );
			rule->setHitFlag( true );												//�J�E���g�����������t���O
			PlaySoundMem(se_hakushu, DX_PLAYTYPE_BACK );
			//PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); 
		}
		//�t�F���X����
		else if( rule->getFenceHitFlag() && !rule->getNormalHitFlag() )				//�t�F���X�ɂ�錋�ʂ�������ǂ̎�
		{
			int n = g->Collision( _ball );							//�t�F���X�̏����擾
			if( n==1 ) SingleHitGoRunner( _ball,rule );
			else if( n==2 ) TwoBaseHitGoRunner( _ball,rule );
			else if( n==3 ) ThreeBaseHitGoRunner( _ball,rule );
			else if( n==4 ) HomeRunGoRunner( _ball,rule );
			else rule->setIsOutFlag( true );										//�A�E�g�̃t�F���X�Ȃ̂ŃA�E�g�����t���O�𗧂Ă�
			rule->setFenceHitFlag( false );
			rule->setHitFlag( true );												//�J�E���g�����������t���O
			
		}
		p->Main( *key,_ball,rule,p1state );		//�h�����Main
		++i;
	}
}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̕`��
/*
IRender		*render	:�����_�����O�N���X
Camera3D	&cam	:�J�����N���X
Bat			&_bat	:�o�b�g�N���X
*/
void PlayerManager::Draw( IRender *render,Camera3D &cam,Bat &_bat )
{	
	std::list<PlayerBase *>::iterator i;
	i = player.begin();							//�擪(begin)��
	while(i != player.end()){					//�I���܂�
		PlayerBase *p = *i;
		p->Draw(render, cam.getMatrix() );					//�h����̕`��
		++i;
	}
	
	int right_type=0;
	if( _bat.getBatter() == Bat::RIGHT ) right_type = -493;	//�ŐȕύX���̍��W

	//�����i�[���
	render->drawImage( 493+right_type,350,128,128,img_minirun,0,0 );
	render->drawImage( 493+right_type,350,128,128,img_minirun,mini_pat*128,0,alpha );
	render->setTexture(NULL);


}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̉��
void PlayerManager::Dispose()
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();					//���X�g�܂킷
	while(i != player.end()){
		delete (*i);
		i = player.erase(i);
	}
}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̉�ʉE���̃����i�[�̃p�^�[���v�Z
void PlayerManager::CalPattern()
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z��Ŏ��̉�
		++i;
		++j;
	}
	//�O�ۂ��璲�׃p�^�[�������߂�	
	if( base[2]->getState() )
	{
		mini_pat =3;					//�O�ۃp�^�[���m��
		if( base[1]->getState() ){		//2�A3�ۃp�^�[���m��
			mini_pat = 6;
			if( base[0]->getState() ){	//���ۃp�^�[���m��
				mini_pat = 7;
			}
		}
		else if( base[0]->getState() )
		{
			mini_pat = 5;
		}
	}
	else if( base[1]->getState() )
	{
		mini_pat =2;		//2�ۃp�^�[���m��
		if( base[0]->getState() ) mini_pat = 4;
	}
	else if( base[0]->getState() )
	{
		mini_pat =1;		//1�ۊm�� 
	}
	else mini_pat = 0;		//�ۂȂ�
}

//�s�b�`���[�ƃ����i�[���܂Ƃ߂�}�l�[�W���[�̐F�v�Z
void PlayerManager::CalColor()
{
	Color col( alpha );
	if( alpha_flag==false )
	{
		col.a+=0.008f;
		if( col.a>=1.0f ) alpha_flag = true;
	}
	else if( alpha_flag==true )
	{
		col.a-=0.008f;
		if( col.a<=0 ) alpha_flag = false;
	}	
	
	alpha = col;
}

//�h����N���X�̎擾
/*
int here	:�ړI�̃}�l�[�W���[�Ǘ��̃I�u�W�F�N�g�܂�
*/
PlayerBase *PlayerManager::getPlayerBase( int here )
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	for( int j=0;j<=here;j++)
	{
		++i;
	}
	PlayerBase *p = *i;		//���̉�
	return p;
}

//�V���O���q�b�g
/*
Ball *bbase	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void PlayerManager::SingleHitGoRunner( Ball *bbase,Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z��Ŏ��̉�
		++i;
		++j;
	}
	//�O�ۑ�����t���O�𒲂ׁA�e�ۂ̏���������
	if( base[2]->getState() )
	{
		if( base[1]->getState() )
		{
			base[1]->setState(false);
			base[2]->setState(true);			//2�ۂɂ��Ȃ������������
		}else base[2]->setState(false); 
		if( base[0]->getState() ) base[1]->setState(true);			//1�ۂɂ͂���
		rule->setAddScoreFlag( true );								//���_���Z
	}
	else if( base[1]->getState() ){
		base[2]->setState(true);									//3�ۂ�
		if( !base[0]->getState()==1 )base[1]->setState(false);		//1�ۂɂ��Ȃ������������
	}
	else if( base[0]->getState() ){
		base[1]->setState(true);									//�����i�[1,2�ۂ�
	}
	base[0]->setState(true);										//�K�����̂ōŌ�

	//�z��ƃ��X�g�֘A�t������
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//�z������X�g��
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );
}

//�c�[�x�[�X
/*
Ball *bbase	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void PlayerManager::TwoBaseHitGoRunner( Ball *bbase, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z������̉�
		++i;
		++j;
	}
	//�O�ۑ�����t���O�𒲂ׁA�e�ۂ̏���
	if( base[2]->getState() )
	{
		if( base[1]->getState() ){							//2�ۂɂ���
			rule->setPoint( 2 );							//2�_���Z������
			base[1]->setState(false);					
		}
		if( base[0]->getState() )
		{
			base[0]->setState(false);
		}else base[2]->setState(false);						//1�ۂɂ��Ȃ������������
		rule->setAddScoreFlag(true);
	}
	else if( base[1]->getState() )
	{
		if( base[0]->getState() ) base[2]->setState( true );
		rule->setAddScoreFlag(true);						//���g�͗��Ă��܂܁A���_�͉��Z
	}
	else if( base[0]->getState() )
	{
		base[0]->setState(false);
		base[2]->setState(true);							//�����i�[�R�ۂ�
	}
	base[1]->setState(true);								//�K�����̂ōŌ�
	//�z��ƃ��X�g�֘A�t������
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];										//�z������X�g��
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );								//�s���I��
}

//�X���[�x�[�X
/*
Ball *bbase	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void PlayerManager::ThreeBaseHitGoRunner( Ball *bbase, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z������̉�
		++i;
		++j;
	}
	//�O�ۑ�����t���O�𒲂ׁA�e�ۂ̏���
	if( base[2]->getState() )			//���g�͗��Ă��܂�
	{
		if( base[1]->getState() )		//2�ۂɂ�����
		{
			rule->setPoint(2);			//��_����
			if( base[0]->getState() ){
				rule->setPoint(3);		//���ۂ̎�
				base[0]->setState(false);
			}
			base[1]->setState(false);	//���Ȃ�����
		}
		if( base[0]->getState() )		//1-3�ۂɂ�����
		{
			rule->setPoint(2);			//��_����
			base[0]->setState(false);	//���Ȃ�����
		}
		rule->setAddScoreFlag(true);	//���Z�t���O�𗧂Ă�
	}
	else if( base[1]->getState() )
	{
		if( base[0]->getState() )
		{
			rule->setPoint(2);			//��_����
			base[0]->setState(false);
		}
		 base[1]->setState(false);
		rule->setAddScoreFlag(true);	//���Z
	 }
	else if( base[0]->getState() )
	{
		base[0]->setState(false);		//1�ۂ͂��Ȃ��Ȃ�
		rule->setAddScoreFlag(true);	//���Z�t���O
	}

	base[2]->setState(1);				//�K�����̂ōŌ�
	//�z��ƃ��X�g�֘A�t������
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//�z������X�g��
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );			//�s���I��
}
//�z�[������
/*
Ball *bbase	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void PlayerManager::HomeRunGoRunner( Ball *_ball, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z������̉�
		++i;
		++j;
	}
	//�O�ۑ�����t���O�𒲂ׁA�e�ۂ̏���
	int n = 1;							//���_�@�K��1�_����̂łP
	if( base[2]->getState() ) n++;
	if( base[1]->getState() ) n++;
	if( base[0]->getState() ) n++;
	rule->setPoint(n);					//�ۂɂ������������Z

	//�����Ă����Ȃ�t���O�𕚂���
	if( base[2]->getState() ) base[2]->setState(false);
	if( base[1]->getState() ) base[1]->setState(false);
	if( base[0]->getState() ) base[0]->setState(false);
	
	rule->setAddScoreFlag(true);		//���Z�t���O�𗧂Ă�

	//�z��ƃ��X�g�֘A�t������
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//�z������X�g��
		++i;
		++j;
	}

	PlaySoundMem( se_home_run, DX_PLAYTYPE_BACK );
	_ball->setStateFlag( 4 );			//�s���I��
}

//3�A�E�g���̏���
void PlayerManager::DeleteRunner()
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//�z������̉�
		++i;
		++j;
	}
	//�����Ă����Ȃ�t���O�𕚂���
	if( base[2]->getState() ) base[2]->setState(false);
	if( base[1]->getState() ) base[1]->setState(false);
	if( base[0]->getState() ) base[0]->setState(false);
	
	//�z��ƃ��X�g�֘A�t������
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//�z������X�g��
		++i;
		++j;
	}
}

//�S���X�g�����ď�ԃt���O���������Ă��邩���ׂ�	�ۂɉ��l���邩�H
void PlayerManager::SarchPlayerState()
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		PlayerBase *p = *i;					//���̉�
		if( p->getState() )	
		{
			if( runner_cnt<3 ) runner_cnt++;	//�ۏ�ɂ��郉���i�[�̐�
		}
		++i;
	}
}

//�w�肵�����X�g�̏o����Ԃ𗧂Ă�
/*
unsigned int num	:�w��̃��X�g�ԍ�
bool		_state	:�o�����
*/
bool PlayerManager::setPlayerState( unsigned int num, bool _state )
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	for( unsigned int j=0;j<num;j++)
	{
		++i;						//�w��񐔂܂Ń��X�g��i�߂�
	}
	PlayerBase *p = *i;				//���̉�
	if( !p->getState() )			//�o����Ԃ������Ă�����
	{
		p->setState( _state );		//�o����Ԃ��Z�b�g���Ȃ���
		return	true;
	}
	if( num< 2 )					//�ۂ̖O�a�����B2�ȏ�̓s�b�`���[�Ȃ̂Ŏg�p���Ȃ�
	{
		num++;			
		if( setPlayerState( num,_state )) return true;	//���̃��X�g������Ȃ�ċA�I�Ɏ��̃��X�g��
	}
	return false;
}

//�J�S�̍s��v�Z
void Basket::calMatrix()
{
	rot += rot_val;

	this->mat_r.RotateY( rot );
	this->mat_r2.RotateY( rot+ 90*math::Math::PI/180 );	//90�x�̎��쐬
	this->mat_t.Translate( pos );
	this->mat_sc.Scale( scale );
	this->mat_basket = mat_r*mat_sc*mat_t;
}

//�J�S�̃��C��
/*
Key  &key	:�L�[�N���X
Ball *_base	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void Basket::Main( Key &key,Ball *_base,Rule *rule )
{
	this->TransformVector();
	//�����蔻��ւ̊֘A�t��
	bobox1	= BoundingOBox3D( pos,Vector(scale.x*1.2f,scale.y*0.6f,0.25f),mat_r );
	bobox2	= BoundingOBox3D( left_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
	bobox3	= BoundingOBox3D( right_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
	bobox4  = BoundingOBox3D( center_pos2,Vector(scale.x*1.2f,scale.y*0.6f,2.25f),mat_r );
	
	b_catch = BoundingOBox3D( forword_pos2,Vector(scale.x*0.7f,scale.y*0.7f,1.35f),mat_r );
	if( this->pos.y>0.5f )this->pos.y-=this->gravity;			//�d�͂�����
	this->Collision( _base,rule );								//�J�S�擾����
	calMatrix();
}

//�J�S�̕`��
/*
IRender		*render	:�����_�����O�N���X
Matrix4x4	&vps	:�J�����N���X
*/
void Basket::Draw( IRender *render, Matrix4x4 &vps )
{
	for( int i=0;i<model->getObjectCount();i++)
	{
		model->getObject(i)->Transform(this->mat_basket, vps);		//�擾���ĕϊ�
		render->setTexture( img );
		model->getObject(i)->Draw( render );						//�\��
		render->setTexture( NULL );
	}
}

//�J�S�̐ڐG����
/*
Ball *ball	:�{�[���N���X
Rule *rule	:���[���N���X
*/
void Basket::Collision( Ball *ball,Rule *rule )
{
	//�h����̃{�[�����瓖���蔻��̏����擾
	sphere = ball->getBoundingObj();
	float speed = ball->getVelosity();
	const float size = sphere.s;
	Vector	vret;

	if( speed )		//�{�[���̑��x������Ȃ�
	{
		if( Collision3D::Collision( sphere,b_catch) && Collision3D::Collision( sphere,bobox1) )	//�J�S�ƃ{�[���̔���
		{
			ball->setVelosity( 0 );			//�{�[���Ƃ߂�
			catch_flag =  true ;			
		}
		else if( Intersect3D::Intersect( sphere,bobox1,&vret) )
		{
			speed/=2;						//���x�ɂ߂�
			vret.Normalize();
			ball->setDirectValue(vret);		//���ˎ��Z�b�g
			ball->setVelosity(speed);
		}
		//�T�C�h�̕�
		else if( Intersect3D::Intersect( sphere,this->bobox2,&vret) )//��
		{
			Vector3 dir = center_pos2 - left_pos2;			//�J�S�̕����֕ω�������
			dir.Normalize();
			vret=dir;
			float ran = Math::rand()*2-1;					//�����ω���^����
			vret.x += ran;
			speed -= 0.035f;
			ball->setDirectValue( vret );		
			ball->setVelosity( speed );
		}
		else if( Intersect3D::Intersect( sphere,this->bobox3,&vret) )
		{		
			Vector3 dir = center_pos2 - left_pos2;			//�J�S�̕����֕ω�������
			dir.Normalize();
			vret=dir;
			float ran = Math::rand()*2-1;					//�����ω���^����
			vret.x += ran;
			speed -= 0.035f;
			ball->setDirectValue( vret );		
			ball->setVelosity( speed );
		}
	}else	//����0�̎�
	{		 
		if( !catch_flag && ball->getStateFlag()==2 )		//�ł��Ԃ���������ĂȂ��t�@�[���ł��Ȃ����
		{
			rule->setNormalHitFlag(true);					//�t�F���X�A�t�@�[���ȊO�ł̃q�b�g�͊m��
		}
	}
}

//�J�S�̃{�[�����W(���[�J��)����O���[�o�����W�ւ̕ϊ�
void Basket::TransformVector()
{
	Vector &v1 = left_pos1;						//�{�[��1�̍��̍��W

	Vector vv1;
	vv1 = mat_basket.TransformVector( v1 );		//�O���[�o�����W��
	left_pos2 = Vector(vv1.x,vv1.y,vv1.z);		//�V�������W�ɕϊ�

	Vector &v2 = right_pos1;				
	Vector vv2;
	vv2 = mat_basket.TransformVector( v2 );
	right_pos2 = Vector(vv2.x,vv2.y,vv2.z);

	Vector &v3 = center_pos1;				
	Vector vv3;
	vv3 = mat_basket.TransformVector( v3 );
	center_pos2 = Vector(vv3.x,vv3.y,vv3.z);

	Vector &v4 = forword_pos1;				
	Vector vv4;
	vv4 = mat_basket.TransformVector( v4 );
	forword_pos2 = Vector(vv4.x,vv4.y,vv4.z);

}

//�J�S���܂Ƃ߂�}�l�[�W���[�̏�����
void BasketManager::Init()
{	
	//����̃J�S��"7��"�ǉ��i�ߎ�Ɠ��菜��) ����͈͂Ɍ�������i��V�͍L�߂Ȃ�
	this->setBasket( new_s Basket( Vector(40,0,-49),Vector(4,9,4),0.6f,13,&mdl_basket,img_basket,0.098f ) );			//��
	this->setBasket( new_s Basket( Vector(25,0,-25),Vector(5.5f,9,5.5f),0.38f,17,&mdl_basket,img_basket,0.098f ) );		//��
	this->setBasket( new_s Basket( Vector(-30,0,-20),Vector(5.5f,9,5.5f),-0.399f,17,&mdl_basket,img_basket,0.098f ) );	//�V
	this->setBasket( new_s Basket( Vector(-40,0,-55),Vector(4,9,4),-0.66f,13,&mdl_basket,img_basket,0.098f ) );			//�O
	this->setBasket( new_s Basket( Vector(-60,0,40),Vector(5.5f,9,5.5f),-0.419f,26,&mdl_basket,img_basket,0.098f ) );			//��
	this->setBasket( new_s Basket( Vector(0,0,53),Vector(5.5f,9,5.5f),0.09f,30,&mdl_basket,img_basket,0.098f ) );				//��
	this->setBasket( new_s Basket( Vector(60,0,49),Vector(5.5f,9,5.5f),0.43f,26,&mdl_basket,img_basket,0.098f ) );			//�E

	std::list<Basket *>::iterator i;
	i = basket.begin();	
	while(i != basket.end())
	{
		Basket *b = *i;
		b->Init();
		++i;
	}
}

//�J�S���܂Ƃ߂�}�l�[�W���[�̃��C��
/*
Key		*key	:�L�[�N���X
Rule	*rule	:���[���N���X
Ground	*g		:�O���E���h�N���X
Ball	*ball	:�{�[���N���X
*/
void BasketManager::Main(  Key *key,Rule *rule,Ground *g,Ball *ball )
{
	catch_flag=false;					//���֐��ɓ��������ɏ�t���O�̏�����
	std::list<Basket *>::iterator i;
	i = basket.begin();									//i��擪(begin)��
	while(i != basket.end()){							//�I���܂�
		Basket *b = *i;
		b->Main( *key,ball,rule );			//�h�����Main���Ă΂��(Picher�p��Key���n��)
		if( b->getCatchFlag() )	catch_flag = true;		//�����J�S�ɓ����Ă�����}�l�[�W���[�̃t���O�𗧂ĂĂ���
		++i;
	}
}

//�J�S���܂Ƃ߂�}�l�[�W���[�̃��C��
/*
IRender		*render	:�����_�����O�N���X
Matrix4x4	&vps	:�J�����s��
*/
void BasketManager::Draw( IRender *render,Matrix4x4	&vps )
{
	std::list<Basket *>::iterator i;
	i = basket.begin();							
	while(i != basket.end()){						
		Basket *p = *i;
		p->Draw(render, vps);							//�h����̕`��
		++i;
	}
	render->setTexture(NULL);
}

//�J�S���܂Ƃ߂�}�l�[�W���[�̉��
void BasketManager::Dispose()
{
	std::list<Basket*>::iterator i;
	i = basket.begin();	
	while(i != basket.end()){
		delete (*i);
		i = basket.erase(i);
	}
}