/** @file
 * �X�^�W�A���N���X�̎���
 * @date 2009/08/28
 * @author ����
 **/

#include "Stadium.h"
#include "../game/Rebound.h"
#include "../game/BoundingObject.h"
#include <algorithm>

#include "../DxLib.h"

using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;


//�ۂ̍s��v�Z
void Base::calMatrix()
{
	mat_sc.Scale( scale );
	mat_r.RotateY( rot );
	mat_t.Translate( pos );
	mat_base = mat_r*mat_sc*mat_t;
}

//�ۂ̃��C������
void Base::Main()
{
	this->pos.y-=gravity;			//�d�͊|���Ă���
	calMatrix();
	
}

//�ۂ̕`��
/*
IRender		*render	:�����_�����O�N���X
Matrix4x4	&vps	:�J�����s��
*/
void Base::Draw( IRender *render, Matrix4x4 &vps )
{
	model->getObject(0)->Transform(mat_base, vps);		//�擾���ĕϊ�
	render->setTexture(img);
	model->getObject(0)->Draw( render );						//�\��
	render->setTexture( NULL );

}

//�ۂ̐ڐG����
/*
Ball *bbase	:�{�[���N���X
*/
void Base::Collision( Ball *bbase )
{
	//�h����̃{�[�����瓖���蔻��̏����擾
	BoundingSphere3D sphere = bbase->getBoundingObj();
	//�o�b�g�ɓ�����������
	Vector	vret;
	float speed = bbase->getVelosity();
	if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
	{
		bbase->setMass( 0 );						//���ʂ��l���Ȃ�
		Vector p;
		p = bbase->getPos();
		p += Vector( speed ,speed*1.8f,speed );	//�{�[���𕂂���
		bbase->setPos( p );
	};
	bbase->setMass( 1.6f );						
}

//�ϋq�Ȃ̍s��v�Z
void Auditorium::calMatrix()
{
	mat_sc.Scale(	scale );
	mat_r.RotateY( rot );
	mat_t.Translate( pos );
	mat_audi = mat_r*mat_sc*mat_t;
}

//�ϋq�Ȃ̃��C��
void Auditorium::Main()
{
	calMatrix();
}

//�ϋq�Ȃ̕`��
/*
IRender		*render	:�����_�����O
Matrix4x4	&vps	:�J�����s��
*/
void Auditorium::Draw( IRender *render, Matrix4x4 &vps )
{
	model->getObject(0)->Transform(mat_audi, vps);		//�擾���ĕϊ�
	render->setTexture(img);
	model->getObject(0)->Draw( render );						//�\��
	render->setTexture( NULL );
}

//���������܂Ƃ߂�}�l�[�W���[�̏�����
void StadiumManager::Init()
{
	//�x�[�X��"3��"�ǉ�
	this->setStadium( new_s Base( Vector(43,1,-45),	Vector(4,3,4),10,&mdl_base,img_base,0.098f  ) );		//1st
	this->setStadium( new_s Base( Vector(0,1,-10),	Vector(4,3,4),40,&mdl_base,img_base,0.098f	 ) );		//2nd
	this->setStadium( new_s Base( Vector(-46,1,-45),Vector(4,3,4),-10,&mdl_base,img_base,0.098f ) );		//3rd
	
	//�ϋq�Ȃ�ǉ�
	this->setStadium( new_s Auditorium( Vector(85,7,-85),Vector(20,30,20),-38.61f,&mdl_audi,img_audi) );		//1st��
	this->setStadium( new_s Auditorium( Vector(-85,7,-80),Vector(20,30,20),-36.79f,&mdl_audi,img_audi) );		//3rd��

	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//i��擪(begin)��
	while( i != stadium.end() ){					//�I���܂�
		StadiumBase *s = *i;
		s->Init();									
		++i;
	}	
}

