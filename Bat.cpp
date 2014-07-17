/** @file
 * バットクラスの実装
 * @date 2009/08/30
 * @author 杉野
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

//初期化
/*
char	*model_name	:モデルのパス
IImage	*_img		:テクスチャ
*/
void Bat::Init( char* model_name,IImage *_img )
{
	model.Load( model_name );			//モデルのロード
	img = _img;							//テクスチャをメンバにセット

	Vector rr;
	rot = 86.3f;						//初期回転値
	rr.Rotate( Vector(0,1,0),rot );		//その回転値で初期化

	pos = math::Vector3(-10,0,-103);			//マップへの配置座標初期化
	mat_sc.Scale( Vector(5.5f,5.5f,5.5f) );		//各5倍
	mat_t.Translate( pos );
	mat_r.RotateY( rot );
	mat_init.Initialize( rr,Vector( 0,1,0 ),pos );	//行列初期化
	mat_bat = mat_init;

	//判定サイズの初期化
	col_pos = model.getBoneArray(1).local;		//ボーン1番目の座標（バットの先)を保存
	line.s = this->pos;							//バットの原点をスタート位置とする
	line.width = 1.1f;							//コリジョンの長さを少し伸ばす。（ボール球にも一応当てるため
	
	//打席の初期化
	type = LEFT;								//打席を初期は右打者でセット

	//傾きの初期化
	lean_rot = 3.8f;							//傾く限界角度
	lean_speed = 0.08f/4;						//傾く早さ
	lean_flag = 0;
	lean_cnt  = 0;								//傾くカウント
	pow =0;										//打ち返す力
	rot_val = 0;

	se_swing = LoadSoundMem("data/se/swing.mp3");		//スイング音
	se_hitting = LoadSoundMem("data/se/hitting.wav");	//打ったときの音
	se_swing_f = false;							//スイング状態
}

//行列計算
void Bat::calMatrix()
{		
	rot_val = rot + (lean_rot*lean_cnt);			//スイングの回転量取得	

	mat_r.RotateY( rot_val );						//Y軸回転
	mat_sc.Scale( Vector(11.5f,10,11.5f) );			//モデルのサイズがどんなボールも当たってしまうように見えるので縮小
	mat_t.Translate( pos );
	mat_bat = mat_sc*mat_r*mat_t;					//行列更新
}

//バットクラス各関数を呼ぶメイン
void Bat::Main( Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state )
{	
	calMatrix();													
	if( _rule->ColAlpha() )											//交代画面を出してない時のみ
	{
		for( int i=0;i<4;i++ )										//ループでまわして速度を上げる
		{
			calMatrix();											
			//Cキーまたは.キーバット回転
			Swing( key,_ball,_rule,p1state );						//スイング関数
			col_pos2 = mat_bat.TransformVector( col_pos );			//グローバル座標に
			line.e = col_pos2;										//バット先の座標を判定として関連付け
			line.s = pos;
			Collision( _ball );										//バットとボールの判定
		}

		//バッター打席変更
		//Zキー又は.で右バッターに
		if( p1state==Rule::ATTACK )									//プレイヤー1Pが攻撃の時
		{
			if( key->isOn( Key::KEY_X ) )	setBatter( RIGHT );		//右打席に
			else if( key->isOn( Key::KEY_Z ) )setBatter( LEFT );	//左打席に
		}else{														//プレイヤー2Pが攻撃の時
			if( key->isOn( Key::SLASH ) ) setBatter( RIGHT );
			else if( key->isOn( Key::DOT ) )setBatter( LEFT );
		}
	}
}

