/** @file
 * ゲームの遊ぶ部分を統括するクラスの定義
 * @date 2009/08/27
 * @author 杉野
 **/
#pragma once

#include	"../game/Camera3D.hpp"		//カメラクラス
#include	"../game/Scene.h"			//シーン基底クラス
#include	"../math/Matrix.h"			//カメラ行列他
#include	"../graphics/IRender.h"		//レンダリング
#include	"../input/Input.h"			//入力
#include	"../game/Object3D.h"		//３ＤModelクラス
#include	"Stadium.h"					//スタジアム
#include	"Player.h"					//選手（ランナー、守備
#include	"Bat.h"						//バット
#include	"Ball.h"					//ボール
#include	"Rule.h"					//ルール
#include	"../game/BoundingObject.h"	//判定
#include	"../editer/Edit.h"			//守備エディター
#include	"../thread/Thread.h"		//スレッド
#include	"CameraAngle.h"				//カメラのアングル

namespace sugino{
/** 野球盤の領域 */
namespace baseball{
	/** 野球盤ゲームのクラス */
	class BaseBall : public game::SceneBase,thread::IThreadRunnable
	{
	private:
			/** 背景を保持 */
			system::AutoPtr<graphics::IImage>	bg;				//背景テクスチャ
			system::AutoPtr<graphics::IImage>	b;				//仮想画面テスト

			//ロード時に必要なもの
			system::AutoPtr<graphics::IImage>	load_bg;		//ローディング中背景テクスチャ
			system::AutoPtr<graphics::IImage>	load_f;			//ローディング中文字テクスチャ
			system::AutoPtr<graphics::IImage>	img_Helpkey;			//キーヘルプテクスチャ
			
			system::AutoPtr<graphics::IImage>	img_attacker;



			DWORD								load_al;		
			int									load_wait;		//100％になったとき少し余裕を持たす
			void CalAlpha();
			int									load_cnt;
			int									draw_cnt;		//表示しているカウント	
			
			/** スレッド */
			thread::Thread	thre;

			/** カメラ */
			game::Camera3D	camera;

			/** グラウンドクラスで管理 */
			Ground			ground;
			
			/** ルールクラスで管理 */
			Rule			rule;

			/** ボールクラスで管理 */
			Ball			ball;

			/** プレイヤー（選手）クラスで管理 */
			PlayerManager	playerM;
			
			/**　守備のカゴクラスで管理 */
			BasketManager	basketM;

			/** スタジアム（球場）クラスで管理 */
			StadiumManager	stadiumM;
						
			/** スカイドームクラスで管理 */
			SkyDome			sky;

			/** バットクラスで管理 */
			Bat				bat;
			
			/** 守備エディタークラスで管理 */
			editer::Editer	edit;

			/** カメラの視点 */
			CamAngle		angle;

			/** モード
			 * @note	false:通常モード	true:エディット中
			 **/
			bool			edit_mode;
			
			/** スレッド終了フラグ
			 * @note	false:シーン自体がデストラクタに入ってない true:入っている
			 **/
			bool			exit;

			/** エディット時にカメラの見てる先 */
			float			at_pos_x;
			float			at_pos_z;
			
			/** 先攻後攻を保持 */
			Rule::SIDE		player1;
			Rule::SIDE		player2;

			/** 攻撃中か守備中かの状態を保持 */
			Rule::STATE		p1_state;
			Rule::STATE		p2_state;

			/** シーン管理の状態
			 * @note RuleクラスのGameSet関数戻り値からシーンを管理する
			 *
			 **/
			 int			scene_flag;

			/** BGM */
			int play_bgm;
			int play_bgm2;
			bool play_bgm_f;

	public:
			/** コンストラクタ
			 * @param [out] IRender *r		:mainでインスタンス化されたレンダー
			 * @param [out] Key		*k		:mainでインスタンス化されたキー
			 * @param [out] Mouse	*m		:mainでインスタンス化されたマウス
			 * @param [in]	int		wid		:画面幅
			 * @param [in]	int		hei		:画面高さ
			 **/
			BaseBall( graphics::IRender *r, input::Key *k, input::Mouse *m,int wid,int hei ) : SceneBase( r,k,m )
			{
				//カメラの初期化
				camera.Initialize( math::Vector3( 0,1,2 ),math::Vector3(0,0,1),wid,hei,(float)wid,(float)hei,2,10000 );
				edit_mode	= false;
				at_pos_x	= 0.0000001f;
				at_pos_z	= 0.0000001f;
				//先攻後攻の初期化
				
				//本来の物
				player1 = Rule::IN_SIDE;			//表、先攻
				p1_state= Rule::DEFFENCE;				//player1を攻撃状態に
				player2 = Rule::OUT_SIDE;			//裏、後攻
				p2_state= Rule::DEFFENCE;			//player2を守備状態に
				/*
				player1 = Rule::OUT_SIDE;			//表、先攻
				p1_state= Rule::ATTACK;				//player1を攻撃状態に
				player2 = Rule::IN_SIDE;			//裏、後攻
				p2_state= Rule::DEFFENCE;			//player2を守備状態に
				*/

				scene_flag = 0;

				b =NULL;
				exit		= false;
				load_cnt = draw_cnt = 0;
				load_wait=50;
				bg	= r->createImage( "data/graphic/sky2.png" );
				
			}

			//デストラクタ
			~BaseBall()
			{
				exit = true;
				if( thre.isActive() ) thre.Wait();
				void Dispose();
			}

			/** 初期化 */
			void Init();

			/** メイン処理 */
			void Main();
			
			/** 画面表示 */
			void Draw();
			
			/** 表裏（先攻後攻）の取得 */
			Rule::SIDE getSidePlayer1(){ return player1; }
			Rule::SIDE getSidePlayer2(){ return player2; }

			/** 表裏（先攻後攻）のセット */
			void setSidePlayer1( Rule::SIDE _side ){ player1 = _side; }
			void setSidePlayer2( Rule::SIDE _side ){ player2 = _side; }

			/** 攻守状態の取得 */
			Rule::STATE getStatePlayer1(){ return p1_state; }
			Rule::STATE getStatePlayer2(){ return p2_state; }

			/** 攻守状態のセット */
			void setStatePlayer1( Rule::STATE _state ){ p1_state = _state; }
			void setStatePlayer2( Rule::STATE _state ){ p2_state = _state; }

			/** 攻守状態の全セット
			 * @param [in]	STATE	p1state	:player1の攻守の状態
			 * @param [in]	STATE	p2state	:player2の攻守の状態
			 * @note p1が攻撃ならば p2は守備でなければならない
			 **/
			void setPlayerAll( Rule::STATE p1state,Rule::STATE p2state )
			{
				if( p1state == p2state ) throw system::Exception( "攻守の状態が同じになってます" );
				setStatePlayer1( p1state );
				setStatePlayer2( p2state );
			}

			/** スレッドメイン */
			virtual DWORD Run();
			
			/** 解放
			 * @note スレッドでロードされたものの解放
			 **/
			void Dispose();

			//カメラ操作で使用
			/** ボールの位置判定
			 * @return true:右方向 :false:左方向
			 **/
			bool isRight( math::Vector3 &pos ); 

			/** ボールの範囲内判定
			 * @param [in] Vector3	pos		:座標
			 * @param [in] float	length	:範囲（度数指定）
			 * @return
			 **/
			bool Inside( math::Vector3 &pos,float length );
			
			/** 試合終了 */
			void GameSet();

			/** ヘルプの表示 */
			void DrawHelp( graphics::IRender *render,Rule::STATE p1state,Ball *ball,Bat *bat );
	};

}}