//���������܂Ƃ߂�}�l�[�W���[�̃��C��
/*
Rule	*rule	:���[���N���X
Ground	*g		:�O���E���h�N���X
Ball	*_ball	:�{�[���N���X
*/
void StadiumManager::Main( Rule *rule,Ground *g,Ball *_ball )
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//i��擪(begin)��
	while( i != stadium.end() ){					//�I���܂�
		StadiumBase *s = *i;
		g->IntersectFunction( rule,s,NULL,NULL );
		s->Main();									//�h�����Main���Ă΂��
		s->Collision( _ball);						//�h����Ƀ{�[���Ƃ̓����蔻�菈����������
		++i;
	}
}

//���������܂Ƃ߂�}�l�[�W���[�̕`��
/*
IRender		*render	:�����_�����O
Matrix4x4	&vps	:�J�����s��
*/
void StadiumManager::Draw( IRender *render,Matrix4x4 &vps )
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//�擪(begin)��
	while(i != stadium.end()){						//�I���܂�
		StadiumBase *s = *i;
		s->Draw( render, vps );						//�h����̕`��
		++i;
	}
	render->setTexture(NULL);
}

//���������܂Ƃ߂�}�l�[�W���[�̉��
void StadiumManager::Dispose()
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();					//���X�g�܂킷
	while(i != stadium.end()){
		delete (*i);
		i = stadium.erase(i);
	}
}
//-----------------------------------------------------------------------------
//		 ���̑��X�^�W�A���i����j�ɕK�v�ȃN���X�̎���
//-----------------------------------------------------------------------------
//����̏�����
/*
char	*model_name	:���f���̃p�X
IImage	*ground_img	:�n�ʃe�N�X�`��
IImage	*wall_img	:�ǃe�N�X�`��
*/
void Ground::Init( char* model_name,IImage *ground_img,IImage *wall_img )
{
	model.Load( model_name );
	img_ground	= ground_img;				//�n�ʂ̃e�N�X�`��
	img_wall	= wall_img;					//�ǂ̃e�N�X�`��
	
	pos = math::Vector3(0,0,4);				//�}�b�v�̔z�u���W������
	//�O���E���h�g�傳����
	math::Matrix4x4 m1;
	m1.Translate( pos );
	mat_sc.Scale(30,30,40);

	Vector local[11];

	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform( mat_sc*m1,math::Matrix::unit_matrix );
		model[i].NormalNomalize();
		model[i].PreTransform();
	}

	for( int i=1;i<11;i++ )								//�{�[�����W�̎擾
	{
		local[i] = model.getBoneArray(i).local;
		TransformVector( i,local[i] );					
	}
	//�����������	
	ResultInit();

	//�X�g���C�N�]�[���̏�����
	bbox.p = Vector(-1.05f,3,-105);			//�x�[�X�̐�
	bbox.s = Vector(3.005f,1.5f,0.5f);			//�X�g���C�N�̔���T�C�Y

	one_hit = false;							//���̂ݏ���������t���O

	se_foul = LoadSoundMem( "data/se/foul.wav" );
}

