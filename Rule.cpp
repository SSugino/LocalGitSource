/** @file
 * �싅�Ճ��[���N���X�̎���
 * @date 2009/10/15
 * @author ����
 **/
#include "Rule.h"
#include "../game/BoundingObject.h"
#include "Player.h" 
#include "Ball.h"
#include "Stadium.h"
#include "../baseball/Ball.h"



#include "../DxLib.h"

using namespace sugino;
using namespace baseball;
using namespace system;
using namespace	input;
using namespace graphics;
using namespace game;
using namespace math;


int end_bit_data[30*60*60];


//�񂪐i�񂾎��̏������B�A�E�g�J�E���g���S�ď�����������
/*
Ball	*bbase	:�{�[���N���X
Bat		&_bat	:�o�b�g�N���X
*/
void Rule::NextInit( baseball::Ball *bbase, Bat &_bat )
{
	strike_add_flag =false;
	ball_add_flag	=false;
	foreball_flag	=false;
	normal_hit_flag =false;
	hit_flag		=false;

	int i;
	for(i=0;i<sizeof(strikeArray)>>2;i++ ) strikeArray[i] = 0;
	for(i=0;i<sizeof(ballArray)>>2;i++ ) ballArray[i] = 0;
	for(i=0;i<sizeof(outArray)>>2;i++ ) outArray[i]= 0;
	ball = 0;
	strike = 0;
	out = 0;
	
	//�s�b�`���[�̋���������
	bbase->setVelosity( 0.75f );			//�ᑬ�ŏ�����
	//�o�b�^�[�̑Őȏ�����
	_bat.setBatter( _bat.LEFT );		//���ŐȂ�


	//�A���t�@�l�̏�����
	alpha	= 0xafFFFFFF;
	r_alpha = 0xdfFFFFFF;
}		

//�Ŏ҂��V�����Ȃ������ɏ���������
void Rule::BatterInit()
{
	strike_add_flag = false;
	ball_add_flag = false;
	
	//�A�E�g�ȊO�̃J�E���g������
	int i;
	for(i=0;i<sizeof(strikeArray)>>2;i++ ) strikeArray[i] = 0;
	for(i=0;i<sizeof(ballArray)>>2;i++ ) ballArray[i] = 0;
	ball = 0;
	strike = 0;
	r_alpha = 0xdfFFFFFF;
}

//��㏈��
/*
BasketManager	&bm		:�J�S�̊Ǘ��N���X
PlayerManager	&pm		:�v���C���[�̊Ǘ��N���X
Ball			*bbase	:�{�[���̊Ǘ��N���X
STATE			*p1		:��ڲ԰1P�̏��
STATE			*p2		:�v���C���[2P�̏��
*/
void Rule::Change( BasketManager &bm,PlayerManager &pm,Ball *bbase,STATE *p1,STATE *p2 )
{
	PlayerBase *p;
	if( change_flag )		//3�A�E�g�ɂ����t���O����������
	{
		//�G�f�B�b�g�Ŕz�u���ꂽ�J�S������������
		bm.Dispose();
		bm.Init();
		//�o�����Ă��������i�[�𕚂���
		pm.DeleteRunner();
		p = pm.getPlayerBase(2);				//���X�g2�Ԗڂ��擾��,(����)�������ēx�ݒ肵�Ȃ���
		p->setSpeed( 1.2f );					//���̉�̏��������x�̃Z�b�g

		if( ColAlpha() )						//Change�ƕ\�������Ȃ����A���t�@�l���P�𒴂���
		{
			//�U��̓���ւ�
			if( *p1==ATTACK ) *p1 = DEFFENCE;	//�U���������̂Ȃ�����
			else *p1 = ATTACK;

			if( *p2==ATTACK ){
				*p2 = DEFFENCE;
				cnt++;							//���i�߂�
			}
			else *p2 = ATTACK;
			
			change_flag = false;				//��x�ڂ͓���Ȃ��悤��
		}
	}
}

//�J�E���g�̃p�^�[���v�Z
void Rule::CalPattern()
{
	//�J�E���g����z�񒆐g�Ɗ֘A�t��
	if( strike>0 ) strikeArray[ strike ]	= 1;
	if( ball>0 )ballArray[ ball ]			= 1;
	if( out>0 ) outArray[ out ]				= 1;

	//���_�̃p�^�[�������߂�
	p1pat = p1score%10;		//1��
	p2pat = p2score%10;
	p1pat1 = p1score/10;	//10��
	p2pat1 = p2score/10;	

}