//スウィング
/*
Key *key,Ball	*_ball		:ボールクラス
Rule			*_rule		:ルールのクラス
Rule::STATE		&p1state	:１Pの状態を参照
*/
void Bat::Swing(  Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state )
{
	if( ( key->isDown( Key::KEY_C)&&p1state==Rule::ATTACK)		//Cキー押してなおかつプレイヤー１が攻撃の時
		||  ( key->isDown( Key::R_SHIFT )&&p1state==Rule::DEFFENCE) )	// . キー押してなおかつプレイヤー１が守備の時
	{
		pow = 0.5f;													//ボタン押したらパワー値0.5を初期値とする
		float cnt;
		cnt	 = lean_cnt;											//今現在のカウントを貰う
		cnt += lean_speed;											//カウントとスピードを加算していく
		if( cnt <= 1.0f ){											//計算結果カウントが1未満
			 lean_cnt =  cnt;										//バットが画面手前へ回転していく
		}else lean_flag= 2;											//戻るフラグへ
	}else{															//ボタンを押してないとき、回転が戻る
		float cnt=0;
		cnt = lean_cnt;
		if( pow <=1.5f ) pow += cnt;								//回転カウントからバットのパワーためる
		else if( pow>1.5f ) pow = 1.5f;								//パワー豊和処理 最大1.5
		cnt -= lean_speed;											//回転速度から現在のカウントを引く
		if( cnt > 0 ){												//計算結果が0以上なら
			lean_cnt =cnt;											//バットが画面奥へ回転する(スイング中)
			PlaySoundMem(se_swing, DX_PLAYTYPE_BACK );
		}else{														//傾きが0以下	
			lean_cnt = 0;
			lean_flag= 0;
		}
	}
}

//ボールとの接触判定
/*
Ball	*_ball		:ボールのクラス
*/
void Bat::Collision( Ball *_ball )
{
	BoundingSphere3D sphere = _ball->getBoundingObj();		//引数から当たり判定の情報を取得
	//バットに当たった判定
	if( _ball->getStateFlag()!=1 )return;					//ボールがピッチング中のみ
	Vector dir = _ball->getDirectValue();				//跳ね返る前の移動量を保存
	if( Rebound::Refrect( sphere,line,&dir ) )			//当たった
	{
		_ball->setMakyuFlag(false);						//魔球状態をなくす
		PlaySoundMem(se_hitting, DX_PLAYTYPE_BACK );
		float ran = Math::rand()*2-1;					//多少変化を与える
		float rot = _ball->getRotateValue();
		rot = -rot;
		_ball->setRotValue(rot);						//回転も反転
		dir.x += ran;									//跳ね返す方向にランダム性を加える
		
		//行列反射分加算した新たな移動量
		_ball->setDirectValue( dir );					//反射軸を方向とする
		float speed = _ball->getVelosity();
		_ball->setVelosity( speed*this->pow );			//スイング力＋球速
		_ball->setStateFlag( 2 );						//ボールの状態を"打った"に	
	}
}

//描画
/*
IRender		*render	:Direct3Dのレンダリング
Matrix4x4	&vps	:view.pos.scale行列
*/
void Bat::Draw( IRender *render, Matrix4x4 &vps )
{
	PointLight3D light( Vector(0,1,0),Color(0,1,0),100000 );	//ライティング
	Light3D *lights[] = { &light };								//ライトデータ

	render->setTexture( img );			
	for(int i=0;i<model.getObjectCount();i++)		//モデル個数分描画
	{
		model[i].Transform( mat_bat,vps );
		model[i].Lighting( lights,1, Color(1,1,1,1) );
		model[i].Draw( render );
	}
	render->setTexture( NULL );	
	
	//影
	Matrix shadow,mm;
	Vector dir;						//方向用ベクトル
	dir = pos-light.getPos();		//光線方向  光源からオブジェクトに対してのベクトル
	
	float t = (dir.x/dir.y);		//影の割合を求める
	if( t<0 ) t = -t;				//絶対値
	float t2 =(dir.z/dir.y);		//Z方向も
	if( t2<0 ) t2=-t2;
	
	shadow.Scale( 1,0,1 );			//バット用の影のサイズ
	mm.Translate( 1,-0.05f,0 );
	
	//影専用の表示
	render->setTexture( NULL );
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].setColor( 0x4f000000 );			//影の色
		model[i].Transform(shadow*mat_bat, vps);	//影とオブジェクトの掛け合わせ
		model[i].Draw( render );					//バット用陰の表示
	}
}

//打席の変更
/*
BATTER_TYPE _type	:新規に入れたい打席の情報
*/
void Bat::setBatter( BATTER_TYPE _type )
{
	switch(_type){
		case LEFT : 
			pos = Vector3(-10,0,-103);
			line.s = pos;						//判定座標も更新
			rot = 86.3f;
			lean_rot = 3.8f;					//限界角度設定し直す
			type = _type;
			break;
		case RIGHT :
			pos = Vector( 8,0,-103);
			line.s = pos;
			rot = 180.53f;
			lean_rot = -3.8f;
			type = _type;
			break;
		default : throw system::Exception("指定された列挙体がありません(BATTER_TYPE)");
	}
}