/** @file
 * プレイヤー(選手）クラスの実装
 * @date 2009/09/04
 * @author 杉野
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



//ピッチャー～キャッチャーまでのキービットデータ配列
int key_bit_array[30*60*60];
int pitch_data_buf[30*60*60];	//いい投球ならこれにデータを残しておく
int get_FC;						//配列要素番号
extern int g_cpuFC;

//派生先の関数
//ランナーの行列計算
void Runner::calMatrix()
{
	rot += rot_val;
	mat_r.RotateY( rot );
	mat_sc.Scale( scale );
	mat_t.Translate( pos );
	mat_run = mat_r*mat_sc*mat_t;


}

//ランナーの各関数を呼ぶメイン
/*
Key			&key	:キークラス
Ball		*_ball	:ボールクラス
Rule		*rule	:ルールクラス
Rule::STATE &p1state:プレイヤー１Pの情報
*/
void Runner::Main( Key &key,Ball *_ball, Rule *rule, Rule::STATE &p1state )
{
	Collision( _ball,rule );		//ボールがランナーに当たったのならアウトにするためのコリジョン
	calMatrix();
	pos.y-=gravity;
	//当たり判定への関連付け
	bobox	= BoundingOBox3D( this->pos+Vector(0,0,-8),Vector(1.5f,scale.y*2.5f,0.25f),this->mat_r );
}

//ランナーの描画
/*
IRender *render	:DirectX9レンダリング
Matrix  &vps	:view.pos.scale行列掛け合わせ
*/
void Runner::Draw( IRender *render,Matrix &vps )
{
	if( state )												//基底が出現可能状態
	{	
		render->setTexture( img );
		for( int i=0;i<model->getObjectCount();i++)
		{
			Object3D *obj = model->getObject(i);			//参照が多いので、受け取る
		
			obj->Transform(this->mat_run, vps);				//取得して変換
			obj->Draw( render );							//表示
			
		}
		render->setTexture( NULL );
	}
}

//ランナーの接触判定
/*
Ball *ball	:ボールクラス
Rule *rule	:ルールクラス
*/
void Runner::Collision( Ball *ball,Rule *rule )
{
	//派生先のボールから当たり判定の情報を取得
	BoundingSphere3D sphere = ball->getBoundingObj();
	if( this->getState() )										//ランナー出現中
	{
		Vector	vret;
		if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
		{
			state = false;										//アウトになる
			rule->setIsOutFlag(true );							//アウト演出のフラグをONに
		};
	}
}

//ピッチャーの行列計算
void Picher::calMatrix()
{
	mat_r.RotateX(rotate);
	mat_sc.Scale( 5,5,5 );
	mat_t.Translate( pos );
	mat_pich = mat_sc * mat_r * mat_t;
}