//Change�e�N�X�`���̃A���t�@�v�Z
bool Rule::ColAlpha()
{
	Color col( alpha );
	if( end_flag==false )
	{
		if( col.a>=1.0f ) return true;
		else col.a+=0.006f;
	}else col.a = 0;
	alpha = col;			//�X�V
	return false;
}

//�q�b�g�Ȃǂ̌��ʕ\���p�̃A���t�@�v�Z
/*
float add	:���Z��
*/
bool Rule::ColResultAlpha( float add )
{
	Color col( r_alpha );
	if( result )
	{
		if( alpha_cnt==3) return true;
		if( alpha_flag==false )
		{
			col.a-=add;
			if( col.a<=0 ) alpha_flag=true;
		}
		else if( alpha_flag==true )
		{
			col.a+=add;
			if( col.a>=1 )
			{		
				alpha_cnt++;
				alpha_flag=false;
			}
		}
		r_alpha = col;			//�X�V
	}
	return false;
}

//�A�E�g����
/*
Ball *bbase	:�{�[���N���X
Bat  *_bat	:�o�b�g�N���X
*/
void Rule::Out( Ball *bbase,Bat *_bat )
{
	if( is_out_flag )
	{
		result = 2;									//���[���N���X��ł̌���
		int n = bbase->getStateFlag();
		if( out<2 )
		{
			PlaySoundMem(se_out, DX_PLAYTYPE_BACK );
			out++;
			//�A�E�g�ƃ{�[���J�E���g�̏�����
			BatterInit();
			if( n==2 ) bbase->setStateFlag(4);		//�ł��Ă����̂Ȃ�J�������o
			else 
			{
				bbase->setStateFlag(0);			//�O�U�Ȃ̂ŉ��o�s�v
			}
		}
		else if( out == 2 )							//�R�A�E�g
		{
			change_flag = true;
			NextInit( bbase,*_bat );				//�J�E���g�̏�����
			PlaySoundMem(se_change, DX_PLAYTYPE_BACK );
			if( n==2 )bbase->setStateFlag(4);					//���̏o����Ԃ𕚂��遢
			else bbase->setStateFlag(0);
		}
		is_out_flag = false;
	}
}

//�J�E���g��������
/*
Ball	*bbase	:�{�[���N���X
Ground	*ground	:�O���E���h�N���X
Bat		&_bat	:�o�b�g�N���X
*/
void Rule::addCount( Ball *bbase,Ground *ground,Bat &_bat )
{
	if( bbase->BackNetCollision()==1 )							//�ڕW�n�_�ɓ��B�����Ƃ��ɔ���(�o�b�N�l�b�g)
	{
		//�O�U�A�E�g
		//�����������ĂȂ��A2�X�g���C�N�łȂ����A�܂��X�g���C�N�]�[����ʉ߂������A�J�E���g�𑝉��t���O
		if( strikeArray[2] ==1 && this->strike_add_flag )	
		{ 
			PlaySoundMem(se_out, DX_PLAYTYPE_BACK );
			is_out_flag = true;
		}
		//�t�H�A�{�[��
		else if( ballArray[3] ==1 && this->ball_add_flag )	
		{
			BatterInit();
			foreball_flag = true;
			result = 3;
			PlaySoundMem(se_foreball, DX_PLAYTYPE_BACK );
			bbase->setStateFlag(0);					//���̏o����Ԃ𕚂���
		}
		//�X�g���C�N
		else if( strike_add_flag  )
		{
			int st = strike;
			if( strike<2 )
			{
				strike++;			//�f�o�b�O�p�ɉ��Z���Ȃ�
				st = strike;
				PlaySoundMem(se_strike, DX_PLAYTYPE_BACK );
				bbase->setStateFlag(0);				//���̏o����Ԃ𕚂���@��
			}
		}
		//�{�[��
		else if( ball_add_flag )
		{
			if( ball<3 )
			{
				ball++;			//�f�o�b�O�p�ɉ��Z���Ȃ�
				PlaySoundMem(se_ball, DX_PLAYTYPE_BACK );
				bbase->setStateFlag(0);
			}
		}
	}
}

//���_���Z
/*
STATE *p1	:�v���C���[�PP���
*/
void Rule::AddScore( STATE *p1 )
{
	if( add_score_flag )							//���Z������������
	{
		if( *p1 == ATTACK )							//�v���C���[1���U���Ȃ�
		{
			p1score+=point;		//�ő�99�_�ȉ��Ȃ���Z
			if( p1score>=99 ) p1score = 99;			//�O�a���� 
		}else{
			p2score+=point;		//��
			if( p2score>=99 ) p2score = 99; 
		}
		add_score_flag = false;
	}else point = 1;								//��{�̉��Z���̓��_
}

