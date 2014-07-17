/** @file
 * �Q�[���̗V�ԕ����𓝊����镔���N���X�̎���
 * @date 2009/08/27
 * @author ����
 **/
#include "Playing.h"
#include "../scene/Winner.h"
#include "../scene/Title.h" 
#include "../game/BoundingObject.h"

#include "../DxLib.h"

using namespace sugino;
using namespace baseball;
using namespace system;
using namespace	input;
using namespace graphics;
using namespace game;
using namespace scen;
using namespace math;


enum BALL_STATE
{
	MOUND,			//�}�E���h��ł܂��������ĂȂ�
	THROW,			//�����ăL���b�`���[�Ɍ������Ă�Œ�
	CATCH,			//�L���b�`���[�����
	BATTING,		//�ł��Ԃ���
	RESULT,			//����
};

BALL_STATE	ball_state; 

void BallStateBegin();
void BallStateEnd();

//�X���b�h���C��
DWORD BaseBall::Run()
{
	//�Z�b�g�������f����e�N�X�`�����g���ď�����
	stadiumM.Init();
	playerM.Init();
	basketM.Init();

	//�J�����̏�����
	camera.setScreen( 640,480 );								//��ʃT�C�Y
	camera.setPercective( 1.3333f*30,1*30,60*1,10000 );			//�p�[�X
	camera.setLookAt( Vector(0,0,-35)*30 ,ground.getPos(),Vector(0,1,0) );	//���Ă���ꏊ�ƌ��Ă��镨
	
	//�����i�[�̃J�����̏�����
	angle.Init();

	return 0;
}

//�Q�[�����̐F�v�Z
void BaseBall::CalAlpha()
{
	Color col( load_al );
	col.a+=0.005f;
	if( col.a>=1.0f ) col.a = 1.0f;	
	load_al = col;
}

/** ������ */
void BaseBall::Init()
{
	//�X���b�h�Ŏg���p�̏�����
	load_bg = render->createImage( "data/graphic/loadingbg.png" );			//���[�f�B���O���̃e�N�X�`��
	load_f	= render->createImage( "data/graphic/nowload.png" );			//���[�f�B���O���̃e�N�X�`��
	load_al = 0x3FFFFFFF;
	load_wait =50;
	


	img_Helpkey = render->createImage( "data/graphic/Helpkey.png" );
	
	img_attacker = render->createImage( "data/graphic/attacker.png" );


	/** �e���f���̃��[�h/������ */
	//�O���E���h
	ground.Init( "data/model/RDX/new_map.RDX",render->createImage("data/graphic/ground.png"),
													render->createImage("data/graphic/wall.png") );
	Sleep( 10 );
	//�o�b�g
	bat.Init("data/model/RDX/bat.RDX",render->createImage("data/graphic/bat.png"));	
	Sleep( 30 );
	//�X�J�C�h�[��
	sky.Init("data/model/RDX/dome.RDX",render->createImage("data/graphic/sky3.png") );
	Sleep( 200 );	
	load_cnt = 40;

	//���[��
	rule.Init( render->createImage("data/graphic/count.png"),
			   render->createImage("data/graphic/result.png"),
			   render->createImage("data/graphic/score.png"),
			   render->createImage("data/graphic/cntside.png"),
			   render->createImage("data/graphic/change.png")
			   );
	Sleep( 10 );
	//�}�l�[�W���[�̊Ǘ����Ă���ꊇ�ŃZ�b�g����
	//�{�[���̏�����
	ball.Init( "data/model/RDX/ball.RDX",render->createImage("data/graphic/ball.png") );
	Sleep( 100 );
	//�����i�[�̔E�I��̏�����
	playerM.setImage( render,"data/graphic/putup01.png","data/graphic/putup02.png","data/graphic/putup01.png","data/graphic/te.png","data/graphic/minirun.png");
	Sleep( 100 );
	playerM.setModel( "data/model/RDX/putup.RDX","data/model/RDX/pitch.RDX" );
	Sleep( 100 );
	load_cnt = 70;

	//�J�S������
	basketM.setImage( render,"data/graphic/basket.png" );
	Sleep( 200 );
	basketM.setModel( "data/model/RDX/basket.RDX" );
	Sleep( 100 );
	//�x�[�X�E�ϋq�Ȃ̏�����
	stadiumM.setImage(  render,"data/graphic/base.png","data/graphic/auditorium.png");
	Sleep( 10 );
	stadiumM.setModel( "data/model/RDX/base.RDX","data/model/RDX/auditorium.RDX" );
	Sleep( 10 );
	load_cnt = 90;
	//�G�f�B�b�g�����̏�����
	edit.Init( "data/model/RDX/range.RDX",
				render->createImage("data/graphic/range.png"),
				render->createImage("data/graphic/wait.png"),
				render->createImage("data/graphic/img_mouse.png")
				);	
	load_cnt = 200;
	
	play_bgm = LoadSoundMem( "data/bgm/play.mp3" );
	play_bgm2 = LoadSoundMem( "data/bgm/baseball_stadium2.mp3" );
	play_bgm_f = false;
	
	//�X���b�h�쐬�A�J�n
	thre.Create( this );
	thre.Start();
}

