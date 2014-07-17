/** @file
 * ボールクラスの実装
 * @date 2009/09/14
 * @author 杉野
 **/

#include "Ball.h"
#include "../game/BoundingObject.h"

using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;
using namespace input;

//ボールの移動加算
/*
const Vector	&dir		:移動させる軸
const float		_velosity	:移動量
*/
void Ball::addMove( const Vector &dir, const float _velosity )
{
	if( _velosity>0 && _velosity<0 )				//速度0以外、速度があるなら
	{
		//移動速度の飽和処理
		if( _velosity >= max_velosity ) velosity = max_velosity;//引数の速度が基底の最大速度を超えていた場合、規定速度を最大に
		if( _velosity <= min_velosity ) velosity = min_velosity;//引数の速度が基底の最低速度を超えていた場合、規定速度を最低に

		this->pos += (dir*_velosity);							//移動量加算
		sphere.p = pos;											//コリジョン球体座標を今の座標と関連付け
		sphere.s = size;										//コリジョン球体サイズを今のサイズと関連付け
	}else velosity = 0;
}

//ボールの回転加算
/*
float add_rot		:回転量
*/
void Ball::addRotate( float add_rot )
{
	if( add_rot>1.0f ) add_rot = 1;					//回転量１以上なら1に
	rotate += (add_rot/((size+size)*Math::PI));		//回転計算
}

//各関数を呼ぶメイン
/*
Ground			*g			:グラウンドクラス
Rule *rule, Key *key		:キークラス
Rule::STATE		&p1state	:プレイヤー１Pの状態参照
*/
void Ball::Main( Ground *g, Rule *rule, Key *key, Rule::STATE &p1state )
{
	g->IntersectFunction( rule,NULL,NULL,this );		//グラウンドの当たり判定
	Move( *key,p1state );								//ピッチャーの移動処理
}

//初期化
/*
char			*model_name:モデルのパス名称
IImage			*ball_img  :ボールのテクスチャ
*/
void Ball::Init( char* model_name,IImage *ball_img )
{
	sphere=BoundingSphere3D(pos,size);					//コリジョンの球にメンバの座標とサイズを初期化
	model.Load( model_name );							//3Dモデルのロード
	img	= ball_img;										//テクスチャを引数から初期化

	pos		= Vector(0.1444f,1,-40.48f);				//初期座標を実値初期化
	gravity = 0.098f;									//重力実値初期化
	mass	= 1.6f;										//質量実値初期化							
	size	= 1.5f;										//球のサイズ実値初期化
}

//ボールの移動
/*
Key				&key		:キークラス参照
Rule::STATE		&p1state	:プレイヤー1Pの状態参照
*/
void Ball::Move( Key &key,Rule::STATE &p1state )
{
	if( direct.Length() )this->direct.Normalize();		//長さがあるなら方向軸は常に単位ベクトルにする
	sphere.p = pos;										//判定座標を今の座標と関連付け
	sphere.s = size;				

	calMatrix();										//行列計算
	if( state_flag )									//状態に値があるときのみ処理
	{
		if( velosity>0 ) rotate_val = ( max_velosity/velosity );//回転量を最大スピード/今のスピードで求める	

		//変化球 D、→　A、←　キー 
		if( p1state==Rule::DEFFENCE )					//プレイヤー１が守備だった
		{
			if( key.isDown( Key::KEY_D ))	target.x +=0.35f;	//玉の座標をずらす
			if( key.isDown( Key::KEY_A ))	target.x -=0.35f;
		}
		else if( p1state == Rule::ATTACK )  //プレイヤー2が守備だった
		{
			if( key.isDown( Key::SEMICORON )) target.x -=0.35f;	//玉の座標をずらす
			if( key.isDown( Key::BRACE )) target.x +=0.35f;
		}

		//投球時は目標に向かって飛ばす
		if( this->state_flag==1 )
		{
			Vector3 direct = (this->target - this->pos).Normalize();	//目的地-ボール
			setDirectValue( direct );									//方向を決める
		}
		//打った後の打球に関して
		if( 1<=state_flag && state_flag<=3 )
		{
		//球の移動量によって座標の更新はここのみ
		direct.y = 0;												//反射した軸がY値も入るから	
		this->pos.y += -( gravity*mass );							//重力＊質量
		//移動
		this->addMove( direct,velosity );							//軸の方向に進ませる
		//球の回転
		this->addRotate( rotate_val*direct.z );
		}
	}else															//stateが０　つまり投げる前
	{
		pos = Vector(0.1444f,1,-40.48f );							//固定値に座標をセット
		rotate = 0;													//回転なし
		rotate_val = 0;												//回転量なし
		velosity = 0;												//移動量なし
	}
}

//行列計算
void Ball::calMatrix()
{
	mat_sc.Scale(size,size,size);
	mat_t.Translate( pos );
	mat_r.RotateX( rotate );
	mat_ball = mat_r*mat_sc*mat_t;					//行列計算
}

//終着点に付いた処理
bool Ball::BackNetCollision()
{	
	if( state_flag!=1 )return false;							//投球時のみの判定
	//バックネットに到達
	if( Collision3D::Collision( sphere,bbox1 ) )				//「キャッチャーに届いたら」がコリジョン判定の処理
	{
		return true;
	}
	return false;
}

//描画
/*
IRender *render					:Direct9レンダリング
sugino::math::Matrix4x4 &vps	:計算された行列(view.pos.scale)
*/
void Ball::Draw( IRender *render, sugino::math::Matrix4x4 &vps )
{

	if( state_flag==0 )return;						//ボール出現中
	
	//影
	Matrix shadow,mm;
	shadow.Shadow( 1,1,sphere.s+0.5f );				//影を出す
	mm.Translate( pos.x,pos.y-0.5f,pos.z );
	//影専用の表示
	render->setTexture( NULL );						//テクスチャをはらない
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform(mm*shadow, vps);			//影とオブジェクトの掛け合わせ
		model[i].setTColor( 0x45000000 );			//影の色
		model[i].Draw( render );					//影の表示
	}
	
	//ボールの表示
	render->setTexture( img );
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform(mat_ball, vps);			//取得して変換
		if( !makyu_flag )model[i].Draw( render );	//魔球状態でなければ表示
		render->setTexture( NULL );					//
	}
}