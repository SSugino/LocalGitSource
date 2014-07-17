/** @file
 * プレイヤー（選手）クラスの定義
 * @date 2009/09/04
 * @author 杉野
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Object2D.h"
#include "../game/Collision.h"
#include "../input/Input.h"

#include "Rule.h"

namespace sugino{
namespace baseball{

	/**  仮定義 */
	class Rule;
	class Ball;
	class Ground;

	/** プレイヤー(選手)の基底クラス */
	class PlayerBase
	{
	protected:
	
		/** 座標 */
		math::Vector3		pos;
		
		/** スケーリング */
		math::Vector3		scale;

		/** 回転 */
		float				rot;

		/** 回転量 */
		float				rot_val;

		/** モデル */
		game::Model3D		*model;

		/** マネージャー管理のテクスチャイメージを参照 */
		graphics::IImage	*img;
				
		/** 出現状態
		 * @note true:出現中	false:出現してない
		 **/
		bool state;	

	public:
		/** コンストラクタ
		 * @pragma [in] _pos		: 座標
		 * @pragma [in] _scale		: 縮尺
		 * @pragma [in] _rot		: 回転
		 * @param　[in] _rot_val	: 回転量 
		 * @pragma [in] _model		: モデルデータ
		 * @pragma [in] _img		: テクスチャデータ
		 **/
		PlayerBase( math::Vector3 &_pos, math::Vector3 &_scale, float _rot,float _rot_val, game::Model3D *_model, graphics::IImage *_img, bool _state )
		{
			pos		= _pos;
			scale	= _scale;
			rot		= _rot;
			rot_val	= _rot_val;
			model	= _model;
			img		= _img;
			state	= _state;
		}

		/** デストラクタ */
		virtual ~PlayerBase(){}

		/** 初期化 */
		virtual void Init(){};			 

		/** メイン処理
		 * @param [out] Key			&key	:メインのキークラス
		 * @param [out] Ball		*_ball	:ボールクラス
		 **/
		virtual void Main( input::Key &key,Ball *_ball,
							Rule *rule,Rule::STATE &p1state ){};		//派生先で書き換えたいので仮想で用意
		
		/** 表示
		 * @param [out] IRender		*r	:レンダー
		 * @param [out]	Matrix4x4	&vps:カメラ行列
		 **/
		virtual void Draw( graphics::IRender *r,math::Matrix &vps )=0;
		
		//--------- Get ------------
		/** 座標の取得
		 * @return	pos					:3次元座標
		 **/
		math::Vector3	getPos(){ return pos; }
		
		/** スケールの取得
		 * @return scale				:3次元スケール
		 **/
		math::Vector3	getScale(){ return scale; }
		
		/** 状態の取得 
		 * @return	state				:出現状態
		 **/
		bool			getState(){ return state; }

		/** 回転の取得
		 * @return	rot					:角度
		 **/
		float			getRot(){ return rot; }

		/** 回転量の取得
		 * @return rot_val				:回転量
		 **/
		float			getRotVal(){ return rot_val; }

		//--------- Set ------------
		/** 座標をセットする
		 * @param [in] Vector3	_pos	:3次元座標
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** スケーリングをセットする
		 * @param [in] Vector3	_scale	:3次元スケール
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** 回転をセットする
		 * @param [in] float	_rot	:回転度数
		 **/
		void setRot( float _rot){ rot = _rot; }
		
		/** 回転量をセットする
		 * @param [in] float	_rot_val:回転量
		 **/
		void setRotaVal( float _rot_val ){ rot = _rot_val; }
		
		/** 出現状態のセット
		 * @param	bool	state		:出現状態
		 **/
		void setState( bool _state ){ state = _state; }

		/** 判定を持たす
		 * @param [out] Ball *bbase		:ボールクラス 
		 **/
		virtual void Collision( Ball *bbase ){};

		/** 速度の変更 */
		virtual void setSpeed( float sp ) =0;

	};	

	/** プレイヤー(選手）管理クラス */
	class PlayerManager{
	private:
		/** 基底クラスを保持 */
		std::list<PlayerBase *>	player;			

		/** ランナーのモデル */
		game::Model3D			mdl_runner;

		/** ピッチャーのモデル */
		game::Model3D			mdl_picher;

		/** 派生先の画像をここで管理 */
		system::AutoPtr<graphics::IImage>	img_runner1;
		system::AutoPtr<graphics::IImage>	img_runner2;
		system::AutoPtr<graphics::IImage>	img_runner3;
		system::AutoPtr<graphics::IImage>	img_pich;
		system::AutoPtr<graphics::IImage>	img_minirun;
		
		/** Se */
		int se_home_run;
		int se_hakushu;

		/** 状態フラグの立っている個数
		 * @note	ランナー人数
		 **/
		int						runner_cnt;

		/** シングルヒット時ランナーを進める処理
		 * @param [out]	Ball	*_ball	:ボールクラス
		 **/
		void					SingleHitGoRunner( Ball *_ball,Rule *rule );
		
		/** ツーベースヒット時ランナーを進める処理
		 * @param [out]	Ball	*_ball	:ボールクラス
		 **/
		void					TwoBaseHitGoRunner( Ball *_ball,Rule *rule );
		
		/** スリーベースヒット時ランナーを進める処理
		 * @param [out]	Ball	*ball	:ボールクラス
		 **/
		void					ThreeBaseHitGoRunner( Ball *_ball,Rule *rule );	
		
		/** ホームラン時のランナーを進める処理
		 * @param [out]	Ball	*_ball	:ボールクラス
		 **/
		void					HomeRunGoRunner( Ball *_ball,Rule *rule );	
		
		/** ミニランナーのパターン番号 */
		int						mini_pat;
		
		/** ランナー情報のパターン */
		void					CalPattern();

		/** 色の合成分解、計算 */
		void					CalColor();
		
		/** アルファ値 */
		DWORD					alpha;
		
		/* アルファ値加算減算フラグ */
		bool					alpha_flag;
		
		/** 押し出し */
		void					PushRunner();

	public:
		/** コンストラクタ */
		PlayerManager(){
			img_runner1	= NULL;
			img_runner2 = NULL;
			img_runner3 = NULL;
			img_pich	= NULL;
			img_minirun	= NULL;
			
			runner_cnt	= 0;
			mini_pat	= 0;
			alpha		= 0xBBFFFFFF; 
			alpha_flag	= false;
		}
		
		/** デストラクタ/解放 */
		~PlayerManager(){ Dispose(); }
		
		/** Baseを必要分初期化
		 * @note new-sでPlayerBase派生型を初期化していく
		 **/
		void Init();
		
		/** メイン
		 * @param [out]	Key				*key	:メインのキークラス
		 * @param [in]	Ground			*g		:グラウンドクラス
		 * @param [out]	Ball			*_ball	:ボールのクラス
		 * @param [out]	BasketManager	*baskM	:カゴのマネージャークラス
		 * @param [out] Rule			*rule	:ルールクラス
		 **/
		void Main( input::Key *key,baseball::Ground *g,Ball *_ball,BasketManager *baskM,
			 baseball::Rule *rule,Rule::STATE &p1state );		//地面や壁との判定を持たせるために引数に入れる

		/** 表示
		 * @param [out] IRender		*render	:レンダー
		 * @param [out] Matrix4x4	&vps	:カメラ行列
		 **/
		void Draw( graphics::IRender *render, game::Camera3D &cam,Bat &_bat );
	
		/** 移動 */
		void Move();
		
		/** リストの解放処理 
		 * @note リストで並んだPlayerBaseの解放
		 **/
		void Dispose();

		//--------- Get ------------
		/** プレイヤーリストの取得
		 * @return player		:基底クラスのリスト
		 **/
		std::list< PlayerBase *> &getPlayer(){ return player; }

		/** 派生先のデータを取得
		 * @param [in]	int here	:欲しいリストデータまでのループ回数
		 * @return	プレイヤーの基底クラス
		 **/
		PlayerBase *getPlayerBase( int here ); 

		/** 派生先の状態データを取得 */
		void SarchPlayerState();

		/** 指定の派生先データの状態をセットする
		 * @param [in]	int		num		:欲しいリストデータまでのループ回数
		 * @param [in]	bool	_state	:新たにセットするフラグ
		 *	@return	true:セット成功	false:失敗
		 **/
		bool setPlayerState( unsigned int num, bool _state ); 

		//--------- Set ------------
		/** プレイヤー基底クラスをリストへ追加
		 * @note "一体分"をリストへ追加
		 **/
		void setPlayer( PlayerBase *play ){ player.push_back(play); }
		
		/** モデルのセット（外部読み込み)
		 * @param [in] char		*model_name		:モデルの名前
		 * @note "PlayerManagerメンバ"「mdl_runner」のLoad関数を呼ぶ
		 **/
		void setRunnerModel( char *model_name ){ mdl_runner.Load(model_name); }

		/** テクスチャの一括セット
		 * @param [out] IRender	*render		:レンダー
		 * @param [in]	char *runner_img1	:ランナーのテクスチャ1
		 * @param [in]	char *runner_img1	:ランナーのテクスチャ2
		 * @param [in]	char *pich_img	:ピッチャーのテクスチャ1
		 **/
		void setImage( graphics::IRender *render,char *runner_img1,char *runner_img2,char *runner_img3,char *pich_img,char *mini_img  )
		{
			img_runner1	= render->createImage( runner_img1 );
			img_runner2 = render->createImage( runner_img2 );
			img_runner3 = render->createImage( runner_img3 );
			img_pich	= render->createImage( pich_img );
			img_minirun = render->createImage( mini_img );
		}

		/** モデルの一括セット */
		void setModel( char *runner_model,char *picher_model )
		{
			mdl_runner.Load( runner_model );
			mdl_picher.Load( picher_model );
		} 
		
		/** ランナーの人数の取得
		 * @return state_cnt	:基底のstateフラグの立ってる数
		 **/
		int		getRunnerCnt(){ return runner_cnt; }

		/** 3アウトの時にランナーを全て消す処理 */
		void					DeleteRunner();


	};
	//---------------------------------------------------------------------
	
	/** ランナー用クラス(PlayerBase派生) */
	class Runner : public PlayerBase 
	{
	private:
		
		/** 行列変換データを保持 */
		math::Matrix4x4						mat_run,mat_r,mat_sc,mat_t,mat_init;
		
		/** 重力値を保持 */
		float								gravity;

		//行列の計算
		void calMatrix();

		/** 判定を保持 */
		game::BoundingOBox3D				bobox;

	public:

		/** 初期化 **/
		Runner( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _rot_val,game::Model3D *_model, graphics::IImage *_img,float _gravity,  bool state ) :
		PlayerBase( _pos,_scale,_rot,_rot_val,_model,_img,state )
		{
			gravity = _gravity;
			//各厚みを変え、引数で受け取ったrotの傾きで判定のOBoxを変える
			mat_r.RotateY( _rot );
			bobox=game::BoundingOBox3D( _pos,math::Vector(1.5f,_scale.y*2.5f,0.25f),mat_r );
	   };
		
		void Init()
		{
			calMatrix();
		}

		/** ランナーに関するメイン
		 * @param [out]	Key			&key	: メインのキークラス
		 * @param [out] Ball		*_base	: ボールクラス
		 * @param [out] Rule		*rule	:ルールクラス
		 * @param [out]	STATE		*p1state:プレイヤー1の攻守状態
		 **/
		void Main( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );

		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** 当たり判定
		 * @param [out]	Ball		*_ball	:ボールクラス
		 * @param [out]	Rule		*rule	:ルールクラス
		 **/
		virtual void Collision( Ball *_ball,Rule *rule );

		/** 出現状態の取得 */
		bool getState(){ return state; }

		/** 出現状態のセット
		 * @param [in]	bool	state		:出現状態
		 * @note	true:出現中		false:出現させない
		 **/
		void setState( bool _state ){ state = _state; }

		/** 速度変更 */
		void setSpeed( float sp ){}
	};
	
	/** ピッチャーの弾放出台用クラス */
	class Picher : public PlayerBase
	{
	private:
		/** 行列変換データを保持 */
		math::Matrix4x4						mat_pich,mat_r,mat_sc,mat_t,mat_init;
	
		/** 回転量を保持 */
		float								rot_val;		
		/** 回転 */
		float								rotate;
		/** 回転カウント */
		int									rot_cnt;
		/** 弾を放つカウント */
		int									rot_save;	
		/** 回転スタートフラグ */
		bool								rot_flag;
		/** 逆回転させるフラグ */
		bool								rot_reverse;

		/** 重力値を保持 */
		float								gravity;
	
		/** 行列の計算 */
		void calMatrix();

		/** 接触判定を保持 */
		game::BoundingBox3D					bobox;

		/** 放出時のスピード */
		float								speed;
		
		/** 球の発射
		 * @param [out]	Key			&key	: メインのキークラス
		 * @param [out] Ball		*_ball	: ボールのクラス
		 * @param [out] Rule		*rule	: ルールクラス		 
		 **/
		void	Shot( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );
		
		/** 投げるモーションの回転
		 * @param [in] ball *_ball	:ボールクラス
		 **/
		void HandMotion( Ball *_ball );

	public:
		/** 初期化 **/
		Picher( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _rot_val,game::Model3D *_model, graphics::IImage *_img,float _gravity, bool state ) :
		PlayerBase( _pos,_scale,_rot,_rot_val,_model,_img,state )
		{
			gravity = _gravity;
			speed = 1.2f;
			//判定の初期化
			bobox.p	= _pos;
			bobox.s  = math::Vector3( _scale.x*2.5f,_scale.y*5,_scale.z*2.5f );
		
			rot_val=0;		
			rotate=0;
			rot_cnt=59;
			rot_save = rot_cnt/2;			
			rot_flag=false;
			rot_reverse=false;

		};
		
		/** 初期化 */
		void Init()
		{
			calMatrix();
		}

		/** ピッチャーに関するメイン
		 * @param [out]	Key			&key	: メインのキークラス
		 * @param [out] Ball		*_ball	: ボールクラス
		 * @param [out] Rule		*rule	:ルールクラス
		 **/
		void Main( input::Key &key,Ball *_ball,Rule	*rule,Rule::STATE &p1state );
	
		/** 表示
		 * @param [in] IRender *render : IRenderクラスを派生したクラス
		 * @param [in] Matrix4x4 &vps  : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		//----- set get -------
		/** 座標の取得
		 * @return	pos					:3次元座標
		 **/
		math::Vector3 getPos(){ return pos; }
		
		/** 座標のセット 
		 * @param [in]	Vector3	_pos	:3次元座標
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** 回転量の取得
		 * @return	rot					:回転量
		 **/
		float getRot(){ return rot; }

		/** 回転量のセット
		 * @param [in]	float _rot		:回転量
		 **/
		void setRot( float _rot ){ rot = _rot; }

		/** 接触判定
		 * @param [out]	Ball	*bbase	:ボールクラス
		 **/
		virtual void Collision( Ball *bbase );

		/** 速度変更 */
		void setSpeed( float sp ){ speed =  sp; }

	};


	//----------------------------------------------------------------------------------------------------
	/** 守備のカゴを管理するクラス */
	class BasketManager
	{
	private:
		/** 基底クラスを保持 */
		std::list<Basket *>	basket;			

		/** "守備のカゴ"のモデル */
		game::Model3D			mdl_basket;

		/** 派生先の画像をここで管理 */
		system::AutoPtr<graphics::IImage>	img_basket;
		
		/** 基底のうちどれかがキャッチしてた */
		bool								catch_flag;
	public:
		/** コンストラクタ */
		BasketManager(){
			img_basket	= NULL;
			catch_flag	= false; 
		}
		
		/** デストラクタ/解放 */
		~BasketManager(){ Dispose(); }
		
		/** Baseを必要分初期化
		 * @note new-sでBasketBase派生型を初期化していく
		 **/
		void Init();
		
		/** メイン
		 * @param [out]	Key			*key	:メインのキークラス
		 * @param [out] Rule		*rule	:ルールクラス
		 * @param [in]	Ground		*g		:グラウンドクラス
		 * @param [out]	Ball		*ball	:ボールのマネージャークラス
		 **/
		void Main(  input::Key *key,baseball::Rule *rule,baseball::Ground *g,Ball *_ball );

		/** 表示
		 * @param [out] IRender		*render	:レンダー
		 * @param [out] Matrix4x4	&vps	:カメラ行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** リストの解放処理 
		 * @note リストで並んだBasketBaseの解放
		 **/
		void Dispose();

		//--------- Get ------------
		/** カゴリストの取得
		 * @return Basket		:基底クラスのリスト
		 **/
		std::list< Basket *> &getBasket(){ return basket; }
		
		//--------- Set ------------
		/** プレイヤー基底クラスをリストへ追加
		 * @note "一体分"をリストへ追加
		 **/
		void setBasket( Basket *bask ){ basket.push_back(bask); }		

		/** モデルのセット
		 * @param [in] char		*model_name		:モデルの名前
		 * @note "BasketManagerメンバ"「mdl_basket」のLoad関数を呼ぶ
		 **/
		void setModel( char *model_name ){ mdl_basket.Load(model_name); }

		/** テクスチャの一括セット
		 * @param [out] IRender	*render		:レンダー
		 * @param [in]	char *basket_img	:カゴのテクスチャ
		 **/
		void setImage( graphics::IRender *render,char *basket_img )
		{
			img_basket	= render->createImage( basket_img );
		}
		
		/** 基底がキャッチしたかどうかのフラグの取得 */
		bool getCatchFlag(){ return catch_flag; }

		/**  基底がキャッチしたかどうかのフラグのセット */
		void setCatchFlag( bool flag ) { catch_flag = flag; }

	};


	/** 守備側のカゴ用クラス */
	class Basket
	{		
	private:
		/** 座標 */
		math::Vector3		pos;
		
		/** デフォルト座標 */
		math::Vector3		default_pos;

		/** デフォルト行列*/
		math::Matrix4x4		mat_default;

		/** 守備範囲 */
		float				def_range;

		/** スケーリング */
		math::Vector3		scale;

		/** 回転 */
		float				rot;

		/** 回転量を保持 */
		float				rot_val;

		/** モデル */
		game::Model3D		*model;

		/** マネージャー管理の選手のテクスチャイメージを参照 */
		graphics::IImage	*img;
		
		/** エディットモードで"選択"されたかどうかの状態フラグ */
		bool				select;

		/** ボールを取ったかの状態 */
		bool				catch_flag;

		/** 守備範囲の判定 */
		game::BoundingSphere3D		b_sphere;

		/** 行列変換データを保持 */
		math::Matrix4x4				mat_basket,mat_r,mat_r2,mat_sc,mat_t,mat_init;
		
		/** カゴの両サイドの壁座標 Baseには後ろの座標のみが入る */
		math::Vector3		left_pos1;			//更新前
		math::Vector3		left_pos2;			//更新後

		math::Vector3		right_pos1;
		math::Vector3		right_pos2;
		
		/** カゴの中心。エディット時の座標 */
		math::Vector3		center_pos1;
		math::Vector3		center_pos2;

		math::Vector3		forword_pos1;
		math::Vector3		forword_pos2;


		/** 重力値を保持 */
		float				gravity;

		/** 行列の計算 */
		void calMatrix();

		/** 接触判定を保持
		 * @note	カゴの形なので両サイドと奥の3つに分けます
		 **/
		game::BoundingOBox3D	bobox1;			//奥
		game::BoundingOBox3D	bobox2;			//左サイド
		game::BoundingOBox3D	bobox3;			//右サイド
		game::BoundingOBox3D	b_catch;

		//エディット用のサイズが大きめの判定
		game::BoundingOBox3D	bobox4;
		
		//判定を保持
		game::BoundingSphere3D sphere;
	public:
		/** 初期化 **/
		Basket( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,float _def_range,game::Model3D *_model, graphics::IImage *_img,float _gravity )
		{
			pos			= _pos;
			default_pos = _pos;
			scale		= _scale;
			rot			= _rot;
			def_range	= _def_range;
			model		= _model;
			img			= _img;
			gravity		= _gravity;	
			//守備範囲の初期化
			b_sphere.p	= _pos;
			b_sphere.s	= def_range+3;								//守備範囲			
		};
		
		/** 初期化
		 * @note コンストラクタで引数で代入しないものを初期化する
		 **/
		void Init()
		{
			rot_val	= 0;
			
			//デフォルト行列の初期化
			const float range = def_range;
			math::Matrix4x4 r,t,s;
			r.RotateY( rot );
			t.Translate( pos );
			s.Scale( math::Vector3(range,0.2f,range)  );			//範囲のサイズ
			mat_default = r*s*t;
			select		= false; 
			
			mat_r.RotateY( rot );
			mat_t.Translate( pos );
			mat_sc.Scale( scale );
			mat_basket = mat_r*mat_sc*mat_t;
			//ボーンから取得
			left_pos1	= model->getBoneArray(1).local;				//左の壁
			right_pos1	= model->getBoneArray(2).local;
			center_pos1 = model->getBoneArray(3).local;
			forword_pos1 = model->getBoneArray(4).local;
			this->TransformVector();								//ローカル座標を変換しておく		
			//各厚みを変え、引数で受け取ったrotの傾きで判定のOBoxを変える		
			bobox1 = game::BoundingOBox3D( pos,math::Vector(scale.x*1.2f,scale.y*0.6f,0.25f),mat_r );
			//両サイド
			mat_r2.RotateY( rot+ 90*math::Math::PI/180 );			//rot角度に対しての90度
			bobox2 = game::BoundingOBox3D( left_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
			bobox3 = game::BoundingOBox3D( right_pos2,math::Vector(scale.x*0.35f,scale.y*0.6f,0.25f),mat_r2 );
			//エディット用判定
			bobox4 = game::BoundingOBox3D( center_pos2,math::Vector(scale.x*1.2f,scale.y*0.6f,1.8f),mat_r );
	
			b_catch= game::BoundingOBox3D( forword_pos2,math::Vector(scale.x*0.8f,scale.y*0.8f,0.8f),mat_r );

			catch_flag = false;
		}

		/** カゴに関するメイン
		 * @param [out]	Key			&key	:キークラス
		 * @param [out] Ball		*_ball	:ボールクラス
		 * @param [out] Rule		*rule	:ルールクラス
		 **/
		void Main( input::Key &key,Ball *_ball,Rule *rule );
	
		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		//----- set get -------
		/** 座標の取得
		 * @return pos					:3次元座標
		 **/
		math::Vector3 getPos(){ return pos; }
		
		/** 座標のセット
		 * @param [in]　Vector	&pos	:3次元座標
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** 回転度数の取得
		 * @return	rot					:回転
		 **/
		float getRot(){ return rot; }

		/** OBB判定の取得
		 * @return	bobox4				:カゴの真ん中の判定を取得
		 **/
		game::BoundingOBox3D	getBoundingOBox(){ return bobox4; }
		
		/** 回転のセット
		 * @param	float _rot			:回転
		 **/
		void setRot( float _rot ){ rot = _rot; }
	
		/** 接触判定
		 * @param [out]	Ball		*_ball	:ボールクラス
		 * @param [out] Rule		*rule	:ルールクラス
		 **/
		void Collision( Ball *_ball,Rule *rule );

		/** サイドの壁の座標がローカルなのでグローバルへの専用の変換 */
		void TransformVector();

		/** 基底の取得フラグの取得 */
		bool getCatchFlag(){ return catch_flag; }

		/** ボールを取ったかどうかのフラグをセット */
		void setCatchFlag( bool flag ){ catch_flag = flag; }
	
		/** デフォルト（初期化時）の座標取得
		 * @return	default_pos			:初期化時の3次元座標
		 **/
		math::Vector3	getDefaultPos(){ return default_pos; }

		/** スケールの取得
		 * @return scale				:3次元スケール
		 **/
		math::Vector3	getScale(){ return scale; }
				
		/** デフォルト行列の取得
		 * @return	初期化時に作成した行列
		 **/
		math::Matrix4x4	getDefaultMatrix(){ return mat_default; }

		/** 守備範囲の判定の取得
		 * @return		b_sphere	守備範囲の球
		 **/
		game::BoundingSphere3D	getBoundingSphere(){ return b_sphere; }

		/** エディット時の状態の取得
		 * @return	選択されたかどうかの状態
		 **/
		bool			getSelect(){ return select; }
		 
		//--------- Set ------------
		
		/** スケーリングをセットする
		 * @param [in] Vector3	_scale	:3次元スケール
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** 回転値をセットする
		 * @param [in]
		 **/
		void setRotVal( float _rot_val ){ rot_val = _rot_val; }
		
		/** エディット時の状態のセット
		 * @return	選択されたかどうかの状態
		 **/
		void setSelect( bool _select ){ select =_select; }


		/** 回転値の取得
		 * @return	rot_val		:回転量
		 **/
		float getRotVal(){ return rot_val; }		
	};

}}