/** ���C������ */
void BaseBall::Main()
{
	if( play_bgm_f ==false )
	{
		PlaySoundMem( play_bgm , DX_PLAYTYPE_LOOP );
		PlaySoundMem( play_bgm2 , DX_PLAYTYPE_LOOP );
		ChangeVolumeSoundMem( 240 , play_bgm2 ) ;
		play_bgm_f = true;
	}

	if( draw_cnt < 200 )
	{
		if( load_cnt>draw_cnt )draw_cnt++;		
		CalAlpha();
	}else{
		if( scene_flag )
		{
			StopSoundMem(play_bgm);
			StopSoundMem(play_bgm2);
			scene->setScene( new_s scen::Winner(render,key,mouse,&rule,scene_flag ), new_s FadeOut(render,Color(0xFFFFFF),0.05f),new_s FadeIn(render,Color(0xFFFFFF),0.05f) );
		}
		//�Q�[���Z�b�g�łȂ�
		else
		{


			//�J�����̎��_
			angle.Main( camera, ball,rule,playerM,*key,edit_mode,p1_state );

			const float  ss = 3;
			sky.Main();						//�����]������
			

			////////////////////////////////////
			//�}�E�X���W�ϊ�
			// 2D��3D
			mouse->TransformPosition3D( camera.getMatrix(),camera.getPosition() );
			
			//���[�h�ؑ�
			if( p1_state==Rule::DEFFENCE )						//�v���C���[1������̎�
			{
				//�R���g���[���L�[�B���[�h�ؑցB�{�[�����o�����ĂȂ����̂݉\
				if( key->isOn( Key::LCONTROL ) && ball.getStateFlag()==0 ) edit_mode^=1;
			}else{												//�v���C���[2������̎�
				if( key->isOn( Key::RCONTROL ) && ball.getStateFlag()==0 ) edit_mode^=1;
			}
			//����̃J�S���Ǘ����Ă���}�l�[�W���[�̃��C������
			basketM.Main( key,&rule,&ground,&ball );
			
			if( edit_mode==false )				//�G�f�B�b�g���[�h�łȂ�
			{
				edit.setMode( false );						//���[���̃��[�h���֘A�t��
				ground.Main( &ball,&bat,&rule );	//�O���E���h�̃��C���@�X�g���C�N�E�{�[���̔��菈��
				ball.Main( &ground,&rule,key,p1_state );			//�{�[���̃��C������
				bat.Main( key,&ball,&rule,p1_state);			//�o�b�g�N���X�̃��C������
				//�v���C���[�i�I��)�N���X���Ǘ����Ă���}�l�[�W���[�̃��C������
				playerM.Main( key,&ground,&ball,&basketM,&rule,p1_state );		
				//�X�^�W�A���i����j�N���X���Ǘ����Ă���}�l�[�W���[�̃��C������
				stadiumM.Main( &rule,&ground,&ball );		//�h����̃��C���E�O���E���h�N���X��n��	
				//���[���̃��C������
				rule.Main( &ball,&ground,basketM,playerM,bat,&p1_state,&p2_state,&scene_flag );
			}else
			{
				//����G�f�B�^�[�N���X
				edit.Main( *mouse,camera,basketM,edit_mode );
			}
		}
	}
}