//ピッチャーの各関数を呼ぶメイン
/*
Key			&key		:キークラス参照
Ball		*_ball		:ボールクラス
Rule		*rule		:ルールクラス
Rule::STATE &p1state	:１Pプレイヤーの状態
*/
void Picher::Main( Key &key,Ball *_ball,Rule *rule,Rule::STATE &p1state)
{
			//キーデータを配列化
	key_bit_array[get_FC++] = key.getKeyData();			//変化球情報などを配列に保存していく


	if( _ball->getStateFlag()==0 )									//ボールが出現してない(一回のみ処理を含む)（交代時の初期化も含む）
	{
		key.gFc_Init();

		//_ball->setPos( Vector(0.1444f,1,-40.48f ));					//発射座標リセット
		_ball->setPos( Vector(pos.x,1,pos.z -0.48f ));					//発射座標リセット
		_ball->setTarget( Vector(pos.x,1.40f,-123.344f ) );			//球の目的地リセット	
		_ball->setDirectValue( _ball->getTarget() - pos );			//球の放出方向をリセット
		_ball->setMakyuFlag( false );
		

		if( p1state==Rule::DEFFENCE )									//プレイヤー1が守備
		{
			if( key.isDown( Key::KEY_A))			//Aキーで投げる位置変更
			{							
				if( pos.x > -6.5f )  pos.x-=0.15f;
			}
			if( key.isDown( Key::KEY_D ))
			{
				 if( pos.x < 6.5f )pos.x+=0.15f;
			}

		}else if (p1state == Rule::ATTACK )
		{
			if( key.isDown( Key::SEMICORON))			//Aキーで投げる位置変更
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
			Shot( key,_ball,rule,p1state );		//交代画面を出してない時のみ発射関数
		}

	}else if( _ball->getStateFlag()==1 )								//Stateフラグ１:出現中の処理(投げてる)
	{	

		_ball->setMakyuFlag( false );
		if( p1state==Rule::DEFFENCE )									//プレイヤー1が守備
		{
			//Sキーで魔球始動
			if( key.isDown( Key::KEY_S) ) _ball->setMakyuFlag( true );		
		}else{
			// :キーで魔球
			if( key.isDown( Key::CONMA ) ) _ball->setMakyuFlag( true );
		}

		//キャッチャーに当たった
		if( _ball->BackNetCollision()==1 )
		{
			pos.x = 0;		//ピッチャーの位置を初期位置へ

			//球筋データの保存///////////////////
			key_bit_array[get_FC] = -1;			//データ末端に終わり情報として-1いれる
			//*pitch_data_buf = *key_bit_array;	//保存
			//投球データを書き出す（開発用）
			FILE *fp=fopen("pitch_data.txt","w");
			//ファイルに出力
			fprintf(fp,"{\n" );
			for(int i=0;i<60*30*30;i++)
			{
				if( key_bit_array[i] == -1 )break;		//末端なら抜ける
				if(i%10 == 0) 			fprintf(fp,"\n" );	//10書き込むごとに改行
				fprintf(fp,"%d,",key_bit_array[i]);
					
			}
			fprintf(fp,"\n" );
			fprintf(fp,"}" );
			fclose(fp);	
			////////////////////////////////////////
			
			key.gFc_Init();
		}
	}

	HandMotion( _ball );							//投げるモーションをさせる
	calMatrix();
}

//手の回転
/*
Ball *_ball	:ボールクラス
*/
void Picher::HandMotion( Ball *_ball )
{	
	if(rot_flag)
	{
		if(rot_reverse!=false)return;						//投げていなければ戻る
		rot_cnt--;											//手の回転カウント
		rot_val = -0.05f;
		rotate+= rot_val;

		if( rot_save+5 ==  rot_cnt ){
			_ball->setVelosity( speed );					//球速をセット
			_ball->setStateFlag(1);							//玉の状態を「投球後」に
		}
		if( rot_cnt<0 ){ 									//手の回転が回転しきった
			rot_reverse = true;								//回転しきったフラグ
			rot_flag = false;
		}
	}else{
		if(rot_reverse!=true)return;
		rot_cnt++;											//手の回転カウントを戻していく
		rot_val = 0.05f;
		rotate+= rot_val;

		if( rot_cnt>59 )									//60カウントが手のカウント上限なので
		{
			rot_reverse = false;
			rotate = 0;
		}
	}
}

//弾の発射
/*
Key			&key	:キークラス
Ball		*_ball	:ボールクラス
Rule		*rule	:ルールクラス
Rule::STATE &p1state:プレイヤーの状態
*/
void Picher::Shot( Key &key,Ball *_ball,Rule *rule,Rule::STATE &p1state )
{
	

	if(rot_flag==false && rot_reverse==false )					//回転開始フラグも回転戻るフラグも降りてなければならない
	{
		if( p1state==Rule::DEFFENCE )							//プレイヤー1が守備の時
		{
			//速度わけ(最低速～最高速まで)			
			if( key.isOn( Key::NUM_ONE )) speed = 0.75f;		    //最低速 1
			else if( key.isOn( Key::NUM_TWO )) speed = 0.85f;		//低速	2キー
			else if( key.isOn( Key::NUM_THREE )) speed = 1.12f;		//中速　3キー
			else if( key.isOn( Key::NUM_FOUR )) speed = 1.5f;		//高速　4キー
			else if( key.isOn( Key::NUM_FIVE )) speed = 1.85f;		//最高速5キー
			
			if(speed)_ball->setVelosity(speed);

			if( key.isOn( Key::KEY_W)) rot_flag = true;		//手を回転させる
		}else if( p1state==Rule::ATTACK )						//プレイヤー2がディフェンス
		{
			if( key.isOn( Key::NUM_ZERO ))speed = 0.75f;		//低速	0キー
			else if( key.isOn( Key::ECORL ))speed = 0.85f;		//-
			else if( key.isOn( Key::CARET )) speed = 1.12f;		//^	
			else if( key.isOn( Key::YEN )) speed = 1.5f;		//\	
			else if( key.isOn( Key::BACKSPACE )) speed = 1.85f;		//低速	BS
			
			if(speed)_ball->setVelosity(speed);

			if( key.isOn( Key::AT_MARK))
			{
				rot_flag = true;
			}
		}
	}
}

