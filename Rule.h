/** @file
 * 野球盤ルールクラスの定義
 * @date 2009/10/15
 * @author 杉野
 **/

#pragma once
#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../game/Camera3D.hpp"
#include "../input/Input.h"

namespace sugino{
/** 野球版の領域 */
namespace baseball{
		
	/** 仮定義　*/
	class PlayerManager;
	class BasketManager;
	class Ground;
	class Ball;
	class Basket;
	class Bat;

	/** 野球版ルールクラス */
	class Rule
	{
	public:
		/** 表裏の列挙体 */
		enum SIDE
		{
			IN_SIDE		= 1,		//表:先攻
			OUT_SIDE	= 2,		//裏:後攻
		};

		/** 攻撃、守備の列挙体 */
		enum STATE 
		{
			ATTACK		= 1,		//攻撃中
			DEFFENCE	= 2,		//守備中
		};
		

	private:
		
		/** テクスチャイメージを保持 */
		system::AutoPtr<graphics::IImage>	img_count;		//カウントのテクスチャ
		system::AutoPtr<graphics::IImage>	img_result;		//アウトやヒットなどの結果テクスチャ
		system::AutoPtr<graphics::IImage>	img_score;		//得点
		system::AutoPtr<graphics::IImage>	img_cntside;	//回,表裏
		system::AutoPtr<graphics::IImage>	img_change;		//交代画面
		
		/** SE */
		int se_strike;
		int se_ball;
		int se_hit;
		int se_out;
		int se_foreball;
		int se_change;
		int se_homerun;
		
		
		/** 回のカウント */
		int  cnt;

		/** カウントであり配列の要素番号でもある */
		int strike;			//ストライク

		int ball;			//ボール

		int out;			//アウト
		
		/** 総得点 */
		int	p1score;		//プレイヤー１の得点

		int p2score;		//プレイヤー２の得点
		
		/** 得点のパターン番号 */
		int p1pat;			//1桁
		int p2pat;
		int p1pat1;			//10桁
		int p2pat1;

		/** 結果の番号
		 * @note 0:表示しない 1:ヒット 2:アウト 3:フォアボール 4:ファール 5:ホームラン
		 * @note 画像のパターンにも使う
		 **/		
		int result;

		/** 点　*/
		int point;			//加算する点 p1score+=pointとなる

		/** カウント配列 */
		int strikeArray[3];	
		int ballArray[4];
		int outArray[3];

		/** trueの時加算してOK */
		bool strike_add_flag;
		bool ball_add_flag;

		/** tureの時アウトの処理をする */
		bool is_out_flag;

		/** フォアボールのフラグ
		 * @note Runnerクラスで進塁処理を行う
		 **/
		bool foreball_flag;

		/** シングルヒットのフラグ
		 * @note ヒットの結果が得られる壁に当たった判定ではない
		 *		 あくまで球の勢いが無くなった時のヒット
		 **/
		bool normal_hit_flag;

		/** フェンスによるヒットのフラグ
		 * @note フェンスに当たった結果のフラグ
		 *　	 アウトのフェンスも含む
		 *		 当たった内容の定数をフラグをして扱う 2ならツーベースヒットなど
		 **/
		int fence_hit_flag;
		
		/** 得点が入るフラグ
		 * @note 条件を満たしたら立つ
		 **/
		bool add_score_flag;

		/** ヒットを打ったというフラグ。ベースだろうがホームランだろうがヒットとして扱う
		 *  ヒットを打ったら、ストライクやボールカウントを初期化しないといけないのでその時に使用する
		 **/
		bool hit_flag;

		/** 交代フラグ */
		bool change_flag;

		/** ゲームセットフラグ */
		bool end_flag;

		/** アルファ値フラグ */
		bool alpha_flag;
		int	 alpha_cnt;

		/** カウントのパターン計算 */
		void CalPattern();

		/** カウント増加処理
		 * @param [out]	Ballbase	*bbase	:ボールクラス
		 * @param [out]	Ground		*ground	:グラウンドクラス
		 * @param [out] Bat			&_bat
		 **/
		void addCount( baseball::Ball *bbase, Ground *ground,Bat &_bat );
		