//�t�F���X����̏�����(�o�b�N�X�N���[���Ȃ�)
void Ground::ResultInit()
{
	Vector sc( 30,30,40 );
	Matrix rr[11];
	
	rr[1].RotateY( math::Math::PI*20/180 );
	bobox1 = game::BoundingOBox3D( bobox1.p*sc ,Vector(2,2.5f,18.5f),rr[1] );	//2BH
	rr[2].RotateY( math::Math::PI*38/180 );
	bobox2 = game::BoundingOBox3D( bobox2.p*sc ,Vector(2,2.5f,19),rr[2] );	//1BH
	rr[3].RotateY( math::Math::PI*55/180 );
	bobox3 = game::BoundingOBox3D( bobox3.p*sc ,Vector(2,2.5f,11),rr[3] );	//3BH
	rr[4].RotateY( math::Math::PI*60/180 );
	bobox4 = game::BoundingOBox3D( bobox4.p*sc ,Vector(2,2.5f,6),rr[4] );	//OUT
	rr[5].RotateY( math::Math::PI*73/180 );
	bobox5 = game::BoundingOBox3D( bobox5.p*sc ,Vector(2,2.5f,17.5f),rr[5] );//HOME
	rr[6].RotateY( math::Math::PI*-75/180 );
	bobox6 = game::BoundingOBox3D( bobox6.p*sc ,Vector(2,2.5f,15.5f),rr[6] );//HOME
	rr[7].RotateY( math::Math::PI*-64/180 );
	bobox7 = game::BoundingOBox3D( bobox7.p*sc ,Vector(2,2.5f,5.2f),rr[7] ); //OUT
	rr[8].RotateY( math::Math::PI*-58/180 );
	bobox8 = game::BoundingOBox3D( bobox8.p*sc ,Vector(2,2.5f,8.5f),rr[8] ); //3BH
	rr[9].RotateY( math::Math::PI*-40/180 );
	bobox9 = game::BoundingOBox3D( bobox9.p*sc ,Vector(2,2.5f,17.5f),rr[9] );//1BH
	rr[10].RotateY( math::Math::PI*-28/180 );
	bobox10 = game::BoundingOBox3D( bobox10.p*sc ,Vector(2,2.5f,19.7f),rr[10] );//2BH
}

//�{�[�����[�J�����W���O���[�o�����W�ɕϊ�
/*
int		num		:�{�[���i���o�[
Vector3 local	:���[�J�����W
*/
void Ground::TransformVector( int num,Vector3 local )
{
	Vector &v1 = local;						//�{�[��1�̍��̍��W
	Vector vv;

	vv = math::Matrix4x4::unit_matrix.TransformVector( v1 );		//�O���[�o�����W��

	switch(num)
	{
	case 1:
		bobox1.p = vv;break;
	case 2:
		bobox2.p = vv;break;
	case 3:
		bobox3.p = vv;break;
	case 4:
		bobox4.p = vv;break;
	case 5:
		bobox5.p = vv;break;
	case 6:
		bobox6.p = vv;break;
	case 7:
		bobox7.p = vv;break;
	case 8:
		bobox8.p = vv;break;
	case 9:
		bobox9.p = vv;break;
	case 10:
		bobox10.p = vv;break;
	default:
		system::Exception("�P�`�P�O�ȊO������܂�");
	}
}

