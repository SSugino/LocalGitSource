/** @file
 * 野球盤ルールクラスの実装
 * @date 2009/10/15
 * @author 杉野
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


//回が進んだ時の初期化。アウトカウントも全て初期化させる
/*
Ball	*bbase	:ボールクラス
Bat		&_bat	:バットクラス
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
	
	//ピッチャーの球速初期化
	bbase->setVelosity( 0.75f );			//低速で初期化
	//バッターの打席初期化
	_bat.setBatter( _bat.LEFT );		//左打席に


	//アルファ値の初期化
	alpha	= 0xafFFFFFF;
	r_alpha = 0xdfFFFFFF;
}		

//打者が新しくなった時に初期化する
void Rule::BatterInit()
{
	strike_add_flag = false;
	ball_add_flag = false;
	
	//アウト以外のカウント初期化
	int i;
	for(i=0;i<sizeof(strikeArray)>>2;i++ ) strikeArray[i] = 0;
	for(i=0;i<sizeof(ballArray)>>2;i++ ) ballArray[i] = 0;
	ball = 0;
	strike = 0;
	r_alpha = 0xdfFFFFFF;
}

//交代処理
/*
BasketManager	&bm		:カゴの管理クラス
PlayerManager	&pm		:プレイヤーの管理クラス
Ball			*bbase	:ボールの管理クラス
STATE			*p1		:ﾌﾟﾚｲﾔｰ1Pの状態
STATE			*p2		:プレイヤー2Pの状態
*/
void Rule::Change( BasketManager &bm,PlayerManager &pm,Ball *bbase,STATE *p1,STATE *p2 )
{
	PlayerBase *p;
	if( change_flag )		//3アウトによる交代フラグが立ったら
	{
		//エディットで配置されたカゴを初期化する
		bm.Dispose();
		bm.Init();
		//出現していたランナーを伏せる
		pm.DeleteRunner();
		p = pm.getPlayerBase(2);				//リスト2番目を取得し,(投手)球速も再度設定しなおす
		p->setSpeed( 1.2f );					//次の回の初期球速度のセット

		if( ColAlpha() )						//Changeと表示させなおかつアルファ値が１を超えた
		{
			//攻守の入れ替え
			if( *p1==ATTACK ) *p1 = DEFFENCE;	//攻撃だったのなら守備に
			else *p1 = ATTACK;

			if( *p2==ATTACK ){
				*p2 = DEFFENCE;
				cnt++;							//回を進める
			}
			else *p2 = ATTACK;
			
			change_flag = false;				//二度目は入らないように
		}
	}
}

//カウントのパターン計算
void Rule::CalPattern()
{
	//カウントから配列中身と関連付け
	if( strike>0 ) strikeArray[ strike ]	= 1;
	if( ball>0 )ballArray[ ball ]			= 1;
	if( out>0 ) outArray[ out ]				= 1;

	//得点のパターンを決める
	p1pat = p1score%10;		//1ｹﾀ
	p2pat = p2score%10;
	p1pat1 = p1score/10;	//10ｹﾀ
	p2pat1 = p2score/10;	

}

//Changeテクスチャのアルファ計算
bool Rule::ColAlpha()
{
	Color col( alpha );
	if( end_flag==false )
	{
		if( col.a>=1.0f ) return true;
		else col.a+=0.006f;
	}else col.a = 0;
	alpha = col;			//更新
	return false;
}

//ヒットなどの結果表示用のアルファ計算
/*
float add	:加算量
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
		r_alpha = col;			//更新
	}
	return false;
}

//アウト処理
/*
Ball *bbase	:ボールクラス
Bat  *_bat	:バットクラス
*/
void Rule::Out( Ball *bbase,Bat *_bat )
{
	if( is_out_flag )
	{
		result = 2;									//ルールクラス上での結果
		int n = bbase->getStateFlag();
		if( out<2 )
		{
			PlaySoundMem(se_out, DX_PLAYTYPE_BACK );
			out++;
			//アウトとボールカウントの初期化
			BatterInit();
			if( n==2 ) bbase->setStateFlag(4);		//打っていたのならカメラ演出
			else 
			{
				bbase->setStateFlag(0);			//三振なので演出不要
			}
		}
		else if( out == 2 )							//３アウト
		{
			change_flag = true;
			NextInit( bbase,*_bat );				//カウントの初期化
			PlaySoundMem(se_change, DX_PLAYTYPE_BACK );
			if( n==2 )bbase->setStateFlag(4);					//球の出現状態を伏せる△
			else bbase->setStateFlag(0);
		}
		is_out_flag = false;
	}
}