		/** カゴがボールを取ったかどうかの判定をマネージャーで受け取りさらに個別に処理させるための処理
		 * @param [out]	Ballbase	*bbase	:ボールクラス
		 * @param [out]	Ground		*ground	:グラウンドクラス
		 * @param [out]	BasketManager	*bm	:カゴのマネージャー
		 **/
		void Result( baseball::Ball *bbase, Ground *ground, BasketManager &bm,PlayerManager &pm );
		
		/** Resultで処理分けされた各カゴでのボールを取ったかどうかの判定
		 * @param [out] BallBase	*bbase	:ボールクラス
		 * @param [out]	Ground		*ground	:グラウンドクラス
		 **/
		void BasketResult( baseball::Ball *bbase, Ground *ground, Basket *bask );

		/** Resultで処理分けされた途中でボールが停止したときのヒット判定
		 * @param [out] BallBase	*bbase	:ボールクラス
		 * @param [out]	Ground		*ground	:グラウンドクラス
		 * @param [out]	PlayerManager	&pm	:プレイヤーのマネージャークラス
		 **/
		void HitResult( baseball::Ball *bbase, Ground *ground,Basket *bask, PlayerManager &pm );

		/** 点加算
		 * @param STATE p1:	メインのプレイヤー１の攻守の状態
		 **/
		void AddScore( STATE *p1 );

		/** 交代処理
		 * @param [out]	BasketManager	&bm:	カゴのマネージャー
		 * @param [out] PlayerManager	&pm:	選手のマネージャー
		 * @param [in]	ball		  *ball:	ボールクラス
		 * @param [out] STATE			*p1:	メインのプレイヤー１の攻守の状態
		 * @param [out] STATE			*p2:	メインのプレイヤー２の攻守の状態
		 **/
		void Change( BasketManager &bm,PlayerManager &pm,Ball *bbase,STATE *p1,STATE *p2 );
		
			
		/** 回が進むごとの初期化
		 * 
		 * @note 回以外のカウントを全て初期化
		 **/
		void NextInit( baseball::Ball *bbase, Bat &_bat );

		/** 打者ごとの初期化
		 * @note ボール、ストライクカウントを初期化
		 **/
		void BatterInit();
		
		/** アウトの処理
		 * @param [in]	Ballbase *bbase	:ボールの基底クラス
		 * @param [in]	Bat		 *_bat	:バットクラス
		 * @note 三振、フェンス、キャッチした時のアウト処理
		 **/
		void Out( Ball *bbase,Bat *_bat );
		
		/** 攻守の状態を保持 */
		STATE	state;

		/** アルファ値 */
		DWORD	alpha;
		DWORD	r_alpha;				//結果表示用のアルファ値

		/** 画像の移動 */
		int home_x;						//ホームラン画像の座標
		int home_mov;					//ホームラン画像の移動量
	
					
		/** 守備範囲の判定 */
		game::BoundingSphere3D	b_sphere;
	
	public:
		/** コンストラクタ */
		Rule() : cnt(1),out(0),ball(0),strike(0)
		{
			strike_add_flag =false;
			ball_add_flag	=false;		
			foreball_flag	=false;
			normal_hit_flag =false;
			hit_flag		=false;		
			change_flag		=false;
			fence_hit_flag	=false;
			add_score_flag	=false;
			is_out_flag		=false;
			end_flag		=false;
			alpha_flag		=false; 
			alpha_cnt		=0;

			img_count		= NULL;
			img_result		= NULL;
			img_score		= NULL;
			img_cntside		= NULL;
			img_change		= NULL;

			int i;
			for(i=0;i<sizeof(outArray)>>2;i++) outArray[i]	=0;
			for(i=0;i<sizeof(ballArray)>>2;i++) ballArray[i]	=0;
			for(i=0;i<sizeof(strikeArray)>>2;i++) strikeArray[i]	=0;

			p1pat	=0;
			p2pat	=0;
			p1pat1	=0;
			p2pat1	=0;

			result = 0;

			p1score = 0;
			p2score = 0;
			point	= 0;

			//範囲初期化
			b_sphere.p = math::Vector3( 1,1,-75 );	//ホームベースより少し前
			b_sphere.s = 28;

		}

		/** ボール状態が0のときの初期化 */
		void isReturn();