//�ł�����̌���
/*
Ball			*bbase	:�{�[���N���X
Ground			*ground	:�O���E���h�N���X
BasketManager	&bm		:�J�S�Ǘ��N���X
PlayerManager	&pm		:�v���C���[�Ǘ��N���X
*/
void Rule::Result( Ball	*bbase	, Ground *ground,BasketManager &bm,PlayerManager &pm )
{
	std::list<Basket *>::iterator i;
	i = bm.getBasket().begin();						//i��擪(begin)��
	while( i != bm.getBasket().end() ){				//�I���܂�
		Basket *b = *i;
		this->BasketResult( bbase,ground,b );		//�J�S����
		this->HitResult( bbase,ground,b,pm );		//�q�b�g
		++i;
	}
}

//�J�S���{�[�����擾�������̌���
/*
Ball			*bbase	:�{�[���N���X
Ground			*ground	:�O���E���h�N���X
Basket			*bask	:�J�S�N���X
*/
void Rule::BasketResult( Ball	*bbase, Ground *ground,Basket *bask )
{
	if( bbase->getStateFlag()==0 )					//�{�[�����o�����ĂȂ�
	{
		bask->setCatchFlag( false );				//�{�[�������t���O�����낵�Ă���
	}
	else if( bbase->getStateFlag()==2 )				//�ł��Ԃ����Ƃ�
	{
		if( bask->getCatchFlag()  )					//�J�S�ɓ����Ă���
		{ 
			is_out_flag = true;
		}
	}
}

