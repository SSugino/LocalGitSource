/** @file
 * 野球盤ルールクラスの実装
 * @date 2009/10/15
 * @author 杉野
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

//初期化
void CamAngle::Init()
{
	//描画範囲を初期化
	right = Rect( 450,10,630,130 );						//画面作成
	left =	Rect( 10,10,190,130 );


	//ランナーを見るカメラの初期化
	cam1st.setPosition( Vector(0,8,-45) );				//1塁側カメラの位置
	cam1st.setAt( Vector(39,3,-40) );					//1塁側見る位置
	cam1st.setPercective( 1.3333f*13,1*15,10,100000 );	//1塁側パース

	cam3rd.setPosition( Vector(0,8,-40) );				//3塁側カメラ位置
	cam3rd.setAt( Vector(-44,3,-48) );					//3塁側見る位置
	cam3rd.setPercective( 1.3333f*13,1*15,10,100000 );	//3塁パース

	//スクリーン行列を作り直す
	cam1st.setScreen( right );
	cam3rd.setScreen( left );

	wait = 80;								//カメラ停止カウント
}

//カメラのメイン
/*
Camera3D		&cam	:カメラクラス参照
Ball			&_ball	:ボールクラス参照
Rule			&_rule	:ルールクラス参照
PlayerManager	&pm		:プレイヤーマネージャークラス参照
Key				&key	:キークラス参照
bool			mode	:エディットモードかどうかの参照
Rule::STATE		&p1		:ルールの状態
*/
void CamAngle::Main( Camera3D &cam, Ball &_ball, Rule &_rule, PlayerManager &pm,Key &key,bool mode,Rule::STATE &p1 )
{
	Aspect( cam,_ball,_rule,key,mode,p1 );
}

//メインカメラの移動やアングル(実質コチラがメイン）
/*
Camera3D	&cam	:カメラクラス参照
Ball		&_ball	:ボールクラス参照
Rule		&_rule	:ルールクラス参照
Key			&key	:キークラス参照
bool		mode	:エディットモードかどうかの参照p1
Rule::STATE &p1		:ルールの状態
*/			 
void CamAngle::Aspect( Camera3D &cam, Ball &_ball, Rule &_rule, Key &key,bool mode,Rule::STATE &p1 )
{
	//投球待ちの時 又は投球中
	if( mode==false )
	{
		if( _ball.getStateFlag()==0 || _ball.getStateFlag()==1 )				//ボールが投げられていない、または投げた状態
		{
			cam.setPercective( 1.3333f*6.2699f,1*6.2699f,6.2699f*1,10000 );　	//カメラのパースをセット
			cam.setAt( Vector(0,0.5f,180)  );									//見る箇所をセット
			cam.setPosition( Vector(0,9.600f,-180) );							//カメラの座標をセット
			wait = 80;															//待ち時間セット
		}
		//打ったとき
		else if( _ball.getStateFlag()== 2 )										//ボールをバットで打ち返したとき
		{
			Vector p = _ball.getPos();											//球の座標を取得
			Vector d = _ball.getDirectValue();									//方向を取得
			if( isRight(p) )													//センターより右方向なら
			{
				cam.setPercective( 1.3333f*5,1*5,5*1,10000 );					//この時のカメラのパースをセット
				cam.setAt( p );													//カメラの見る先はボールにセット
				cam.setPosition( Vector( -65,180,-160 ) );						//少し上から見る？
			}else{																//センターより左方向なら
				cam.setPercective( 1.3333f*5,1*5,5*1,10000 );	
				cam.setAt( p );
				cam.setPosition( Vector( 65,180,-160 ) );						//上から見る？
			}			
		}
		else if ( _ball.getStateFlag()==4 )										//ボール結果状態
		{
			//球発射以外のキーが押されたらカメラ演出スキップ
			if( key.isKeyData() && !key.isDown(Key::KEY_W) && !key.isDown(Key::AT_MARK) )
			{
				_ball.setStateFlag(0);											//ボールの状態を0に（なくす）
			}
			//res = 0:表示しない 1:ヒット 2:アウト 3:フォアボール 4:ファール 5:ホームラン
			int res = _rule.getResult();
			switch( res )
			{
			case( 1 ) :isHit(cam,_ball,_rule);break;			//ヒット時のカメラ演出
			case( 2 ) :isOut(cam,_ball,_rule);break;			//アウト時のカメラ演出
			case( 4 ) :isFoul(cam,_ball,_rule);break;			//ファール時のカメラ演出
			case( 5 ) :isHomeRun(cam,_ball,_rule);break;		//ホームラン時のカメラ演出
			};
		}
	}else if( mode==true )								//守備エディット中
	{
		//カメラ移動の最大
		const float y_max = 3;
		const float y_min = -2;
		const float x_max = -80;
		const float x_min =  70;
		
		Vector at = Vector(0,0,-40);						//ピッチャーのマウンド(出てない時のボール位置)を見る基準とする
		const float chei =400;							//カメラの高さ
		const float sp = 1.5f;							//カメラ移動スピード
		//見ている先の移動
		if( p1==Rule::DEFFENCE )						//プレイヤー１が守備なら
		{
			if( key.isDown( Key::KEY_S )){
				if( edit_cpos.y <= y_min ) edit_cpos.y = y_min;
				else edit_cpos -= cam.getUp_axis()*sp;				//カメラの↓移動
			}else if( key.isDown( Key::KEY_W )){
				if( edit_cpos.y >= y_max ) edit_cpos.y = y_max;
				else edit_cpos += cam.getUp_axis()*sp;				//カメラの↑移動
			}
			if( key.isDown( Key::KEY_D )){
				if( edit_cpos.x <= x_max ) edit_cpos.x = x_max;
				else edit_cpos += cam.getRight_axis()*sp;			//カメラの→移動
			}
			else if( key.isDown( Key::KEY_A )){
				if( edit_cpos.x >= x_min ) edit_cpos.x = x_min;
				else edit_cpos -= cam.getRight_axis()*sp;			//カメラの←移動
			}

		}else{
			if( key.isDown( Key::CONMA )){
				if( edit_cpos.y <= y_min ) edit_cpos.y = y_min;
				else edit_cpos -= cam.getUp_axis()*sp;				//カメラの↓移動
			}
			else if( key.isDown( Key::AT_MARK )){
				if( edit_cpos.y >= y_max ) edit_cpos.y = y_max;
				else edit_cpos += cam.getUp_axis()*sp;				//カメラの↑移動
			}
			if( key.isDown( Key::BRACE )){
				if( edit_cpos.x <= x_max ) edit_cpos.x = x_max;
				else edit_cpos += cam.getRight_axis()*sp;			//カメラの→移動
			}
			else if( key.isDown( Key::SEMICORON )){
				if( edit_cpos.x >= x_min ) edit_cpos.x = x_min;
				else edit_cpos -= cam.getRight_axis()*sp;			//カメラの←移動
			}
		}
		
		cam.setPercective( 1.3333f*23,1*23,23*1,10000 );			//パース設定しなおし
		cam.setAt( at+edit_cpos );									//見る箇所をピッチャーのほうへ設定しなおし
		cam.setPosition( edit_cpos + Vector(0,chei,0)*3 );			//カメラの座標を設定しなおし
	}	
}