//�ڐG����������ʂɂ킯��
/*
Rule		*rule	:���[���N���X
StadiumBase *base1	:�X�^�W�A���N���X	
PlayerBase	*base2	:�v���C���[�N���X
Ball		*base3	:�{�[���N���X
*/
void Ground::IntersectFunction( Rule *rule,StadiumBase *base1,PlayerBase *base2,Ball *base3 )
{
	//StadiumBase�̂Ƃ��̔���i�x�[�X�j
	Vector f_ray(0,-1,0);					//�^���̌����x�N�g��
	Vector w_ray(-1,0,0);					//�^���̌����x�N�g��
	Vector r1,norm1;
	if( base1 )
	{
		if( Intersect( model[0],base1->getPos(),f_ray,0.5f,&mf,&r1,&norm1 ) )
		{
			Vector p;
			p = base1->getPos();
			p = r1 - f_ray*0.5f;					//�����߂�
			base1->setPos( p );
		};
		if( Intersect( model[1],base1->getPos(),w_ray,0.5f,&mf,&r1,&norm1 ) )		//�t�@�[���ǂɓ���������
		{
			Vector p;
			p = base1->getPos();
			p = r1 - w_ray*0.75f;					//�����߂�
			base1->setPos( p );
		};
	}
	//PlayerBase�̎��̔���(�����i�[��s�b�`���[�j
	Vector r2,norm2;
	if( base2 )
	{
		if( Intersect( model[0],base2->getPos(),f_ray,0.5f,&mf,&r2,&norm2 ) )
		{
			Vector p;
			p = base2->getPos();
			p = r2 - f_ray*0.5f;					//�����߂�
			base2->setPos( p );
		};
		if( Intersect( model[1],base2->getPos(),w_ray,0.5f,&mf,&r2,&norm2 ) )
		{
			Vector p;
			p = base2->getPos();
			p = r2 - w_ray*0.5f;					//�����߂�
			base2->setPos( p );
		};
	
	}
	
	//Ball�̎��̔���
	if( base3 )
	{
		const float size = base3->getBoundingObj().s;
		Vector r3,norm3;
		float speed = base3->getVelosity();				//�X�s�[�h������Ă���
		//�n�ʔ���
		if( Intersect( model[0],base3->getPos(),f_ray,size,&mf,&r3,&norm3 ) )
		{
			//�펞����
			Vector p;
			p = base3->getPos();
			p = r3 - f_ray*1.6f;						//�����߂�
			base3->setPos( p );
			//�ł��Ԃ������̂�
			if( base3->getStateFlag()>=2 )
			{
				if( speed )
				{
					speed -= ( 0.065f*(base3->getMass()*base3->getGravity()) );	//�����𖳂����Ă���					
					base3->setVelosity( speed );
				}
			}
		};
		//�T�C�h�ǔ���
		Vector d;		
		if( IntersectSideWall( model[1],base3->getPos(),0.25f,&base3->getPos(),&d ) )
		{
			speed*=0.5f;									//�ᔽ��
			base3->setVelosity( speed );
			base3->setDirectValue( -d );
		}
		
		Vector r,n;
		if( IntersectWall( model[2],base3->getPos(),size,&mf,&r,&n ) )
		{
			Vector p;
			p = r+n.Normalize()*size;
			base3->setPos( p );			
		}
		if( IntersectWall( model[4],base3->getPos(),size,&mf,&r,&n ) )
		{
			Vector p;
			p = r+n.Normalize()*size;
			base3->setPos( p );			
		}
		//���ʂ�������t�F���X�i�q�b�g�Ȃǂ̔���͍s���Ȃ���Collision�����ցj
		if( base3->getStateFlag()>=2 )							//�{�[�����ł��ꂽ���
		{
			Collision( base3 );									//�t�F���X�O�̌��ʂ������锻��
			//�t�@�[������
			if( base3->getVelosity() <= 0  && base3->getStateFlag()==2  )						//���x�O�̂Ƃ�
			{
				//�t�@�[���]�[���Ƃ̔���B�ʂ̗��łȂ����J�S���{�[��������ĂȂ��Ȃ�
				if( IntersectFoul( model[3],base3->getPos()) )
				{
					result_state	= 1;						//��ԃt�@�[����
					PlaySoundMem(se_foul, DX_PLAYTYPE_BACK );
					int st = rule->getStrike();
					if( st < 2 ) st++;							//�X�g���C�N�J�E���g�O�a����
					rule->setStrike(st);						//�X�g���C�N�J�E���g�X�V	
					rule->setResult(4);
					base3->setStateFlag( 4 );					//���̏o����Ԃ𕚂���
				};
			}
		}
	}
}

//�t�F���X�ɂ�錋�ʔ���
/*
Ball *bbase	:�{�[���N���X
*/
int Ground::Collision( Ball *bbase )
{
	BoundingSphere3D sphere = bbase->getBoundingObj();		//�������瓖���蔻��̏����擾
	int ret = -1;												//�߂�l �������͂킴�Ɗ֌W�Ȃ��l�����Ă���

	//�e�t�F���X�O��OBox���������
	if( Collision3D::Collision( sphere,bobox1 ))	ret =  2;		//���t�g2BH
	else if( Collision3D::Collision( sphere,bobox2 )) ret = 1;		//���t�g1BH
	else if( Collision3D::Collision( sphere,bobox3 )) ret = 3;		//���t�g3BH
	else if( Collision3D::Collision( sphere,bobox4 )) ret = 0;		//���t�gOUT
	else if( Collision3D::Collision( sphere,bobox5 )) ret = 4;		//���t�g�z�[������
	else if( Collision3D::Collision( sphere,bobox6 )) ret = 4;		//���C�g�z�[������
	else if( Collision3D::Collision( sphere,bobox7 )) ret = 0;		//���C�gOUT
	else if( Collision3D::Collision( sphere,bobox8 )) ret = 3;		//���C�g3BH
	else if( Collision3D::Collision( sphere,bobox9 )) ret = 1;		//���C�g1BH
	else if( Collision3D::Collision( sphere,bobox10 ))ret = 2;		//���C�g2BH
	
	return ret;
}