//カウント増加処理
/*
Ball	*bbase	:ボールクラス
Ground	*ground	:グラウンドクラス
Bat		&_bat	:バットクラス
*/
void Rule::addCount( Ball *bbase,Ground *ground,Bat &_bat )
{
	if( bbase->BackNetCollision()==1 )							//目標地点に到達したときに判定(バックネット)
	{
		//三振アウト
		//一回も処理してなく、2ストライクでなおかつ、またストライクゾーンを通過したし、カウントを増加フラグ
		if( strikeArray[2] ==1 && this->strike_add_flag )	
		{ 
			PlaySoundMem(se_out, DX_PLAYTYPE_BACK );
			is_out_flag = true;
		}
		//フォアボール
		else if( ballArray[3] ==1 && this->ball_add_flag )	
		{
			BatterInit();
			foreball_flag = true;
			result = 3;
			PlaySoundMem(se_foreball, DX_PLAYTYPE_BACK );
			bbase->setStateFlag(0);					//球の出現状態を伏せる
		}
		//ストライク
		else if( strike_add_flag  )
		{
			int st = strike;
			if( strike<2 )
			{
				strike++;			//デバッグ用に加算しない
				st = strike;
				PlaySoundMem(se_strike, DX_PLAYTYPE_BACK );
				bbase->setStateFlag(0);				//球の出現状態を伏せる　△
			}
		}
		//ボール
		else if( ball_add_flag )
		{
			if( ball<3 )
			{
				ball++;			//デバッグ用に加算しない
				PlaySoundMem(se_ball, DX_PLAYTYPE_BACK );
				bbase->setStateFlag(0);
			}
		}
	}
}

//得点加算
/*
STATE *p1	:プレイヤー１P状態
*/
void Rule::AddScore( STATE *p1 )
{
	if( add_score_flag )							//加算条件が揃った
	{
		if( *p1 == ATTACK )							//プレイヤー1が攻撃なら
		{
			p1score+=point;		//最大99点以下なら加算
			if( p1score>=99 ) p1score = 99;			//飽和処理 
		}else{
			p2score+=point;		//裏
			if( p2score>=99 ) p2score = 99; 
		}
		add_score_flag = false;
	}else point = 1;								//基本の加算時の得点
}

//打った後の結果
/*
Ball			*bbase	:ボールクラス
Ground			*ground	:グラウンドクラス
BasketManager	&bm		:カゴ管理クラス
PlayerManager	&pm		:プレイヤー管理クラス
*/
void Rule::Result( Ball	*bbase	, Ground *ground,BasketManager &bm,PlayerManager &pm )
{
	std::list<Basket *>::iterator i;
	i = bm.getBasket().begin();						//iを先頭(begin)に
	while( i != bm.getBasket().end() ){				//終わるまで
		Basket *b = *i;
		this->BasketResult( bbase,ground,b );		//カゴ判定
		this->HitResult( bbase,ground,b,pm );		//ヒット
		++i;
	}
}

//カゴがボールを取得した時の結果
/*
Ball			*bbase	:ボールクラス
Ground			*ground	:グラウンドクラス
Basket			*bask	:カゴクラス
*/
void Rule::BasketResult( Ball	*bbase, Ground *ground,Basket *bask )
{
	if( bbase->getStateFlag()==0 )					//ボールが出現してない
	{
		bask->setCatchFlag( false );				//ボールを取るフラグをおろしておく
	}
	else if( bbase->getStateFlag()==2 )				//打ち返したとき
	{
		if( bask->getCatchFlag()  )					//カゴに入っていた
		{ 
			is_out_flag = true;
		}
	}
}

//ヒットを打ったときの結果
/*
Ball			*bbase	:ボールクラス
Ground			*ground	:グラウンドクラス
Basket			*bask	:カゴクラス
PlayerManager	&pm		:プレイヤー管理クラス
*/
void Rule::HitResult( Ball	*bbase, Ground *ground,Basket *bask, PlayerManager &pm )
{
	if( bbase->getStateFlag()==0 ){									//ボールが出現してないとき							
		normal_hit_flag = false;									//通常ヒットフラグをおろす
		fence_hit_flag	= false;									//フェンスによるヒットフラグを下ろす
	}
	else if( bbase->getStateFlag()==2 )								//打ち返したとき
	{
		//カゴは取ってないし、球のスピードもなく、ファールでもない,フォアボールでもない。通常ヒットの時
		if(  !ground->getResultState() && this->foreball_flag==false )
		{
			int n;
			//カゴが取ってなく、球の勢いが0以下でなおかつ打ったとしても止まった先がホームベース付近でない
			if( !bask->getCatchFlag() && bbase->getVelosity()<=0  && !InField( bbase ))//通常ヒットのフラグを立てる
			{
				normal_hit_flag = true;
				PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); 
				result = 1;
			}
			else if( ( n = ground->Collision(bbase) ) > 0 )
			{
				//パターン番号を決める
				if(n>=1 && n<=3 ){ result = 1;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }					//ヒット
				else if( n==4 ) result = 5;						//ホームラン
				
				if(n==1) {normal_hit_flag = true;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//シングルヒット
				else if(n==2){ fence_hit_flag = 2;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//ツーベースヒット
				else if(n==3){ fence_hit_flag = 3;PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); }				//スリーベースヒット
				else if(n==4){ fence_hit_flag = 4;PlaySoundMem(se_homerun, DX_PLAYTYPE_BACK ); }				//ホームラン
			}else if( 0==ground->Collision(bbase) )				//アウトフェンス
			{
				is_out_flag = true;								//アウトフラグを立てる Out関数へ

			}
			else if( InField( bbase ))							//かごの取れない場所なら
			{
				is_out_flag = true;								//アウトフラグを立てる Out関数へ
			}
		}
	}
}