//�q�b�g��ł����Ƃ��̌���
/*
Ball			*bbase	:�{�[���N���X
Ground			*ground	:�O���E���h�N���X
Basket			*bask	:�J�S�N���X
PlayerManager	&pm		:�v���C���[�Ǘ��N���X
*/
void Rule::HitResult( Ball	*bbase, Ground *ground,Basket *bask, PlayerManager &pm )
{
	if( bbase->getStateFlag()==0 ){									//�{�[�����o�����ĂȂ��Ƃ�							
		normal_hit_flag = false;									//�ʏ�q�b�g�t���O�����낷
		fence_hit_flag	= false;									//�t�F���X�ɂ��q�b�g�t���O�����낷
	}
	else if( bbase->getStateFlag()==2 )								//�ł��Ԃ����Ƃ�
	{
		//�J�S�͎���ĂȂ����A���̃X�s�[�h���Ȃ��A�t�@�[���ł��Ȃ�,�t�H�A�{�[���ł��Ȃ��B�ʏ�q�b�g�̎�
		if(  !ground->getResultState() && this->foreball_flag==false )
		{
			int n;
			//�J�S������ĂȂ��A���̐�����0�ȉ��łȂ����ł����Ƃ��Ă��~�܂����悪�z�[���x�[�X�t�߂łȂ�
			if( !bask->getCatchFlag() && bbase->getVelosity()<=0  && !InField( bbase ))//�ʏ�q�b�g�̃t���O�𗧂Ă�
			{
				normal_hit_flag = true;
				PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); 
				result = 1;
			}
			else if( ( n = ground->Collision(bbase) ) > 0 )
			{
				//�p�^�[���ԍ������߂�
				if(n>=1 && n<=3 ){ result = 1;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }					//�q�b�g
				else if( n==4 ) result = 5;						//�z�[������
				
				if(n==1) {normal_hit_flag = true;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//�V���O���q�b�g
				else if(n==2){ fence_hit_flag = 2;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//�c�[�x�[�X�q�b�g
				else if(n==3){ fence_hit_flag = 3;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//�X���[�x�[�X�q�b�g
				else if(n==4){ fence_hit_flag = 4;PlaySoundMem(se_homerun, DX_PLAYTYPE_BACK ); }				//�z�[������
			}else if( 0==ground->Collision(bbase) )				//�A�E�g�t�F���X
			{
				is_out_flag = true;								//�A�E�g�t���O�𗧂Ă� Out�֐���

			}
			else if( InField( bbase ))							//�����̎��Ȃ��ꏊ�Ȃ�
			{
				is_out_flag = true;								//�A�E�g�t���O�𗧂Ă� Out�֐���
			}
		}
	}
}

//������
/*
IImage *count_img	:�����̃e�N�X�`��
IImage *result_img	:���ʂ̃e�N�X�`��
IImage *score_img	:�X�R�A�̃e�N�X�`��
IImage *side_img	:�`���U���̃e�N�X�`��
IImage *change_img	:���̃e�N�X�`��
*/
void Rule::Init( IImage *count_img, IImage *result_img, IImage *score_img,IImage *side_img,IImage *change_img )
{
	img_count	= count_img;
	img_result	= result_img;
	img_score	= score_img;
	img_cntside	= side_img;
	img_change	= change_img;

	//�A���t�@�l�̏�����
	alpha = 0xafFFFFFF;
	r_alpha = 0xffFFFFFF;

	//�e����ʉ�
	se_strike = LoadSoundMem( "data/se/strike.wav" );
	se_ball = LoadSoundMem( "data/se/ball.wav" ); 
	se_hit = LoadSoundMem( "data/se/hit.wav" );
	se_out = LoadSoundMem( "data/se/out.wav" );
	se_foreball = LoadSoundMem( "data/se/foreball.wav" );
	se_change = LoadSoundMem( "data/se/change.wav" );
	se_homerun = LoadSoundMem( "data/se/homerun.wav" );
	
}

//�Q�[���Z�b�g����(�����I��)
/*
STATE *p1	:�v���C���[�PP�̏��
*/
int Rule::GameSet( STATE *p1 )
{
	if( change_flag )//���̃t���O�������Ă����Ȃ�
	{
		//9��ȏォ��A�E�g�ɂȂ�����
		if( 9<=cnt && cnt<=12 )
		{
			//�v���C���[1���U�����Ɂi��U�j���������ɏ����Ă���  =  ��U�̍U����
			if( *p1==DEFFENCE && p1score>p2score )
			{
				end_flag = true;
				return 6;			//��U�̏���
			}
			else if( p1score<p2score )
			{
				end_flag = true;
				return 7;			//��U�̏���
			}else if( p1score==p2score && cnt==12 && *p1==DEFFENCE ) //��������
			{
				end_flag = true;
				return 8;
			}
		}
	}
	return 0;
}

//BallState��0�̂Ƃ��̏�����
void Rule::isReturn()
{
	result = 0;
	r_alpha = 0xdfFFFFFF;
	alpha_cnt = 0;
	home_x = 645;						
	home_mov = 2;
	//�Ƃɂ����q�b�g��������(�ʏ�q�b�g�A�t�F���X����ɂ��q�b�g�j
	if( hit_flag )
	{
		//�A�E�g�ƃ{�[���J�E���g�̏�����
		strike= 0;
		ball = 0;
		int i;
		for(i=0;i<sizeof(strikeArray)>>2;i++ ) strikeArray[i] = 0;
		for(i=0;i<sizeof(ballArray)>>2;i++ ) ballArray[i] = 0;
		hit_flag =false;
	}

	ball_add_flag=false;
	strike_add_flag=false;
	normal_hit_flag=false;
}

//�e�������Ăԃ��C��.
/*
Ball			*bbase	:�{�[���N���X
Ground			*ground	:�O���E���h�N���X
BasketManager	&bm		:�J�S�Ǘ��N���X
PlayerManager	&pm		:�v���C���[�Ǘ��N���X
Bat				&_bat	:�o�b�g�N���X
STATE			*p1		:��ڲ԰1P�̏��
STATE			*p2		:��ڲ԰2P�̏��
int				*scene_f:�V�[���̏��
*/
void Rule::Main( Ball *bbase,Ground *ground, baseball::BasketManager &bm,
				 PlayerManager &pm,Bat &_bat,STATE *p1,STATE *p2,int *scene_f )
{
	state	= *p1;										//�U��̏�Ԃ��֘A�t���Ă���
	int f	=0;											//�Q�[���Z�b�g�̃t���O
	
	Out( bbase,&_bat );									//�A�E�g�̏���
	if( ( f=GameSet(p1) )==0 )							//�������I����ĂȂ����
	{
		Change( bm,pm,bbase,p1,p2 );					//�����Ŏ󂯎�����v���C���[�̍U���Ԃ����,���ɂ����鏉������
		
		if( bbase->getStateFlag()==0 )					//�{�[�����Ȃ��Ԃ̓t���O�ނ����������Ă���
		{
			isReturn();									//�J�E���g�Ƃ͕ʂ̏�����
		}else if( bbase->getStateFlag()==4 )
		{
			if( result== 5 ) DrawMove();				//�z�[���������̉摜�ړ�
		}

		Result( bbase,ground,bm,pm );					//�J�S���{�[����������Ƃ��̔���

		addCount( bbase,ground,_bat );					//�J�E���g����
		
		CalPattern();									//�J�E���g�̃p�^�[���v�Z

		AddScore( p1 );									//���_���Z
	}else{												//�I������̂Ō��ʔ��\��
		*scene_f = f;									//�߂�l���V�[���̃t���O�ɑ������
	}
}

//�ł������ʂ��z�[�����̔���
/*
Ball *ball	:�{�[���N���X
*/
bool Rule::InField( Ball *ball )
{
	if( ball->getStateFlag()==2 )
	{
		if( ball->getVelosity()==0 )	//��������������
		{
			//�x�[�X�ɋ߂����Ȃ�
			if( Collision3D::Collision( b_sphere,ball->getBoundingObj() ))
			{
				int n= getResult();
				if( n!=4 || n==0 ) //�t�@-���łȂ�����
				{
					return true;		//�A�E�g
				}
			}
		}
	}
	return false;
}

//�`��̈ړ�
bool Rule::DrawMove()
{
	if( result==5 )
	{
		if( alpha_cnt==3) return true;
		home_x -= home_mov;
		if( home_x+256 <= 0 )
		{	
			alpha_cnt++;			//Move��������Ă邱�Ƃ�Ă΂��ꏊ��ColAlpha�Ɠ����Ȃ̂Ŏg���܂킷
			home_x = 645;
		}
	}
	return false;
}

//�`��
/*
IRender		*render	:�����_�����O�N���X
Matrix4x4	&vps	:�J�����s��
Bat			&_bat	:�o�b�g�N���X�Q��
*/
void Rule::Draw( IRender *render, Matrix4x4 &vps,Bat &_bat )
{
	//�\���ꏊ������
	const int st_x=510,st_y=370;				//�X�g���C�N���W
	const int ba_x=st_x,ba_y=st_y+30;			//�{�[�����W
	const int ou_x=st_x,ou_y=st_y+60;			//�A�E�g���W
	const int sc_x=256,sc_y = 62;				//�X�R�A���W
	
	int right_type = 0;							//�E�ŐȎ��̈ړ�
	if( _bat.getBatter() == Bat::RIGHT )
	{
		right_type = -490;
	}

	//�X�g���C�N
	render->drawImage( st_x+right_type,st_y,	32,32,img_count,strikeArray[1]*32,0 );			
	render->drawImage( st_x+30+right_type,st_y,32,32,img_count,strikeArray[2]*32,0 );	
	
	//�{�[��
	render->drawImage( ba_x+right_type,ba_y,	32,32,img_count,ballArray[1]*64,0 );	
	render->drawImage( ba_x+30+right_type,ba_y,32,32,img_count,ballArray[2]*64,0 );	
	render->drawImage( ba_x+60+right_type,ba_y,32,32,img_count,ballArray[3]*64,0 );	
	
	//�A�E�g
	render->drawImage( ou_x+right_type,ou_y,	32,32,img_count,outArray[1]*96,0 );
	render->drawImage( ou_x+30+right_type,ou_y,32,32,img_count,outArray[2]*96,0 );	
	
	render->drawImage( sc_x,sc_y,32,32,img_score,p1pat*32,0);		//�v���C���[�P�̃X�R�A
	render->drawImage( sc_x-16,sc_y,32,32,img_score,(p1pat1*32),0);

	render->drawImage( sc_x+35,sc_y,32,32,img_score,0,32);			//��؂�
	
	render->drawImage( sc_x+82,sc_y,32,32,img_score,p2pat*32,0);	//�v���C���[�Q�̃X�R�A	
	render->drawImage( sc_x+66,sc_y,32,32,img_score,p2pat1*32,0);	
	
	//�U�����Ɂ@�Q�@��\��
	if( state == ATTACK )											//�v���C���[�P���U���Ȃ�
	{
		render->drawImage( sc_x-8,sc_y+18,32,32,img_score,32,32);
		render->drawImage( 290,38,32,32,img_cntside,32,32);			//���\�\��
	}else{
		render->drawImage( sc_x+76,sc_y+18,32,32,img_score,32,32);
		render->drawImage( 290,38,32,32,img_cntside,64,32);	
	}
	
	//���\��
	render->drawImage( 270,5,32,32,img_cntside,64+(cnt*32),0);	
	render->drawImage( 300,5,32,32,img_cntside,0,0);
	
	//���
	if( change_flag && end_flag ==false ) 
	{
		render->drawImage( 180,130,256,256,img_change,0,0,alpha);
	}
	
	//�A�E�g��q�b�g�Ȃǂ̕\��
	if( 0<result && result<5 ) render->drawImage( 180,130,256,128,this->img_result,0,128*result,r_alpha);
	else if( result==5 )render->drawImage( home_x,130,256,128,this->img_result,0,128*result,0xffFFFFFF);

}