//����̃��C��
/*
Ball	*_ball	:�{�[���N���X
Bat		*_bat	:�o�b�g�N���X
Rule	*_rule	:���[���N���X
*/
void Ground::Main( Ball *_ball,Bat *_bat,Rule *_rule )
{
	if( _ball->getStateFlag()==0 || _rule->getChangeFlag()  )	//��サ���u�Ԃ�
	{
		one_hit = false;										//�{�[���̏�Ԃ�0�̂Ƃ�
		result_state = 0;										//�{�[���̌��ʏ�Ԃ�0��
	}
	else if( _ball->getStateFlag()==1 )							//�{�[�����������Ȃ�
	{
		if( one_hit ) return;
		//�X�g���C�N�]�[���ɓ���������
		if( StrikeJudge( _ball ) == true )		
		{
			int a = 0;

			//�X�g���C�N���Z�t���O
			_rule->setStrikeAddFlag( true );
			if(!one_hit)one_hit=true;						//�t���O�𗧂ē��ڂ̏��������Ȃ��悤�ɂ�����	
		}else{												//�]�[���O�i�{�[���̂Ƃ��j
			//�X�g���C�N�]�[��������W����o�b�N�l�b�g������W�܂Œ����𓾂�
			const float len = (bbox.p-_ball->getBackNetPos() ).Length2();
			const float cnt = ( 0 + (_bat->getLeanCnt()*2) )*len - (_bat->getLeanCnt()*2);	//�U���Ă������@�o�b�g��U���Ă���Ԃ̓J�E���g������
			//��U�菈��  �L���b�`���[�ɓ���//
			if( _ball->BackNetCollision()==1 )				//�I���_�܂ŕt��������	
			{
				if( cnt ){									//�J�E���g���c���Ă�����(�O�`�P�̊Ԃ����J�E���g�������̂Ńo�b�N�l�b�g����X�g���C�N����܂ł̋����Ԃ񑝉�)
					_rule->setBallAddFlag( false );			//�{�[���t���O�͕�����
					_rule->setStrikeAddFlag( true );		//�ς��ɃX�g���C�N�t���O�𗧂Ă�
				}else _rule->setBallAddFlag( true );
			}
		}
	}
}

//�X�g���C�N�]�[���̔���
/*
Ball *bbase	:�{�[���N���X
*/
bool Ground::StrikeJudge( Ball *bbase )
{
	//�h����̃{�[�����瓖���蔻��̏����擾
	BoundingSphere3D sphere = bbase->getBoundingObj();
	sphere.s*=0.5f;										//���̌��̃T�C�Y��菬�������Ĕ��������������
	
	//���łɂ������Ă����Ȃ�
	if( one_hit ) return false;
	//�X�g���C�N�]�[������
	Vector	vret;
	//�����������ĂȂ��t���O�������ĂȂ��Ƃ��ɓ�������
	if( Intersect3D::Intersect( sphere,this->bbox,&vret)  ) 
	{	
		one_hit = true;
		return true;
	}else  return false;
}

