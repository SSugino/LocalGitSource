/** @file
 * エフェクトクラスの実装
 * @date 2009/06/19
 * @author 杉野
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
	rot += 5 *Math::PI/180;	//回転していきながら
//	scale *=0.99f;			//縮小していきながら

	//anim1.setAnimation()	//アニメーション処理を入れるなら
	cnt-=5;
	if(cnt<=0) return false;	//死亡
	return true;				//生存

}

//第三引数はカメラ行列とか
void EffectBase::Draw( IRender *render,Model2D &model,Matrix3x3 &m )
{
	Matrix3x3 p,r,scal,m1;			
	r.Rotate( rot );					//回転
	p.Translate( pos );					//位置の行列
	scal.Scale( scale );				//サイズ

	m1 = scal * r * p;					//行列掛け合わせ
	
	model[0].Transform( m1,m );
	//model[0].setColor( (cnt<<24) | 0xffFFFFFF );
	model[0].Draw( render );
}

void EffectManager::Anim()
{
	std::list< EffectBase* >::iterator	i;
	i = effect.begin();

	while( i != effect.end() )	//終わりでなければ
	{
		EffectBase *e=*i;		//リストで参照している中身をぶち込む
		bool ret = e->Anim();	//戻り値を受け取る
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
	while( i != effect.end() )	//終わりでなければ
	{
		EffectBase *e=*i;		//リストで参照している中身をぶち込む
		e->Draw( render,model,m );	//ここで呼ぶ
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
	ef = new_s EffectBase( p,count,r );//Base関数呼び出し
	effect.push_back(ef);
}


void EffectManager::Dispose()
{
	std::list< EffectBase* >::iterator i;
	i = effect.begin();
	while( i != effect.end() )	//終わりでなければ
	{
		delete( *i );
		i = effect.erase(i);
	};
}