		/** 初期化
		 * @param [in]	IImage	*count_img	:レンダーのcreateImageで作成したデータ
		 * @param [in]	IImage	*result_img	:レンダーのcreateImageで作成したデータ
		 * @param [in]	IImage	*score_img	:レンダーのcreateImageで作成したデータ
		 * @param [in]	IImage	*side_img	:レンダーのcreateImageで作成したデータ
		 * @param [in]	IImage	*side_img	:レンダーのcreateImageで作成したデータ
		 **/
		void Init( graphics::IImage *count_img,graphics::IImage *result_img,
				   graphics::IImage *score_img,graphics::IImage *side_img,
				   graphics::IImage *change_img);

		/** メイン
		 * @param [in]	BallBase		*bbase	:ボールクラス
		 * @param [in]	Ground			*ground	:グラウンドクラス
		 * @param [in]	BasketManager	*bm		:カゴのマネージャークラス
		 * @param [in]	PlayerManager	&pm		:カゴのマネージャークラス
		 * pparam [in]	Bat				&_bat	:バットクラス
		 * @param [in]	STATE			*p1		:プレイヤー１の状態列挙体
		 * @param [in]	STATE			*p2		:プレイヤー２の状態列挙体
		 * @param [in]	int				*scene_f:メインのシーンフラグ
		 **/
		void Main( baseball::Ball *bbase, Ground *ground,BasketManager &bm,PlayerManager &pm,Bat &_bat,STATE *p1,STATE *p2,int *scene_f );

		/** 描画
		 * @param [out]	IRender		*render	:レンダー
		 * @param [out]	Matrix4x4	&vps	:掛け合わしたカメラ行列
		 * @param [out]	Bat			*bat	:バットクラス
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps,Bat &_bat );

		/** アルファ値分解、計算、 合成
		 * @return alpha >= 1.0f:true		それ以外:false 
		 **/
		bool ColAlpha();

		//--------- Set ------------
		/** 回のセット
		 * @param [in]	int _cnt	:回
		 **/
		void setCnt( const int _cnt ){ cnt = _cnt; }		//回のセット

		/** アウトカウントをセット
		 * @param [in]	int _out	:アウトカウント
		 **/
		void setOut( const int _out ){ out = _out; }	
		
		/** ボールカウントのセット
		 * @param [in]	int _ball	:ボールカウント
		 **/
		void setBall( const int _ball ){ ball = _ball; }	

		/** ストライクカウントのセット
		 * @param [in]	int _strike	:ストライクカウント
		 **/
		void setStrike( int _strike ){ strike = _strike; }
		

		/** 加算条件フラグのセット
		 * @note 別のクラスで加算条件が成立(ストライクやファール)したらセットする
		 **/
		void setStrikeAddFlag( bool flag ){ strike_add_flag = flag; }
		
		/** 加算条件フラグのセット
		 * @note 別のクラスで加算条件が成立したらセットする
		 **/
		void setBallAddFlag( bool flag ){ ball_add_flag = flag; }

		/** 通常ヒットのフラグのセット
		 * @param [in] bool flag			:状態フラグ
		 **/
		void setNormalHitFlag( bool flag ){ normal_hit_flag = flag; }
		
		/** ヒットを打ったフラグ
		 * @param [in] bool	flag			:状態フラグ
		 * @note フェンスも含む
		 **/
		void setHitFlag( bool flag ){ hit_flag = flag; }

		/** フェンスに当たったヒットフラグ
		 * @param [in]	bool flag			:状態フラグ
		 * @note 0:アウトフェンス 1:シングルヒット 2:ツーベースヒット
		 *		 3:スリーベースヒット 4:ホームラン
		 **/
		void setFenceHitFlag( int flag ){ fence_hit_flag = flag; }
		
		/** 得点加算フラグ
		 * @param [in]	bool flag			:状態フラグ
		 **/
		void setAddScoreFlag( bool flag ) { add_score_flag = flag; }

		/** フォアボールのフラグのセット
		 * @param [in] bool	flag			:状態フラグ
		 **/
		void setForeballFlag( bool flag ){ foreball_flag = flag; }

		/** 交代フラグのセット
		 * @param [in]	bool flag			:状態フラグ 
		 **/
		void setChangeFlag( bool flag ){ change_flag = flag; }
		