//����̕`��
/*
IRender		*render	:�����_�����O�N���X
Matrix4x4	&vps	:�J�����s��
*/
void Ground::Draw( IRender *render, Matrix4x4 &vps )
{	
	//�n�ʕ`��
	render->setTexture( img_ground );
	model[0].Transform( math::Matrix4x4::unit_matrix,vps );
	model[0].Draw( render );

	//�Ǖ`��
	render->setTexture( img_wall );
	model[1].Transform( math::Matrix4x4::unit_matrix,vps );
	model[1].Draw( render );
	
	//���ʂ̕Ǖ`��
	model[2].Transform( math::Matrix4x4::unit_matrix,vps );
	model[2].Draw( render );
	
	//���ʂ̕ǂ𕪂����
	model[4].Transform( math::Matrix4x4::unit_matrix,vps );
	model[4].Draw( render );
}

//����̐ڐG����
/*
Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
Vector3		&pos	:���̒��S���W
float		size	:����T�C�Y
ModelFace	**f		:���f���̖�
Vector3		*ret	:��_
Vector3		*norm	:�@�� 
*/
bool Ground::Intersect( game::Object3D &map,Vector3 &pos, Vector3 &ray, float size, ModelFace **f, Vector3 *ret, Vector3 *norm )
{
	Vector cros;				//��_�߂�l�p
	
	//�S�ʔ���
	int i;
	for( i=0;i<map.getFaceCount();i++ )
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex( mf[0] );		//��0
		Vertex3D  &v2 = map.getVertex( mf[1] );		//��1
		Vertex3D  &v3 = map.getVertex( mf[2] );		//��2
		Plane pl( v1.p,v2.p,v3.p );					//�ʒu�֌W
		if( pl.IntersectRay( pos,ray,&cros )==1 )	//���C�̕����ɖʂ�����Ȃ�
		{
			if( pl.Distance( pos ) < size && pl.Inside( v1.p,v2.p,v3.p,cros )){
				*f		=&mf;
				*ret	=cros;		//��_�ʒu
				*norm	=pl.n;		//�@���̈ʒu
				
				return true;
			}
		}
	}
	return false;
}

//���������̖ʔ��� �ǔ���
/*
Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
Vector3		&pos1	:���̒��S���W
float		size	:���肳���邽�߂͈̔̓T�C�Y
Vector3		*pos	:�����̕␳���W
Vector3		*dir	:�����̕␳��
*/
bool Ground::IntersectSideWall( game::Object3D &map,math::Vector3 &pos1,float size,math::Vector3 *pos,Vector3 *dir )
{
	Vector ret;				//��_�߂�l�p
	*pos = pos1;			//���̍��W���R�s�[���Ď����Ă���
	int hit=0;				//�ʂƂ̓���������

	//�S�ʔ����{
	int i;
	for(i=0;i<map.getFaceCount();i++)
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex( mf[0] );		//��0
		Vertex3D  &v2 = map.getVertex( mf[1] );		//��1
		Vertex3D  &v3 = map.getVertex( mf[2] );		//��2
		Plane pl( v1.p,v2.p,v3.p );					//�ʒu�֌W		
		Vector ray = -pl.n;							//�e�ʂ�Plane�@����ray�Ƃ��čl����(����)

		float side = pl.Side( *pos );
		if( side - size < 0 )
		if( pl.IntersectRay( *pos,ray,&ret )>0 )	//���C�����ɖʂ�����Ȃ�
		{
			if(	pl.Inside(v1.p,v2.p,v3.p,ret) )		//�_�Ɩʂ̈ʒu�𒲂ׁA�����|���S����
			{
				hit++;		//�␳
				*dir = ret;
				if( hit>=2 ) return true;	//2���ȏ㓯���ɂ͂�����Ȃ�
			}
		}
	}
	return hit!=0;
}

//�J�x����
/*
Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
Vector3		&pos	:���̒��S���W
float		size	:����T�C�Y
ModelFace	**f		:���f���̖�
Vector3		*r		:��_
Vector3		*n		:�@�� 
*/
bool Ground::IntersectWall( Object3D &map,Vector &pos,float size,ModelFace**f,Vector *r,Vector *n )
{
	Vector ret;
	int i;
	for(i=0;i<map.getFaceCount();++i)
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex(mf[0]);
		Vertex3D  &v2 = map.getVertex(mf[1]);
		Vertex3D  &v3 = map.getVertex(mf[2]);
		Plane pl( v1.p,v2.p,v3.p );
		Vector ray = -pl.n;

		if( pl.IntersectRay( pos,ray,&ret ) == 1 )
		{
			if( pl.Distance(pos)< size && pl.Inside( v1.p,v2.p,v3.p,ret ))
			{
				*f = &mf;
				*r = ret;
				*n = pl.n;
				return true;
			}
		}
	}
	return false;
}