//ピッチャーの描画
/*
IRender *render	:Direct3D9 レンダリング
Matrix	&vps	:view.pos.scale
*/
void Picher::Draw( IRender *render, Matrix &vps )
{
	for( int i=0;i<model->getObjectCount();i++)
	{
		model->getObject(i)->Transform(this->mat_pich, vps);		//取得して変換
		render->setTexture(img);
		model->getObject(i)->Draw( render );						//表示
		render->setTexture( NULL );
	}	
}

//ピッチャーの接触判定
/*
Ball	*ball	:ボールクラス
*/
void Picher::Collision( Ball *ball )
{
	//派生先のボールから当たり判定の情報を取得
	BoundingSphere3D sphere = ball->getBoundingObj();

	//当たった判定
	Vector	vret;
	if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
	{
		ball->revisePos( vret );									//補正
	};
}

//ピッチャーとランナーをまとめるマネージャーの初期化
void PlayerManager::Init()
{
	//ランナーを"3体"追加
	this->setPlayer( new_s Runner( Vector(39,1,-40),Vector(2.5f,2.5f,2.5f),(Math::PI*45)/180,0,&mdl_runner,img_runner1,0.098f,false ) );
	this->setPlayer( new_s Runner( Vector(-11,1,-10),Vector(2.5f,2.5f,2.5f),(Math::PI*-15)/180,0,&mdl_runner,img_runner2,0.098f,false) );
	this->setPlayer( new_s Runner( Vector(-44,1,-48),Vector(2.5f,2.5f,2.5f),(Math::PI*-115)/180,0,&mdl_runner,img_runner1,0.098f,false) );

	//ピッチャー（発射台)の追加
	this->setPlayer( new_s Picher( Vector(0,0,-40),Vector(1,1,1),0,0,&mdl_picher,img_pich,0.098f,true ));
	
	//投球配列保存用フレームカウント初期化
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
	
	se_home_run = LoadSoundMem("data/se/gaya.mp3");			//ホームランの効果音
	se_hakushu = LoadSoundMem("data/se/hakusyu.mp3");		//拍手の効果音

}

//ピッチャーとランナーをまとめるマネージャーのメイン
/*
Key						*key		:キークラス
Ground					*g			:グラウンドクラス
Ball					*_ball		:ボールクラス
BasketManager			*baskM		:エディットの守備カゴクラス
Rule *rule,Rule::STATE  &p1state	:プレイヤーの情報
*/
void PlayerManager::Main( Key *key,Ground *g,Ball *_ball,BasketManager *baskM,Rule *rule,Rule::STATE &p1state )
{	
	CalColor();			//アルファ値の計算
	CalPattern();
	PlayerBase *three = this->getPlayerBase(1);
	std::list<PlayerBase *>::iterator i;
	i = player.begin();								
	while(i != player.end()){						
		PlayerBase *p = *i;
		g->IntersectFunction( NULL,NULL,p,NULL );		//グラウンドとの判定
		
		if( rule->getForeballFlag() )						//フォアボール時の処理
		{	
			if( three->getState() )							//3塁から調べフラグが立っていたのなら
			{
				rule->setAddScoreFlag(true);				//押し出し点
			}
			if( setPlayerState(0,true) )					//全塁調べあるなら次の塁のフラグを立ててていく
			{
				rule->setForeballFlag( false );
			}
			rule->setForeballFlag( false );
		}
		//通常ヒットの時。カゴが取ってなく、なおかつグランド上で、止まったとき,ただしカゴが絶対に取れないホームよりはアウトとする
		else if( rule->getNormalHitFlag() && g->getResultState()==0 && baskM->getCatchFlag()==false && !rule->InField(_ball)  )	
		{
			SingleHitGoRunner( _ball, rule );						//塁を進める
			rule->setNormalHitFlag( false );
			rule->setHitFlag( true );												//カウント初期化条件フラグ
			PlaySoundMem(se_hakushu, DX_PLAYTYPE_BACK );
			//PlaySoundMem(se_hit, DX_PLAYTYPE_BACK ); 
		}
		//フェンス判定
		else if( rule->getFenceHitFlag() && !rule->getNormalHitFlag() )				//フェンスによる結果が得られる壁の時
		{
			int n = g->Collision( _ball );							//フェンスの情報を取得
			if( n==1 ) SingleHitGoRunner( _ball,rule );
			else if( n==2 ) TwoBaseHitGoRunner( _ball,rule );
			else if( n==3 ) ThreeBaseHitGoRunner( _ball,rule );
			else if( n==4 ) HomeRunGoRunner( _ball,rule );
			else rule->setIsOutFlag( true );										//アウトのフェンスなのでアウト処理フラグを立てる
			rule->setFenceHitFlag( false );
			rule->setHitFlag( true );												//カウント初期化条件フラグ
			
		}
		p->Main( *key,_ball,rule,p1state );		//派生先のMain
		++i;
	}
}