		/** 点のセット
		 * @param [in]	int point			:点
		 * @note addScoreFlag=trueの時に加算される値。falseの時は常に1が入る
		 **/
		void setPoint( int _point ){ point = _point; }
		
		/** アウトフラグのセット
		 * @param [in]	bool flag			:フラグ
		 * @note trueになるとアウト処理をする
		 **/
		void setIsOutFlag( bool flag ){ is_out_flag= flag; }

		//--------- Get ------------
		/** 回の取得
		 * @return	cnt			:回
		 **/
		const int &getCnt(){ return cnt; }			//回の取得
		
		/** アウトカウント取得
		 * @return	out				:アウトカウント
		 **/
		const int &getOut(){ return out; }
		
		/** ボールカウントの取得
		 * @return	ball			:ボールカウント
		 **/
		const int &getBall(){ return ball; }
		
		/** ストライクカウントの取得
		 * @return strike			:ストライクカウント
		 **/
		const int &getStrike(){ return strike; }
	
		/** フォアボールフラグの取得
		 * @return true:フォアボール	false:フォアボールでない
		 **/
		bool getForeballFlag(){ return  foreball_flag; }		

		/** 通常ヒットフラグの取得
		 * @return true:ヒット			false:ヒットではない
		 **/
		bool getNormalHitFlag(){ return normal_hit_flag; }

		/** ヒットフラグの取得
		 * @return	true:アウトやファールではない結果が得られた false:それ以外 
		 **/
		bool getHitFlag(){ return hit_flag; }
		
		/** 交代フラグの取得
		 * @return	true:交代				false:交代すべきでない
		 **/
		bool getChangeFlag(){ return change_flag; }
		
		/** フェンスに当たったヒットフラグの取得
		 * @return	0:当たってない　1:シングルヒット　2:ツーベースヒット 
		 *			3:スリーベースヒット 4:ホームラン
		 **/
		int getFenceHitFlag(){ return fence_hit_flag; }
		
		/** 得点加算フラグ取得
		 * @return	true:得点加算			false:加算しない
		 **/
		bool getAddScoreFlag() { return add_score_flag; }

		/** プレイヤー1の得点の取得 */
		int getP1Score(){ return p1score; }
		/** プレイヤー2の得点の取得 */
		int getP2Score(){ return p2score; }
				
		/** 得点のパターン番号の取得
		 * @param [in]	int *pat1	:1桁のパターン番号
		 * @param [in]	int *pat2	:10桁のパターン番号
		 **/
		void getP1Pat( int *pat1, int *pat2 )
		{
			*pat1 = p1pat;
			*pat2 = p1pat1;
		};

		void getP2Pat( int *pat1, int *pat2 )
		{
			*pat1 = p2pat;
			*pat2 = p2pat1;			
		};

		/** 結果パターンのセット 
		 * @param [in]	pat	:パターン番号
		 * @note 0:表示しない 1:ヒット 2:アウト 3:フォアボール 4:ファール 5:ホームラン
		 **/
		//0:表示しない 1:ヒット 2:アウト 3:フォアボール 4:ファール 5:ホームラン
		
		void setResult( int res  ){ result = res; } 
		/** 結果の取得
		 * @return	result_pat			:結果
		 * @note 結果を得られるのでパターンだけでなく、「その結果」だからこその処理も使える
		 **/
		int getResult(){ return result; }

		/** 結果表示のアルファ値計算
		 * @param [in]	float	add	:アルファ加算値
		 * @return true:処理終了 false:処理中
		 * @note falseの間は他の処理をさせない
		 **/
		bool ColResultAlpha( float add );
		
		/** ホームラン時の画像の移動 */
		bool DrawMove();

		/** ゲームセットの処理
		 * @param [out] STATE *p1	:プレイヤー1の状態
		 * @return 0:終了条件に満たしてない	6:プレイヤー1の勝利（パターン番号) 7:プレイヤー2の勝利（パターン番号) 8:引き分けのパターン番号
		 **/
		int GameSet( STATE *p1 );	
	
		/** 守備のカゴが絶対に取得できない、ピッチャーよりも前のキャッチャー寄り部分の判定
		 * @param [in]	Ball	*_ball	:ボールクラス
		 **/
		bool InField( Ball *_ball );
	};
}}