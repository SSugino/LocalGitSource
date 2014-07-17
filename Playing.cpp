/** @file
 * ゲームの遊ぶ部分を統括する部分クラスの実装
 * @date 2009/08/27
 * @author 杉野
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
	MOUND,			//マウンド上でまだ投球してない
	THROW,			//投げてキャッチャーに向かってる最中
	CATCH,			//キャッチャー取った
	BATTING,		//打ち返した
	RESULT,			//結果
};

BALL_STATE	ball_state; 

void BallStateBegin();
void BallStateEnd();

//スレッドメイン
DWORD BaseBall::Run()
{
	//セットしたモデルやテクスチャを使って初期化
	stadiumM.Init();
	playerM.Init();
	basketM.Init();

	//カメラの初期化
	camera.setScreen( 640,480 );								//画面サイズ
	camera.setPercective( 1.3333f*30,1*30,60*1,10000 );			//パース
	camera.setLookAt( Vector(0,0,-35)*30 ,ground.getPos(),Vector(0,1,0) );	//見ている場所と見ている物
	
	//ランナーのカメラの初期化
	angle.Init();

	return 0;
}

//ゲーム中の色計算
void BaseBall::CalAlpha()
{
	Color col( load_al );
	col.a+=0.005f;
	if( col.a>=1.0f ) col.a = 1.0f;	
	load_al = col;
}

/** 初期化 */
void BaseBall::Init()
{
	//スレッドで使う用の初期化
	load_bg = render->createImage( "data/graphic/loadingbg.png" );			//ローディング中のテクスチャ
	load_f	= render->createImage( "data/graphic/nowload.png" );			//ローディング中のテクスチャ
	load_al = 0x3FFFFFFF;
	load_wait =50;
	


	img_Helpkey = render->createImage( "data/graphic/Helpkey.png" );
	
	img_attacker = render->createImage( "data/graphic/attacker.png" );


	/** 各モデルのロード/初期化 */
	//グラウンド
	ground.Init( "data/model/RDX/new_map.RDX",render->createImage("data/graphic/ground.png"),
													render->createImage("data/graphic/wall.png") );
	Sleep( 10 );
	//バット
	bat.Init("data/model/RDX/bat.RDX",render->createImage("data/graphic/bat.png"));	
	Sleep( 30 );
	//スカイドーム
	sky.Init("data/model/RDX/dome.RDX",render->createImage("data/graphic/sky3.png") );
	Sleep( 200 );	
	load_cnt = 40;

	//ルール
	rule.Init( render->createImage("data/graphic/count.png"),
			   render->createImage("data/graphic/result.png"),
			   render->createImage("data/graphic/score.png"),
			   render->createImage("data/graphic/cntside.png"),
			   render->createImage("data/graphic/change.png")
			   );
	Sleep( 10 );
	//マネージャーの管理している一括でセットする
	//ボールの初期化
	ball.Init( "data/model/RDX/ball.RDX",render->createImage("data/graphic/ball.png") );
	Sleep( 100 );
	//ランナーの板・選手の初期化
	playerM.setImage( render,"data/graphic/putup01.png","data/graphic/putup02.png","data/graphic/putup01.png","data/graphic/te.png","data/graphic/minirun.png");
	Sleep( 100 );
	playerM.setModel( "data/model/RDX/putup.RDX","data/model/RDX/pitch.RDX" );
	Sleep( 100 );
	load_cnt = 70;

	//カゴ初期化
	basketM.setImage( render,"data/graphic/basket.png" );
	Sleep( 200 );
	basketM.setModel( "data/model/RDX/basket.RDX" );
	Sleep( 100 );
	//ベース・観客席の初期化
	stadiumM.setImage(  render,"data/graphic/base.png","data/graphic/auditorium.png");
	Sleep( 10 );
	stadiumM.setModel( "data/model/RDX/base.RDX","data/model/RDX/auditorium.RDX" );
	Sleep( 10 );
	load_cnt = 90;
	//エディット処理の初期化
	edit.Init( "data/model/RDX/range.RDX",
				render->createImage("data/graphic/range.png"),
				render->createImage("data/graphic/wait.png"),
				render->createImage("data/graphic/img_mouse.png")
				);	
	load_cnt = 200;
	
	play_bgm = LoadSoundMem( "data/bgm/play.mp3" );
	play_bgm2 = LoadSoundMem( "data/bgm/baseball_stadium2.mp3" );
	play_bgm_f = false;
	
	//スレッド作成、開始
	thre.Create( this );
	thre.Start();
}