//ピッチャーとランナーをまとめるマネージャーの描画
/*
IRender		*render	:レンダリングクラス
Camera3D	&cam	:カメラクラス
Bat			&_bat	:バットクラス
*/
void PlayerManager::Draw( IRender *render,Camera3D &cam,Bat &_bat )
{	
	std::list<PlayerBase *>::iterator i;
	i = player.begin();							//先頭(begin)に
	while(i != player.end()){					//終わるまで
		PlayerBase *p = *i;
		p->Draw(render, cam.getMatrix() );					//派生先の描画
		++i;
	}
	
	int right_type=0;
	if( _bat.getBatter() == Bat::RIGHT ) right_type = -493;	//打席変更時の座標

	//ランナー情報
	render->drawImage( 493+right_type,350,128,128,img_minirun,0,0 );
	render->drawImage( 493+right_type,350,128,128,img_minirun,mini_pat*128,0,alpha );
	render->setTexture(NULL);


}

//ピッチャーとランナーをまとめるマネージャーの解放
void PlayerManager::Dispose()
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();					//リストまわす
	while(i != player.end()){
		delete (*i);
		i = player.erase(i);
	}
}

//ピッチャーとランナーをまとめるマネージャーの画面右下のランナーのパターン計算
void PlayerManager::CalPattern()
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列で実体化
		++i;
		++j;
	}
	//三塁から調べパターンを決める	
	if( base[2]->getState() )
	{
		mini_pat =3;					//三塁パターン確定
		if( base[1]->getState() ){		//2、3塁パターン確定
			mini_pat = 6;
			if( base[0]->getState() ){	//満塁パターン確定
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
		mini_pat =2;		//2塁パターン確定
		if( base[0]->getState() ) mini_pat = 4;
	}
	else if( base[0]->getState() )
	{
		mini_pat =1;		//1塁確定 
	}
	else mini_pat = 0;		//塁なし
}

//ピッチャーとランナーをまとめるマネージャーの色計算
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

//派生先クラスの取得
/*
int here	:目的のマネージャー管理のオブジェクトまで
*/
PlayerBase *PlayerManager::getPlayerBase( int here )
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	for( int j=0;j<=here;j++)
	{
		++i;
	}
	PlayerBase *p = *i;		//実体化
	return p;
}

//シングルヒット
/*
Ball *bbase	:ボールクラス
Rule *rule	:ルールクラス
*/
void PlayerManager::SingleHitGoRunner( Ball *bbase,Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列で実体化
		++i;
		++j;
	}
	//三塁側からフラグを調べ、各塁の処理をする
	if( base[2]->getState() )
	{
		if( base[1]->getState() )
		{
			base[1]->setState(false);
			base[2]->setState(true);			//2塁にいなかったら消える
		}else base[2]->setState(false); 
		if( base[0]->getState() ) base[1]->setState(true);			//1塁にはいる
		rule->setAddScoreFlag( true );								//得点加算
	}
	else if( base[1]->getState() ){
		base[2]->setState(true);									//3塁へ
		if( !base[0]->getState()==1 )base[1]->setState(false);		//1塁にいなかったら消える
	}
	else if( base[0]->getState() ){
		base[1]->setState(true);									//ランナー1,2塁に
	}
	base[0]->setState(true);										//必ず立つので最後

	//配列とリスト関連付け直し
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//配列をリストに
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );
}

