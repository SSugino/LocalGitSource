/** @file
 * �싅�Ճ��[���N���X�̎���
 * @date 2009/10/15
 * @author ����
 **/
#include "CameraAngle.h"
#include "../baseball/ball.h"

#include "../game/BoundingObject.h"	

using namespace sugino;
using namespace baseball;
using namespace system;
using namespace	input;
using namespace graphics;
using namespace game;
using namespace math;

//������
void CamAngle::Init()
{
	//�`��͈͂�������
	right = Rect( 450,10,630,130 );						//��ʍ쐬
	left =	Rect( 10,10,190,130 );


	//�����i�[������J�����̏�����
	cam1st.setPosition( Vector(0,8,-45) );				//1�ۑ��J�����̈ʒu
	cam1st.setAt( Vector(39,3,-40) );					//1�ۑ�����ʒu
	cam1st.setPercective( 1.3333f*13,1*15,10,100000 );	//1�ۑ��p�[�X

	cam3rd.setPosition( Vector(0,8,-40) );				//3�ۑ��J�����ʒu
	cam3rd.setAt( Vector(-44,3,-48) );					//3�ۑ�����ʒu
	cam3rd.setPercective( 1.3333f*13,1*15,10,100000 );	//3�ۃp�[�X

	//�X�N���[���s�����蒼��
	cam1st.setScreen( right );
	cam3rd.setScreen( left );

	wait = 80;								//�J������~�J�E���g
}

//�J�����̃��C��
/*
Camera3D		&cam	:�J�����N���X�Q��
Ball			&_ball	:�{�[���N���X�Q��
Rule			&_rule	:���[���N���X�Q��
PlayerManager	&pm		:�v���C���[�}�l�[�W���[�N���X�Q��
Key				&key	:�L�[�N���X�Q��
bool			mode	:�G�f�B�b�g���[�h���ǂ����̎Q��
Rule::STATE		&p1		:���[���̏��
*/
void CamAngle::Main( Camera3D &cam, Ball &_ball, Rule &_rule, PlayerManager &pm,Key &key,bool mode,Rule::STATE &p1 )
{
	Aspect( cam,_ball,_rule,key,mode,p1 );
}

