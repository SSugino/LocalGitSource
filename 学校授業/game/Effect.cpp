/** @file
 * �G�t�F�N�g�N���X�̎���
 * @date 2009/06/19
 * @author ����
 **/

#include "Effect.h"
#include "XML.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;

bool EffectBase::Anim()
{
	rot += 5 *Math::PI/180;	//��]���Ă����Ȃ���
//	scale *=0.99f;			//�k�����Ă����Ȃ���

	//anim1.setAnimation()	//�A�j���[�V��������������Ȃ�
	cnt-=5;
	if(cnt<=0) return false;	//���S
	return true;				//����

}

//��O�����̓J�����s��Ƃ�
void EffectBase::Draw( IRender *render,Model2D &model,Matrix3x3 &m )
{
	Matrix3x3 p,r,scal,m1;			
	r.Rotate( rot );					//��]
	p.Translate( pos );					//�ʒu�̍s��
	scal.Scale( scale );				//�T�C�Y

	m1 = scal * r * p;					//�s��|�����킹
	
	model[0].Transform( m1,m );
	//model[0].setColor( (cnt<<24) | 0xffFFFFFF );
	model[0].Draw( render );
}

void EffectManager::Anim()
{
	std::list< EffectBase* >::iterator	i;
	i = effect.begin();

	while( i != effect.end() )	//�I���łȂ����
	{
		EffectBase *e=*i;		//���X�g�ŎQ�Ƃ��Ă��钆�g���Ԃ�����
		bool ret = e->Anim();	//�߂�l���󂯎��
		if( ret ) ++i;
		else
		{
				delete( *i );
				i=effect.erase( i );
		}
	};
}

void EffectManager::Draw( IRender *render,Matrix3x3 &m )
{
	std::list< EffectBase* >::iterator	i;
	i = effect.begin();
	render->setTexture( tex );
	while( i != effect.end() )	//�I���łȂ����
	{
		EffectBase *e=*i;		//���X�g�ŎQ�Ƃ��Ă��钆�g���Ԃ�����
		e->Draw( render,model,m );	//�����ŌĂ�
		i++;
	};
}

void EffectManager::Load( char *model_name,const char *tex_name,graphics::IRender *render )
{
	model.Load( model_name );
	tex = render->createImage( tex_name );
}

void EffectManager::Init( math::Vector2 &p,int count,float r )
{
	EffectBase *ef;
	ef = new_s EffectBase( p,count,r );//Base�֐��Ăяo��
	effect.push_back(ef);
}


void EffectManager::Dispose()
{
	std::list< EffectBase* >::iterator i;
	i = effect.begin();
	while( i != effect.end() )	//�I���łȂ����
	{
		delete( *i );
		i = effect.erase(i);
	};
}