//ツーベース
/*
Ball *bbase	:ボールクラス
Rule *rule	:ルールクラス
*/
void PlayerManager::TwoBaseHitGoRunner( Ball *bbase, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列を実体化
		++i;
		++j;
	}
	//三塁側からフラグを調べ、各塁の処理
	if( base[2]->getState() )
	{
		if( base[1]->getState() ){							//2塁にいた
			rule->setPoint( 2 );							//2点加算させる
			base[1]->setState(false);					
		}
		if( base[0]->getState() )
		{
			base[0]->setState(false);
		}else base[2]->setState(false);						//1塁にいなかったら消える
		rule->setAddScoreFlag(true);
	}
	else if( base[1]->getState() )
	{
		if( base[0]->getState() ) base[2]->setState( true );
		rule->setAddScoreFlag(true);						//自身は立てたまま、得点は加算
	}
	else if( base[0]->getState() )
	{
		base[0]->setState(false);
		base[2]->setState(true);							//ランナー３塁に
	}
	base[1]->setState(true);								//必ず立つので最後
	//配列とリスト関連付け直し
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];										//配列をリストに
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );								//行動終了
}

//スリーベース
/*
Ball *bbase	:ボールクラス
Rule *rule	:ルールクラス
*/
void PlayerManager::ThreeBaseHitGoRunner( Ball *bbase, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列を実体化
		++i;
		++j;
	}
	//三塁側からフラグを調べ、各塁の処理
	if( base[2]->getState() )			//自身は立てたまま
	{
		if( base[1]->getState() )		//2塁にいたら
		{
			rule->setPoint(2);			//二点入る
			if( base[0]->getState() ){
				rule->setPoint(3);		//満塁の時
				base[0]->setState(false);
			}
			base[1]->setState(false);	//いなくする
		}
		if( base[0]->getState() )		//1-3塁にいたら
		{
			rule->setPoint(2);			//二点入る
			base[0]->setState(false);	//いなくする
		}
		rule->setAddScoreFlag(true);	//加算フラグを立てる
	}
	else if( base[1]->getState() )
	{
		if( base[0]->getState() )
		{
			rule->setPoint(2);			//二点入る
			base[0]->setState(false);
		}
		 base[1]->setState(false);
		rule->setAddScoreFlag(true);	//加算
	 }
	else if( base[0]->getState() )
	{
		base[0]->setState(false);		//1塁はいなくなる
		rule->setAddScoreFlag(true);	//加算フラグ
	}

	base[2]->setState(1);				//必ず立つので最後
	//配列とリスト関連付け直し
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//配列をリストに
		++i;
		++j;
	}
	bbase->setStateFlag( 4 );			//行動終了
}
//ホームラン
/*
Ball *bbase	:ボールクラス
Rule *rule	:ルールクラス
*/
void PlayerManager::HomeRunGoRunner( Ball *_ball, Rule *rule )
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列を実体化
		++i;
		++j;
	}
	//三塁側からフラグを調べ、各塁の処理
	int n = 1;							//得点　必ず1点入るので１
	if( base[2]->getState() ) n++;
	if( base[1]->getState() ) n++;
	if( base[0]->getState() ) n++;
	rule->setPoint(n);					//塁にいた分だけ加算

	//立っていたならフラグを伏せる
	if( base[2]->getState() ) base[2]->setState(false);
	if( base[1]->getState() ) base[1]->setState(false);
	if( base[0]->getState() ) base[0]->setState(false);
	
	rule->setAddScoreFlag(true);		//加算フラグを立てる

	//配列とリスト関連付け直し
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//配列をリストに
		++i;
		++j;
	}

	PlaySoundMem( se_home_run, DX_PLAYTYPE_BACK );
	_ball->setStateFlag( 4 );			//行動終了
}

//3アウト時の処理
void PlayerManager::DeleteRunner()
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		base [j] = *i;					//配列を実体化
		++i;
		++j;
	}
	//立っていたならフラグを伏せる
	if( base[2]->getState() ) base[2]->setState(false);
	if( base[1]->getState() ) base[1]->setState(false);
	if( base[0]->getState() ) base[0]->setState(false);
	
	//配列とリスト関連付け直し
	i = player.begin();
	j = 0;
	while(i != player.end() )
	{
		*i = base [j];					//配列をリストに
		++i;
		++j;
	}
}