//���C���J�����̈ړ���A���O��(�����R�`�������C���j
/*
Camera3D	&cam	:�J�����N���X�Q��
Ball		&_ball	:�{�[���N���X�Q��
Rule		&_rule	:���[���N���X�Q��
Key			&key	:�L�[�N���X�Q��
bool		mode	:�G�f�B�b�g���[�h���ǂ����̎Q��p1
Rule::STATE &p1		:���[���̏��
*/			 
void CamAngle::Aspect( Camera3D &cam, Ball &_ball, Rule &_rule, Key &key,bool mode,Rule::STATE &p1 )
{
	//�����҂��̎� ���͓�����
	if( mode==false )
	{
		if( _ball.getStateFlag()==0 || _ball.getStateFlag()==1 )				//�{�[�����������Ă��Ȃ��A�܂��͓��������
		{
			cam.setPercective( 1.3333f*6.2699f,1*6.2699f,6.2699f*1,10000 );�@	//�J�����̃p�[�X���Z�b�g
			cam.setAt( Vector(0,0.5f,180)  );									//����ӏ����Z�b�g
			cam.setPosition( Vector(0,9.600f,-180) );							//�J�����̍��W���Z�b�g
			wait = 80;															//�҂����ԃZ�b�g
		}
		//�ł����Ƃ�
		else if( _ball.getStateFlag()== 2 )										//�{�[�����o�b�g�őł��Ԃ����Ƃ�
		{
			Vector p = _ball.getPos();											//���̍��W���擾
			Vector d = _ball.getDirectValue();									//�������擾
			if( isRight(p) )													//�Z���^�[���E�����Ȃ�
			{
				cam.setPercective( 1.3333f*5,1*5,5*1,10000 );					//���̎��̃J�����̃p�[�X���Z�b�g
				cam.setAt( p );													//�J�����̌����̓{�[���ɃZ�b�g
				cam.setPosition( Vector( -65,180,-160 ) );						//�����ォ�猩��H
			}else{																//�Z���^�[��荶�����Ȃ�
				cam.setPercective( 1.3333f*5,1*5,5*1,10000 );	
				cam.setAt( p );
				cam.setPosition( Vector( 65,180,-160 ) );						//�ォ�猩��H
			}			
		}
		else if ( _ball.getStateFlag()==4 )										//�{�[�����ʏ��
		{
			//�����ˈȊO�̃L�[�������ꂽ��J�������o�X�L�b�v
			if( key.isKeyData() && !key.isDown(Key::KEY_W) && !key.isDown(Key::AT_MARK) )
			{
				_ball.setStateFlag(0);											//�{�[���̏�Ԃ�0�Ɂi�Ȃ����j
			}
			//res = 0:�\�����Ȃ� 1:�q�b�g 2:�A�E�g 3:�t�H�A�{�[�� 4:�t�@�[�� 5:�z�[������
			int res = _rule.getResult();
			switch( res )
			{
			case( 1 ) :isHit(cam,_ball,_rule);break;			//�q�b�g���̃J�������o
			case( 2 ) :isOut(cam,_ball,_rule);break;			//�A�E�g���̃J�������o
			case( 4 ) :isFoul(cam,_ball,_rule);break;			//�t�@�[�����̃J�������o
			case( 5 ) :isHomeRun(cam,_ball,_rule);break;		//�z�[���������̃J�������o
			};
		}
	}else if( mode==true )								//����G�f�B�b�g��
	{
		//�J�����ړ��̍ő�
		const float y_max = 3;
		const float y_min = -2;
		const float x_max = -80;
		const float x_min =  70;
		
		Vector at = Vector(0,0,-40);						//�s�b�`���[�̃}�E���h(�o�ĂȂ����̃{�[���ʒu)�������Ƃ���
		const float chei =400;							//�J�����̍���
		const float sp = 1.5f;							//�J�����ړ��X�s�[�h
		//���Ă����̈ړ�
		if( p1==Rule::DEFFENCE )						//�v���C���[�P������Ȃ�
		{
			if( key.isDown( Key::KEY_S )){
				if( edit_cpos.y <= y_min ) edit_cpos.y = y_min;
				else edit_cpos -= cam.getUp_axis()*sp;				//�J�����́��ړ�
			}else if( key.isDown( Key::KEY_W )){
				if( edit_cpos.y >= y_max ) edit_cpos.y = y_max;
				else edit_cpos += cam.getUp_axis()*sp;				//�J�����́��ړ�
			}
			if( key.isDown( Key::KEY_D )){
				if( edit_cpos.x <= x_max ) edit_cpos.x = x_max;
				else edit_cpos += cam.getRight_axis()*sp;			//�J�����́��ړ�
			}
			else if( key.isDown( Key::KEY_A )){
				if( edit_cpos.x >= x_min ) edit_cpos.x = x_min;
				else edit_cpos -= cam.getRight_axis()*sp;			//�J�����́��ړ�
			}

		}else{
			if( key.isDown( Key::CONMA )){
				if( edit_cpos.y <= y_min ) edit_cpos.y = y_min;
				else edit_cpos -= cam.getUp_axis()*sp;				//�J�����́��ړ�
			}
			else if( key.isDown( Key::AT_MARK )){
				if( edit_cpos.y >= y_max ) edit_cpos.y = y_max;
				else edit_cpos += cam.getUp_axis()*sp;				//�J�����́��ړ�
			}
			if( key.isDown( Key::BRACE )){
				if( edit_cpos.x <= x_max ) edit_cpos.x = x_max;
				else edit_cpos += cam.getRight_axis()*sp;			//�J�����́��ړ�
			}
			else if( key.isDown( Key::SEMICORON )){
				if( edit_cpos.x >= x_min ) edit_cpos.x = x_min;
				else edit_cpos -= cam.getRight_axis()*sp;			//�J�����́��ړ�
			}
		}
		
		cam.setPercective( 1.3333f*23,1*23,23*1,10000 );			//�p�[�X�ݒ肵�Ȃ���
		cam.setAt( at+edit_cpos );									//����ӏ����s�b�`���[�̂ق��֐ݒ肵�Ȃ���
		cam.setPosition( edit_cpos + Vector(0,chei,0)*3 );			//�J�����̍��W��ݒ肵�Ȃ���
	}	
}