/** �\�� */
void BaseBall::Draw()
{
	render->setViewport( Rect(0,0,640,480 ) );

	if( draw_cnt<200 )
	{
		int cnt = draw_cnt;
		if( cnt>100 ) cnt = 100;
		render->drawImage( Rect(0,0,640,480),load_bg,Rect(0,0,512,512) );	//���[�f�B���O���e�N�X�`��
		render->drawImage( Rect(80,130,80+512,130+64),load_f,Rect(0,0,512,64),load_al );	//���[�f�B���O���e�N�X�`��
		render->drawFill( Rect(300,450,300+cnt*3,470 ),0xAf0000FF );
		render->drawBox( Rect(299,449,301+cnt*3,471 ),0x4fFFFF00 );
		for(int i=0;i<3;i++)render->drawBox( Rect(298-i,448-i,602+i,472+i ),0xffF0F0FF );		
	}else{
		//�ꖇ�G����Ԍ��ɕ\���������̂�
		render->setZBufferMode( render->ZM_OFF );
		render->drawImage( Rect(0,0,640,480),bg,Rect(0,0,512,512) );	//���[�f�B���O���e�N�X�`��
		render->setZBufferMode( render->ZM_ON_WRITE );

		sky.Draw( render,camera.getMatrix() );					//�X�J�C�h�[��
		stadiumM.Draw( render,camera.getMatrix() );				//�X�^�W�A���i����j�}�l�[�W���[
		ground.Draw( render,camera.getMatrix() );				//�O���E���h���f��

		basketM.Draw( render,camera.getMatrix() );				//�J�S�i����j�}�l�[�W���[
		ball.Draw( render,camera.getMatrix() );				//�{�[��
		playerM.Draw( render,camera,bat );				//�v���C���[�i�I��j�}�l�[�W���[
		rule.Draw( render,camera.getMatrix(),bat );				//���[��	���̏ꏊ
		bat.Draw( render,camera.getMatrix() );				//�o�b�g���f��
		angle.Draw( render,playerM,ground,ball );				//�����i�[�p�J����
		edit.Draw( render,camera.getMatrix(),*mouse,basketM );	//�G�f�B�b�g�}�l�[�W���[

		DrawHelp( render,p1_state,&ball,&bat );
	}
}