//全リストを見て状態フラグが何個立っているか調べる	塁に何人いるか？
void PlayerManager::SarchPlayerState()
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	while(i != player.end() )
	{
		PlayerBase *p = *i;					//実体化
		if( p->getState() )	
		{
			if( runner_cnt<3 ) runner_cnt++;	//塁上にいるランナーの数
		}
		++i;
	}
}

//指定したリストの出現状態を立てる
/*
unsigned int num	:指定のリスト番号
bool		_state	:出現状態
*/
bool PlayerManager::setPlayerState( unsigned int num, bool _state )
{
	std::list<PlayerBase*>::iterator i;
	i = player.begin();
	for( unsigned int j=0;j<num;j++)
	{
		++i;						//指定回数までリストを進める
	}
	PlayerBase *p = *i;				//実体化
	if( !p->getState() )			//出現状態が伏せてあった
	{
		p->setState( _state );		//出現状態をセットしなおす
		return	true;
	}
	if( num< 2 )					//塁の飽和処理。2以上はピッチャーなので使用しない
	{
		num++;			
		if( setPlayerState( num,_state )) return true;	//次のリストがあるなら再帰的に次のリストへ
	}
	return false;
}

//カゴの行列計算
void Basket::calMatrix()
{
	rot += rot_val;

	this->mat_r.RotateY( rot );
	this->mat_r2.RotateY( rot+ 90*math::Math::PI/180 );	//90度の軸作成
	this->mat_t.Translate( pos );
	this->mat_sc.Scale( scale );
	this->mat_basket = mat_r*mat_sc*mat_t;
}