/** メイン処理 */
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
		//ゲームセットでない
		else
		{


			//カメラの視点
			angle.Main( camera, ball,rule,playerM,*key,edit_mode,p1_state );

			const float  ss = 3;
			sky.Main();						//空を回転させる
			

			////////////////////////////////////
			//マウス座標変換
			// 2D→3D
			mouse->TransformPosition3D( camera.getMatrix(),camera.getPosition() );
			
			//モード切替
			if( p1_state==Rule::DEFFENCE )						//プレイヤー1が守備の時
			{
				//コントロールキー。モード切替。ボールが出現してない時のみ可能
				if( key->isOn( Key::LCONTROL ) && ball.getStateFlag()==0 ) edit_mode^=1;
			}else{												//プレイヤー2が守備の時
				if( key->isOn( Key::RCONTROL ) && ball.getStateFlag()==0 ) edit_mode^=1;
			}
			//守備のカゴを管理しているマネージャーのメイン処理
			basketM.Main( key,&rule,&ground,&ball );
			
			if( edit_mode==false )				//エディットモードでない
			{
				edit.setMode( false );						//ルールのモードも関連付け
				ground.Main( &ball,&bat,&rule );	//グラウンドのメイン　ストライク・ボールの判定処理
				ball.Main( &ground,&rule,key,p1_state );			//ボールのメイン処理
				bat.Main( key,&ball,&rule,p1_state);			//バットクラスのメイン処理
				//プレイヤー（選手)クラスを管理しているマネージャーのメイン処理
				playerM.Main( key,&ground,&ball,&basketM,&rule,p1_state );		
				//スタジアム（球場）クラスを管理しているマネージャーのメイン処理
				stadiumM.Main( &rule,&ground,&ball );		//派生先のメイン・グラウンドクラスを渡す	
				//ルールのメイン処理
				rule.Main( &ball,&ground,basketM,playerM,bat,&p1_state,&p2_state,&scene_flag );
			}else
			{
				//守備エディタークラス
				edit.Main( *mouse,camera,basketM,edit_mode );
			}
		}
	}
}

/** 表示 */
void BaseBall::Draw()
{
	render->setViewport( Rect(0,0,640,480 ) );

	if( draw_cnt<200 )
	{
		int cnt = draw_cnt;
		if( cnt>100 ) cnt = 100;
		render->drawImage( Rect(0,0,640,480),load_bg,Rect(0,0,512,512) );	//ローディング中テクスチャ
		render->drawImage( Rect(80,130,80+512,130+64),load_f,Rect(0,0,512,64),load_al );	//ローディング中テクスチャ
		render->drawFill( Rect(300,450,300+cnt*3,470 ),0xAf0000FF );
		render->drawBox( Rect(299,449,301+cnt*3,471 ),0x4fFFFF00 );
		for(int i=0;i<3;i++)render->drawBox( Rect(298-i,448-i,602+i,472+i ),0xffF0F0FF );		
	}else{
		//一枚絵を一番後ろに表示したいので
		render->setZBufferMode( render->ZM_OFF );
		render->drawImage( Rect(0,0,640,480),bg,Rect(0,0,512,512) );	//ローディング中テクスチャ
		render->setZBufferMode( render->ZM_ON_WRITE );

		sky.Draw( render,camera.getMatrix() );					//スカイドーム
		stadiumM.Draw( render,camera.getMatrix() );				//スタジアム（球場）マネージャー
		ground.Draw( render,camera.getMatrix() );				//グラウンドモデル

		basketM.Draw( render,camera.getMatrix() );				//カゴ（守備）マネージャー
		ball.Draw( render,camera.getMatrix() );				//ボール
		playerM.Draw( render,camera,bat );				//プレイヤー（選手）マネージャー
		rule.Draw( render,camera.getMatrix(),bat );				//ルール	元の場所
		bat.Draw( render,camera.getMatrix() );				//バットモデル
		angle.Draw( render,playerM,ground,ball );				//ランナー用カメラ
		edit.Draw( render,camera.getMatrix(),*mouse,basketM );	//エディットマネージャー

		DrawHelp( render,p1_state,&ball,&bat );
	}
}

//ヘルプの表示
/*
IRender		*render		:レンダリングクラス
Rule::STATE p1state		:プレイヤーの情報
Ball		*ball		:ボールクラス
Bat			*bat		:バットクラス
*/
void BaseBall::DrawHelp(IRender *render,Rule::STATE p1state,Ball *ball,Bat *bat )
{
	//常にヘルプキー表示
	if( ball->getStateFlag() == 2 || ball->getStateFlag()== 3 ) return;	//打ち返したときやヒットのカメラ演出の時は処理に入らない
	if( edit_mode ) return;			//エディットモード時は入らない
	
	const int s = 32;	//画像サイズ
	//投球してない時
	if( ball->getStateFlag() ==0 )
	{
	if( p1state == Rule::STATE::ATTACK )	//プレイヤー１P攻撃側
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

			//バッター側
			if( bat->getBatter() == Bat::BATTER_TYPE::LEFT )
			{
				render->drawImage( 60+s,410,32,32,img_Helpkey,352,32 );		// X
				render->drawImage( 60+s*2,410,32,32,img_Helpkey,352+s,32 );		// C
			}else{
				render->drawImage( 460,410,32,32,img_Helpkey,320,32 );			//Z
				render->drawImage( 460+s,410,32,32,img_Helpkey,352+s,32 );		// C
			}


		}else if( p1state == Rule::STATE::DEFFENCE )									//プレイヤー１P守備
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


			//バッター側
			if( bat->getBatter() == Bat::BATTER_TYPE::LEFT )
			{
				render->drawImage( 60+s,410,32,32,img_Helpkey,286+s,64 );		// /
				render->drawImage( 60+s*2,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}else{
				render->drawImage( 460,410,32,32,img_Helpkey,286,64 );			//.
				render->drawImage( 460+s,410,32,32,img_Helpkey,286+s*2,64 );		// _
			}

		}
	}else if( ball->getStateFlag() == 1 )		//投球時　変化球とかのキー表示
	{
		if( p1state == Rule::STATE::ATTACK )	//プレイヤー攻撃側
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
			
			//バッター側
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



//ゲーム中の解放
void BaseBall::Dispose()
{
	bg = NULL;
	load_bg =NULL;
	load_f =NULL;
}