//ランナー用の仮想画面描画
/*
*/
void CamAngle::Draw( IRender *render,PlayerManager &pm,Ground &g,Ball &_rule )
{
	PlayerBase *base[4]= {NULL};		//塁情報を配列で管理
	int j=0;							//配列要素用
	//全塁の情報を取得
	std::list<PlayerBase*>::iterator i;
	i = pm.getPlayer().begin();
	while(i != pm.getPlayer().end() )
	{
		base [j] = *i;					//配列で実体化
		++i;
		++j;
	}
	
	int r = _rule.getStateFlag();
	if( r==0 || r==1 )
	{
		//一塁がいたなら
		if( base[0]->getState() )
		{
			render->setViewport( right );	
			render->Clear(0);				
			render->ClearZ();				
			g.Draw( render,cam1st.getMatrix() );
			//新たな描画範囲に描画
			base[0]->Draw( render,cam1st.getMatrix() );
		}
		render->setViewport( Rect(0,0,640,480) );
		
		//三塁がいたなら
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


//ファールのときのカメラの演出
/*
Camera3D	&cam	:カメラクラス参照
Ball		&_ball	:ボールクラス参照
Rule		&_rule	:ルールクラス参照
*/
void CamAngle::isFoul( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;										//カメラから球へのベクトル
	float len = v.Length();										//カメラと球までの距離
	v.Normalize();
	float t	  =	len/50;											//距離の等分（1回分の距離)
	const float rot = 0.01f;									//回転量
	float hei = cpos.y - pos.y;

	if( isRight(pos) ) cpos.Rotate( cam.getUp_axis(),rot );		//右側なら左側にカメラを回す
	else cpos.Rotate( cam.getUp_axis(),-rot );
	cpos-=(v*t);												//カメラを近づける
	if( hei>2.5f ) cpos.y-=0.3f;								//カメラの高さも下げていく
	cam.setPosition( cpos );

	if( len<50.5f )
	{
		bool r  = _rule.ColResultAlpha( 0.08f );
		if( r ){
			if( wait )wait--;
			else _ball.setStateFlag(0);							//球を消す
		}
	}
}

//ヒットのとき（シングルも２BHもホームラン以外は含む )のカメラ演出
/*
Camera3D	&cam	:カメラクラス参照型
Ball		&_ball	:ボールクラス参照型
Rule		&_rule	:ルールクラス参照型
*/
void CamAngle::isHit()
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;							//カメラから球へのベクトル
	
	float len = v.Length();							//カメラと球までの距離
	v.Normalize();
	float t	  =	len/50;								//距離の等分（1回分の距離)
	float hei = cpos.y - pos.y;						//球とカメラの高低差
	if( hei>6.0f ) cpos.y-=1.0f;					//一定の高さになるまで下げていく
	if( len>80 )
	{
		cpos-=(v*t);								//カメラを近づける
		cam.setPosition( cpos );
	}else{
		bool r = _rule.ColResultAlpha( 0.08f );
		if( r )
		{
			if( wait )wait--;						//ウェイトカウント減らす
			else _ball.setStateFlag(0);				//球を消す	
		}
	}
}

//ホームランのときのカメラ演出
/*
Camera3D	&cam	:カメラクラス参照型
Ball		&_ball	:ボールクラス参照型
Rule		&_rule	:ルールクラス参照型
*/
void CamAngle::isHomeRun( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;							//カメラから球へのベクトル	
	float len = v.Length();							//カメラと球までの距離
	v.Normalize();
	float t	  =	len/50;								//距離の等分（1回分の距離)
	float hei = cpos.y - pos.y;						//球とカメラの高低差
	
	if( len>140 )
	{
		if( hei>6.0f ) cpos.y-=1.0f;				//一定の高さになるまで下げていく
		at = pos;
		cpos-=(v*t);								//カメラを近づける
		cam.setPosition( cpos );
	}else{
		if( at.y <= 50 )
		{	at.y += 0.5f;
			cam.setAt(  at  );
		}else{
			bool r;
			r = _rule.DrawMove();
			if( r )	_ball.setStateFlag(0);					//球を消す	
		}
	}
}

//アウトの時のカメラ演出
/*
Camera3D	&cam	:カメラクラス参照型
Ball		&_ball	:ボールクラス参照型
Rule		&_rule	:ルールクラス参照型
*/
void CamAngle::isOut( Camera3D &cam, Ball &_ball, Rule &_rule )
{
	Vector pos = _ball.getPos();
	Vector cpos= cam.getPosition();
	Vector v =  cpos - pos;										//カメラから球へのベクトル
	float len = v.Length();										//カメラと球までの距離
	v.Normalize();
	float t	  =	len/30;											//距離の等分（1回分の距離)
	const float rot = 0.05f;									//回転量
	float hei = cpos.y - pos.y;

	if( hei>30.5f ) cpos.y-=0.3f;
	if( len>120.5f )
	{
		cpos-=(v*t);											//カメラを近づける
	}else{
		if( isRight(pos) )cpos.Rotate( Vector(0,1,0) ,rot/(Math::PI*2) );	//右側なら左側にカメラを回す
		else cpos.Rotate( Vector(0,1,0) ,-rot/(Math::PI*2) );				//左側なら右側にカメラを回す
		bool r = _rule.ColResultAlpha( 0.08f );
		if( r )
		{
			if( wait )wait--;
			else  _ball.setStateFlag(0);							//球を消す	
		}
	}
	cam.setPosition( cpos );
}

//球の位置を判定
/*
Vector3 &pos	:玉の座標
*/
bool CamAngle::isRight( Vector3 &pos )
{
	Vector v;				//90度ベクトル
	v = Vector3::Cross( Vector(0,1,0),Vector(0,0,1) ).Normalize();
	return Vector3::Dot( v,pos - Vector(0,0,0) ) > 0;		//センターより右か左か
}

//範囲内判定
/*
Vector3 &pos	:ボールの座標
float 　length	:範囲（度数指定）
*/
bool CamAngle::Inside( Vector3 &pos,float length )
{
	Vector v;
	v = Vector3::Cross(Vector(0,1,0),v );
	float cos2 = v.Length();
	if( cos2 < (Math::cos(length*(Math::PI/180))) ) return false;
	return true;
}