//初期化
/*
IImage *count_img	:数字のテクスチャ
IImage *result_img	:結果のテクスチャ
IImage *score_img	:スコアのテクスチャ
IImage *side_img	:～側攻撃のテクスチャ
IImage *change_img	:交代のテクスチャ
*/
void Rule::Init( IImage *count_img, IImage *result_img, IImage *score_img,IImage *side_img,IImage *change_img )
{
	img_count	= count_img;
	img_result	= result_img;
	img_score	= score_img;
	img_cntside	= side_img;
	img_change	= change_img;

	//アルファ値の初期化
	alpha = 0xafFFFFFF;
	r_alpha = 0xffFFFFFF;

	//各種効果音
	se_strike = LoadSoundMem( "data/se/strike.wav" );
	se_ball = LoadSoundMem( "data/se/ball.wav" ); 
	se_hit = LoadSoundMem( "data/se/hit.wav" );
	se_out = LoadSoundMem( "data/se/out.wav" );
	se_foreball = LoadSoundMem( "data/se/foreball.wav" );
	se_change = LoadSoundMem( "data/se/change.wav" );
	se_homerun = LoadSoundMem( "data/se/homerun.wav" );
	
}

//ゲームセット処理(試合終了)
/*
STATE *p1	:プレイヤー１Pの状態
*/
int Rule::GameSet( STATE *p1 )
{
	if( change_flag )//交代のフラグが立っていたなら
	{
		//9回以上からアウトになった時
		if( 9<=cnt && cnt<=12 )
		{
			//プレイヤー1が攻撃中に（先攻）だった時に勝っていた  =  後攻の攻撃へ
			if( *p1==DEFFENCE && p1score>p2score )
			{
				end_flag = true;
				return 6;			//先攻の勝ち
			}
			else if( p1score<p2score )
			{
				end_flag = true;
				return 7;			//後攻の勝ち
			}else if( p1score==p2score && cnt==12 && *p1==DEFFENCE ) //引き分け
			{
				end_flag = true;
				return 8;
			}
		}
	}
	return 0;
}