//�t�@�[������
/*
Object3D	&map	:�}�b�v���f���̃I�u�W�F�N�g
Vector3		&pos	:���̒��S���W
*/
bool Ground::IntersectFoul( Object3D &map, Vector3 &pos )
{

	ModelFace &mf = map.getFace(0);				//��0�Ԗ�
	Vertex3D  &v1 = map.getVertex( mf[0] );		//���_
	Vertex3D  &v2 = map.getVertex( mf[1] );		
	Vertex3D  &v3 = map.getVertex( mf[2] );		
	Plane p1( v1.p,v2.p,v3.p );
	
	ModelFace  &mf2  = map.getFace(3);
	Vertex3D  &vv1 = map.getVertex( mf2[0] );
	Vertex3D  &vv2 = map.getVertex( mf2[1] );
	Vertex3D  &vv3 = map.getVertex( mf2[2] );
	Plane p2( vv1.p,vv2.p,vv3.p );
	
	//�t�@�[���]�[��
	if( p1.Side(pos) < 0 || p2.Side(pos)<0 )
	{
		return true;
	}
	
	return false;
}

//�t�F���X����̎擾
/*
int num	:�I�u�W�F�N�g�ԍ�
*/
game::BoundingOBox3D Ground::getBoundingObj( int num )
{
	game::BoundingOBox3D	ret;		//�߂�l�p
	switch( num )
	{
		case 1:
			ret  = bobox1;break;
		case 2:
			ret  = bobox2;break;
		case 3:
			ret  = bobox3;break;
		case 4:
			ret  = bobox4;break;
		case 5:
			ret  = bobox5;break;
		case 6:
			ret  = bobox6;break;
		case 7:
			ret  = bobox7;break;
		case 8:
			ret  = bobox8;break;
		case 9:
			ret  = bobox9;break;
		case 10:
			ret  = bobox10;break;
		default: system::Exception("1�`10��I�����Ă�������");
	};
	return ret;
}



//-------------------------------------------------------------------------
//��̏�����
/*
char	*model_name	:���f���̃p�X
IImage	*sky_img	:��̃e�N�X�`��
*/
void SkyDome::Init( char* model_name,IImage *sky_img )
{
	model.Load( model_name );
	img_sky	= sky_img;						//��̃e�N�X�`��

	pos = math::Vector3(0,-10,0);			//�}�b�v�̔z�u���W������
	rot = 0;								//��]�x��
	rot_val	= 0.0003f;						//������]�X�s�[�h
	//����g�傳����
	math::Matrix4x4 m1;
	m1.Translate( pos );
	const float ss = 130;					//�X�P�[�����O�T�C�Y
	mat_sc.Scale(ss,ss,ss);
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform( mat_sc*m1,math::Matrix::unit_matrix );
		model[i].NormalNomalize();
		model[i].PreTransform();
	}	
}

//��̃��C��
void SkyDome::Main()
{
	rot-=rot_val;				//�x����^����	
	mat_sc.RotateY( rot );		//��]������
}

//��i�X�J�C�h�[���j�̕`��
/*
IRender		*render	:�����_�����O
Matrix4x4	&vps	:�J�����s��
*/
void SkyDome::Draw( IRender *render, Matrix4x4 &vps )
{			
	for(int i=0;i<model.getObjectCount();i++)		//���f�������`��
	{
		//��`��
		render->setTexture( img_sky );
		model[0].Transform( mat_sc,vps );
		model[0].Draw( render );		
	}
	render->setTexture( NULL );	
}