//�����i�[�p�̉��z��ʕ`��
/*
*/
void CamAngle::Draw( IRender *render,PlayerManager &pm,Ground &g,Ball &_rule )
{
	PlayerBase *base[4]= {NULL};		//�ۏ���z��ŊǗ�
	int j=0;							//�z��v�f�p
	//�S�ۂ̏����擾
	std::list<PlayerBase*>::iterator i;
	i = pm.getPlayer().begin();
	while(i != pm.getPlayer().end() )
	{
		base [j] = *i;					//�z��Ŏ��̉�
		++i;
		++j;
	}
	
	int r = _rule.getStateFlag();
	if( r==0 || r==1 )
	{
		//��ۂ������Ȃ�
		if( base[0]->getState() )
		{
			render->setViewport( right );	
			render->Clear(0);				
			render->ClearZ();				
			g.Draw( render,cam1st.getMatrix() );
			//�V���ȕ`��͈͂ɕ`��
			base[0]->Draw( render,cam1st.getMatrix() );
		}
		render->setViewport( Rect(0,0,640,480) );
		
		//�O�ۂ������Ȃ�
		if( base[2]->getState() )
		{
			render->setViewport( left );
			render->Clear(0);						
			render->ClearZ();		
			
			g.Draw( render,cam3rd.getMatrix() );
			base[2]->Draw( render,cam3rd.getMatrix() );
		}
		render->setViewport( Rect(0,0,640,480) );
	}
}


//�t�@�[���̂Ƃ��̃J�����̉��o
/*
Camera3D	&cam	:�J�����N���X�Q��
Ball		&_ball	:�{�[���N���X�Q��
Rule		&_rule	:���[���N���X�Q��
*/
void CamAngle::isFoul( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;										//�J�������狅�ւ̃x�N�g��
	float len = v.Length();										//�J�����Ƌ��܂ł̋���
	v.Normalize();
	float t	  =	len/50;											//�����̓����i1�񕪂̋���)
	const float rot = 0.01f;									//��]��
	float hei = cpos.y - pos.y;

	if( isRight(pos) ) cpos.Rotate( cam.getUp_axis(),rot );		//�E���Ȃ獶���ɃJ��������
	else cpos.Rotate( cam.getUp_axis(),-rot );
	cpos-=(v*t);												//�J�������߂Â���
	if( hei>2.5f ) cpos.y-=0.3f;								//�J�����̍����������Ă���
	cam.setPosition( cpos );

	if( len<50.5f )
	{
		bool r  = _rule.ColResultAlpha( 0.08f );
		if( r ){
			if( wait )wait--;
			else _ball.setStateFlag(0);							//��������
		}
	}
}