//�w���v�̕\��
/*
IRender		*render		:�����_�����O�N���X
Rule::STATE p1state		:�v���C���[�̏��
Ball		*ball		:�{�[���N���X
Bat			*bat		:�o�b�g�N���X
*/
void BaseBall::DrawHelp(IRender *render,Rule::STATE p1state,Ball *ball,Bat *bat )
{
	//��Ƀw���v�L�[�\��
	if( ball->getStateFlag() == 2 || ball->getStateFlag()== 3 ) return;	//�ł��Ԃ����Ƃ���q�b�g�̃J�������o�̎��͏����ɓ���Ȃ�
	if( edit_mode ) return;			//�G�f�B�b�g���[�h���͓���Ȃ�
	
	const int s = 32;	//�摜�T�C�Y
	//�������ĂȂ���
	if( ball->getStateFlag() ==0 )
	{
	if( p1state == Rule::STATE::ATTACK )	//�v���C���[�PP�U����
		{
			render->drawImage( 90,25,128,64,img_attacker,0,0 );

			render->drawImage( 235,150,32,32,img_Helpkey,0,64 );			//0
			render->drawImage( 235+s,150,32,32,img_Helpkey,32,64 );			//-
			render->drawImage( 235+s*2,150,32,32,img_Helpkey,64,64 );		//^
			render->drawImage( 235+s*3,150,32,32,img_Helpkey,96,64 );		//\//
			render->drawImage( 235+s*4,150,32,32,img_Helpkey,128,64 );		//BS
			
			render->drawImage( 235+s*2,155+s,32,32,img_Helpkey,160,64 );	//@
			
			render->drawImage( 235+s,155+s*2,32,32,img_Helpkey,191,64 );	//;
			render->drawImage( 235+s*3,155+s*2,32,32,img_Helpkey,256,64 );	//]
			
			render->drawImage( 235+s*2,110,32,32,img_Helpkey,418,64 );	//Ctrl

			//�o�b�^�[��
			if( bat->getBatter() == Bat::BATTER_TYPE::LEFT )
			{
				render->drawImage( 60+s,410,32,32,img_Helpkey,352,32 );		// X
				render->drawImage( 60+s*2,410,32,32,img_Helpkey,352+s,32 );		// C
			}else{
				render->drawImage( 460,410,32,32,img_Helpkey,320,32 );			//Z
				render->drawImage( 460+s,410,32,32,img_Helpkey,352+s,32 );		// C
			}


		}else if( p1state == Rule::STATE::DEFFENCE )									//�v���C���[�PP���
		{
			render->drawImage( 390,25,128,64,img_attacker,0,64 );

			render->drawImage( 235,150,32,32,img_Helpkey,0,32 );			//1
			render->drawImage( 235+s,150,32,32,img_Helpkey,32,32 );			//2
			render->drawImage( 235+s*2,150,32,32,img_Helpkey,64,32 );		//3
			render->drawImage( 235+s*3,150,32,32,img_Helpkey,96,32 );		//4
			render->drawImage( 235+s*4,150,32,32,img_Helpkey,128,32 );		//5
			
			render->drawImage( 235+s*2,155+s,32,32,img_Helpkey,160,32 );	//W
			
			render->drawImage( 235+s,155+s*2,32,32,img_Helpkey,191,32 );	//A
			render->drawImage( 235+s*3,155+s*2,32,32,img_Helpkey,256,32 );	//D
			
			
			render->drawImage( 235+s*2,110,32,32,img_Helpkey,418,32 );	//Ctrl


			//�o�b�^�[��
			if( bat->getBatter() == Bat::BATTER_TYPE::LEFT )
			{
				render->drawImage( 60+s,410,32,32,img_Helpkey,286+s,64 );		// /
				render->drawImage( 60+s*2,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}else{
				render->drawImage( 460,410,32,32,img_Helpkey,286,64 );			//.
				render->drawImage( 460+s,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}

		}
	}else if( ball->getStateFlag() == 1 )		//�������@�ω����Ƃ��̃L�[�\��
	{
		if( p1state == Rule::STATE::ATTACK )	//�v���C���[�U����
		{
			render->drawImage( 90,25,128,64,img_attacker,0,0 );

			render->drawImage( 235+s,155+s*2,32,32,img_Helpkey,191,64 );	//;
			render->drawImage( 235+s*2,155+s*2,32,32,img_Helpkey,191+s,64 );//:
			render->drawImage( 235+s*3,155+s*2,32,32,img_Helpkey,256,64 );	//]

		}else if( p1state == Rule::STATE::DEFFENCE ){
			render->drawImage( 390,25,128,64,img_attacker,0,64 );


			render->drawImage( 235+s,155+s*2,32,32,img_Helpkey,191,32 );	//A
			render->drawImage( 235+s*2,155+s*2,32,32,img_Helpkey,191+s,32 );//S
			render->drawImage( 235+s*3,155+s*2,32,32,img_Helpkey,256,32 );	//D
			
			//�o�b�^�[��
			if( bat->getBatter() == Bat::BATTER_TYPE::LEFT )
			{
				render->drawImage( 60+s,410,32,32,img_Helpkey,286+s,64 );		// /
				render->drawImage( 60+s*2,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}else{
				render->drawImage( 460,410,32,32,img_Helpkey,286,64 );			//.
				render->drawImage( 460+s,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}
		}
	}

			
}



//�Q�[�����̉��
void BaseBall::Dispose()
{
	bg = NULL;
	load_bg =NULL;
	load_f =NULL;
}