//カゴのメイン
/*
Key  &key	:キークラス
Ball *_base	:ボールクラス
Rule *rule	:ルールクラス
*/
void Basket::Main( Key &key,Ball *_base,Rule *rule )
{
	this->TransformVector();
	//当たり判定への関連付け
	bobox1	= BoundingOBox3D( pos,Vector(scale.x*1.2f,scale.y*0.6f,0.25f),mat_r );
	bobox2	= BoundingOBox3D( left_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
	bobox3	= BoundingOBox3D( right_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
	bobox4  = BoundingOBox3D( center_pos2,Vector(scale.x*1.2f,scale.y*0.6f,2.25f),mat_r );
	
	b_catch = BoundingOBox3D( forword_pos2,Vector(scale.x*0.7f,scale.y*0.7f,1.35f),mat_r );
	if( this->pos.y>0.5f )this->pos.y-=this->gravity;			//重力かける
	this->Collision( _base,rule );								//カゴ取得判定
	calMatrix();
}

//カゴの描画
/*
IRender		*render	:レンダリングクラス
Matrix4x4	&vps	:カメラクラス
*/
void Basket::Draw( IRender *render, Matrix4x4 &vps )
{
	for( int i=0;i<model->getObjectCount();i++)
	{
		model->getObject(i)->Transform(this->mat_basket, vps);		//取得して変換
		render->setTexture( img );
		model->getObject(i)->Draw( render );						//表示
		render->setTexture( NULL );
	}
}

//カゴの接触判定
/*
Ball *ball	:ボールクラス
Rule *rule	:ルールクラス
*/
void Basket::Collision( Ball *ball,Rule *rule )
{
	//派生先のボールから当たり判定の情報を取得
	sphere = ball->getBoundingObj();
	float speed = ball->getVelosity();
	const float size = sphere.s;
	Vector	vret;

	if( speed )		//ボールの速度があるなら
	{
		if( Collision3D::Collision( sphere,b_catch) && Collision3D::Collision( sphere,bobox1) )	//カゴとボールの判定
		{
			ball->setVelosity( 0 );			//ボールとめる
			catch_flag =  true ;			
		}
		else if( Intersect3D::Intersect( sphere,bobox1,&vret) )
		{
			speed/=2;						//速度緩める
			vret.Normalize();
			ball->setDirectValue(vret);		//反射軸セット
			ball->setVelosity(speed);
		}
		//サイドの壁
		else if( Intersect3D::Intersect( sphere,this->bobox2,&vret) )//左
		{
			Vector3 dir = center_pos2 - left_pos2;			//カゴの方向へ変化させる
			dir.Normalize();
			vret=dir;
			float ran = Math::rand()*2-1;					//多少変化を与える
			vret.x += ran;
			speed -= 0.035f;
			ball->setDirectValue( vret );		
			ball->setVelosity( speed );
		}
		else if( Intersect3D::Intersect( sphere,this->bobox3,&vret) )
		{		
			Vector3 dir = center_pos2 - left_pos2;			//カゴの方向へ変化させる
			dir.Normalize();
			vret=dir;
			float ran = Math::rand()*2-1;					//多少変化を与える
			vret.x += ran;
			speed -= 0.035f;
			ball->setDirectValue( vret );		
			ball->setVelosity( speed );
		}
	}else	//速さ0の時
	{		 
		if( !catch_flag && ball->getStateFlag()==2 )		//打ち返した時取ってなくファールでもなければ
		{
			rule->setNormalHitFlag(true);					//フェンス、ファール以外でのヒットは確定
		}
	}
}

//カゴのボーン座標(ローカル)からグローバル座標への変換
void Basket::TransformVector()
{
	Vector &v1 = left_pos1;						//ボーン1の今の座標

	Vector vv1;
	vv1 = mat_basket.TransformVector( v1 );		//グローバル座標に
	left_pos2 = Vector(vv1.x,vv1.y,vv1.z);		//新しい座標に変換

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

//カゴをまとめるマネージャーの初期化
void BasketManager::Init()
{	
	//守備のカゴを"7体"追加（捕手と投手除く) 守備範囲に個性をつける（二遊は広めなど
	this->setBasket( new_s Basket( Vector(40,0,-49),Vector(4,9,4),0.6f,13,&mdl_basket,img_basket,0.098f ) );			//一
	this->setBasket( new_s Basket( Vector(25,0,-25),Vector(5.5f,9,5.5f),0.38f,17,&mdl_basket,img_basket,0.098f ) );		//二
	this->setBasket( new_s Basket( Vector(-30,0,-20),Vector(5.5f,9,5.5f),-0.399f,17,&mdl_basket,img_basket,0.098f ) );	//遊
	this->setBasket( new_s Basket( Vector(-40,0,-55),Vector(4,9,4),-0.66f,13,&mdl_basket,img_basket,0.098f ) );			//三
	this->setBasket( new_s Basket( Vector(-60,0,40),Vector(5.5f,9,5.5f),-0.419f,26,&mdl_basket,img_basket,0.098f ) );			//左
	this->setBasket( new_s Basket( Vector(0,0,53),Vector(5.5f,9,5.5f),0.09f,30,&mdl_basket,img_basket,0.098f ) );				//中
	this->setBasket( new_s Basket( Vector(60,0,49),Vector(5.5f,9,5.5f),0.43f,26,&mdl_basket,img_basket,0.098f ) );			//右

	std::list<Basket *>::iterator i;
	i = basket.begin();	
	while(i != basket.end())
	{
		Basket *b = *i;
		b->Init();
		++i;
	}
}

//カゴをまとめるマネージャーのメイン
/*
Key		*key	:キークラス
Rule	*rule	:ルールクラス
Ground	*g		:グラウンドクラス
Ball	*ball	:ボールクラス
*/
void BasketManager::Main(  Key *key,Rule *rule,Ground *g,Ball *ball )
{
	catch_flag=false;					//次関数に入った時に上フラグの初期化
	std::list<Basket *>::iterator i;
	i = basket.begin();									//iを先頭(begin)に
	while(i != basket.end()){							//終わるまで
		Basket *b = *i;
		b->Main( *key,ball,rule );			//派生先のMainが呼ばれる(Picher用にKeyも渡す)
		if( b->getCatchFlag() )	catch_flag = true;		//もしカゴに入っていたらマネージャーのフラグを立てておく
		++i;
	}
}

//カゴをまとめるマネージャーのメイン
/*
IRender		*render	:レンダリングクラス
Matrix4x4	&vps	:カメラ行列
*/
void BasketManager::Draw( IRender *render,Matrix4x4	&vps )
{
	std::list<Basket *>::iterator i;
	i = basket.begin();							
	while(i != basket.end()){						
		Basket *p = *i;
		p->Draw(render, vps);							//派生先の描画
		++i;
	}
	render->setTexture(NULL);
}

//カゴをまとめるマネージャーの解放
void BasketManager::Dispose()
{
	std::list<Basket*>::iterator i;
	i = basket.begin();	
	while(i != basket.end()){
		delete (*i);
		i = basket.erase(i);
	}
}