//�q�b�g�̂Ƃ��i�V���O�����QBH���z�[�������ȊO�͊܂� )�̃J�������o
/*
Camera3D	&cam	:�J�����N���X�Q�ƌ^
Ball		&_ball	:�{�[���N���X�Q�ƌ^
Rule		&_rule	:���[���N���X�Q�ƌ^
*/
void CamAngle::isHit()
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;							//�J�������狅�ւ̃x�N�g��
	
	float len = v.Length();							//�J�����Ƌ��܂ł̋���
	v.Normalize();
	float t	  =	len/50;								//�����̓����i1�񕪂̋���)
	float hei = cpos.y - pos.y;						//���ƃJ�����̍��፷
	if( hei>6.0f ) cpos.y-=1.0f;					//���̍����ɂȂ�܂ŉ����Ă���
	if( len>80 )
	{
		cpos-=(v*t);								//�J�������߂Â���
		cam.setPosition( cpos );
	}else{
		bool r = _rule.ColResultAlpha( 0.08f );
		if( r )
		{
			if( wait )wait--;						//�E�F�C�g�J�E���g���炷
			else _ball.setStateFlag(0);				//��������	
		}
	}
}

//�z�[�������̂Ƃ��̃J�������o
/*
Camera3D	&cam	:�J�����N���X�Q�ƌ^
Ball		&_ball	:�{�[���N���X�Q�ƌ^
Rule		&_rule	:���[���N���X�Q�ƌ^
*/
void CamAngle::isHomeRun( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;							//�J�������狅�ւ̃x�N�g��	
	float len = v.Length();							//�J�����Ƌ��܂ł̋���
	v.Normalize();
	float t	  =	len/50;								//�����̓����i1�񕪂̋���)
	float hei = cpos.y - pos.y;						//���ƃJ�����̍��፷
	
	if( len>140 )
	{
		if( hei>6.0f ) cpos.y-=1.0f;				//���̍����ɂȂ�܂ŉ����Ă���
		at = pos;
		cpos-=(v*t);								//�J�������߂Â���
		cam.setPosition( cpos );
	}else{
		if( at.y <= 50 )
		{	at.y += 0.5f;
			cam.setAt(  at  );
		}else{
			bool r;
			r = _rule.DrawMove();
			if( r )	_ball.setStateFlag(0);					//��������	
		}
	}
}

//�A�E�g�̎��̃J�������o
/*
Camera3D	&cam	:�J�����N���X�Q�ƌ^
Ball		&_ball	:�{�[���N���X�Q�ƌ^
Rule		&_rule	:���[���N���X�Q�ƌ^
*/
void CamAngle::isOut( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;										//�J�������狅�ւ̃x�N�g��
	float len = v.Length();										//�J�����Ƌ��܂ł̋���
	v.Normalize();
	float t	  =	len/30;											//�����̓����i1�񕪂̋���)
	const float rot = 0.05f;									//��]��
	float hei = cpos.y - pos.y;

	if( hei>30.5f ) cpos.y-=0.3f;
	if( len>120.5f )
	{
		cpos-=(v*t);											//�J�������߂Â���
	}else{
		if( isRight(pos) )cpos.Rotate( Vector(0,1,0) ,rot/(Math::PI*2) );	//�E���Ȃ獶���ɃJ��������
		else cpos.Rotate( Vector(0,1,0) ,-rot/(Math::PI*2) );				//�����Ȃ�E���ɃJ��������
		bool r = _rule.ColResultAlpha( 0.08f );
		if( r )
		{
			if( wait )wait--;
			else  _ball.setStateFlag(0);							//��������	
		}
	}
	cam.setPosition( cpos );
}

//���̈ʒu�𔻒�
/*
Vector3 &pos	:�ʂ̍��W
*/
bool CamAngle::isRight( Vector3 &pos )
{
	Vector v;				//90�x�x�N�g��
	v = Vector3::Cross( Vector(0,1,0),Vector(0,0,1) ).Normalize();
	return Vector3::Dot( v,pos - Vector(0,0,0) ) > 0;		//�Z���^�[���E������
}

//�͈͓�����
/*
Vector3 &pos	:�{�[���̍��W
float �@length	:�͈́i�x���w��j
*/
bool CamAngle::Inside( Vector3 &pos,float length )
{
	Vector v;
	v = Vector3::Cross(Vector(0,1,0),v );
	float cos2 = v.Length();
	if( cos2 < (Math::cos(length*(Math::PI/180))) ) return false;
	return true;
}