//BallStateが0のときの初期化
void Rule::isReturn()
{
	result = 0;
	r_alpha = 0xdfFFFFFF;
	alpha_cnt = 0;
	home_x = 645;						
	home_mov = 2;
	//とにかくヒットだったら(通常ヒット、フェンス判定によるヒット）
	if( hit_flag )
	{
		//アウトとボールカウントの初期化
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

//各処理を呼ぶメイン.
/*
Ball			*bbase	:ボールクラス
Ground			*ground	:グラウンドクラス
BasketManager	&bm		:カゴ管理クラス
PlayerManager	&pm		:プレイヤー管理クラス
Bat				&_bat	:バットクラス
STATE			*p1		:ﾌﾟﾚｲﾔｰ1Pの状態
STATE			*p2		:ﾌﾟﾚｲﾔｰ2Pの状態
int				*scene_f:シーンの状態
*/
void Rule::Main( Ball *bbase,Ground *ground, baseball::BasketManager &bm,
				 PlayerManager &pm,Bat &_bat,STATE *p1,STATE *p2,int *scene_f )
{
	state	= *p1;										//攻守の状態を関連付けておく
	int f	=0;											//ゲームセットのフラグ
	
	Out( bbase,&_bat );									//アウトの処理
	if( ( f=GameSet(p1) )==0 )							//試合が終わってなければ
	{
		Change( bm,pm,bbase,p1,p2 );					//引数で受け取ったプレイヤーの攻守状態を交替,交代における初期化等
		
		if( bbase->getStateFlag()==0 )					//ボールがない間はフラグ類を初期化しておく
		{
			isReturn();									//カウントとは別の初期化
		}else if( bbase->getStateFlag()==4 )
		{
			if( result== 5 ) DrawMove();				//ホームラン時の画像移動
		}

		Result( bbase,ground,bm,pm );					//カゴがボールを取ったときの判定

		addCount( bbase,ground,_bat );					//カウント処理
		
		CalPattern();									//カウントのパターン計算

		AddScore( p1 );									//得点加算
	}else{												//終わったので結果発表へ
		*scene_f = f;									//戻り値をシーンのフラグに代入する
	}
}

//打った結果がホームよりの判定
/*
Ball *ball	:ボールクラス
*/
bool Rule::InField( Ball *ball )
{
	if( ball->getStateFlag()==2 )
	{
		if( ball->getVelosity()==0 )	//勢いが無い時に
		{
			//ベースに近すぎなら
			if( Collision3D::Collision( b_sphere,ball->getBoundingObj() ))
			{
				int n= getResult();
				if( n!=4 || n==0 ) //ファ-ルでない時か
				{
					return true;		//アウト
				}
			}
		}
	}
	return false;
}

//描画の移動
bool Rule::DrawMove()
{
	if( result==5 )
	{
		if( alpha_cnt==3) return true;
		home_x -= home_mov;
		if( home_x+256 <= 0 )
		{	
			alpha_cnt++;			//Moveだがやってることや呼ばれる場所はColAlphaと同じなので使いまわす
			home_x = 645;
		}
	}
	return false;
}

//描画
/*
IRender		*render	:レンダリングクラス
Matrix4x4	&vps	:カメラ行列
Bat			&_bat	:バットクラス参照
*/
void Rule::Draw( IRender *render, Matrix4x4 &vps,Bat &_bat )
{
	//表示場所初期化
	const int st_x=510,st_y=370;				//ストライク座標
	const int ba_x=st_x,ba_y=st_y+30;			//ボール座標
	const int ou_x=st_x,ou_y=st_y+60;			//アウト座標
	const int sc_x=256,sc_y = 62;				//スコア座標
	
	int right_type = 0;							//右打席時の移動
	if( _bat.getBatter() == Bat::RIGHT )
	{
		right_type = -490;
	}

	//ストライク
	render->drawImage( st_x+right_type,st_y,	32,32,img_count,strikeArray[1]*32,0 );			
	render->drawImage( st_x+30+right_type,st_y,32,32,img_count,strikeArray[2]*32,0 );	
	
	//ボール
	render->drawImage( ba_x+right_type,ba_y,	32,32,img_count,ballArray[1]*64,0 );	
	render->drawImage( ba_x+30+right_type,ba_y,32,32,img_count,ballArray[2]*64,0 );	
	render->drawImage( ba_x+60+right_type,ba_y,32,32,img_count,ballArray[3]*64,0 );	
	
	//アウト
	render->drawImage( ou_x+right_type,ou_y,	32,32,img_count,outArray[1]*96,0 );
	render->drawImage( ou_x+30+right_type,ou_y,32,32,img_count,outArray[2]*96,0 );	
	
	render->drawImage( sc_x,sc_y,32,32,img_score,p1pat*32,0);		//プレイヤー１のスコア
	render->drawImage( sc_x-16,sc_y,32,32,img_score,(p1pat1*32),0);

	render->drawImage( sc_x+35,sc_y,32,32,img_score,0,32);			//区切り
	
	render->drawImage( sc_x+82,sc_y,32,32,img_score,p2pat*32,0);	//プレイヤー２のスコア	
	render->drawImage( sc_x+66,sc_y,32,32,img_score,p2pat1*32,0);	
	
	//攻撃側に　＿　を表示
	if( state == ATTACK )											//プレイヤー１が攻撃なら
	{
		render->drawImage( sc_x-8,sc_y+18,32,32,img_score,32,32);
		render->drawImage( 290,38,32,32,img_cntside,32,32);			//裏表表示
	}else{
		render->drawImage( sc_x+76,sc_y+18,32,32,img_score,32,32);
		render->drawImage( 290,38,32,32,img_cntside,64,32);	
	}
	
	//回を表示
	render->drawImage( 270,5,32,32,img_cntside,64+(cnt*32),0);	
	render->drawImage( 300,5,32,32,img_cntside,0,0);
	
	//交代
	if( change_flag && end_flag ==false ) 
	{
		render->drawImage( 180,130,256,256,img_change,0,0,alpha);
	}
	
	//アウトやヒットなどの表示
	if( 0<result && result<5 ) render->drawImage( 180,130,256,128,this->img_result,0,128*result,r_alpha);
	else if( result==5 )render->drawImage( home_x,130,256,128,this->img_result,0,128*result,0xffFFFFFF);

}