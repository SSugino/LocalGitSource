/** @file
 * バットクラスの定義
 * @date 2009/08/30
 * @author 杉野
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../input/Input.h"
#include "Rule.h"

namespace sugino{
namespace baseball{

	/** 仮定義 */
	class Ball;

	/** バット/バッター（打席)クラス */
	class Bat
	{
	public:
		
		/** 打席の列挙体 */
		enum BATTER_TYPE{
			LEFT	 = 1,				//キャッチャーからみて左バッター
			RIGHT	 = 2,				//キャッチャーからみて右バッター
		};

	private:
		/** バットのテクスチャイメージを保持 */
		system::AutoPtr<graphics::IImage>	img; 

		/** モデルを保持 */
		game::Model3D						model; 
		
		/** 打席を保持 */
		BATTER_TYPE							type;

		/** 行列変換データを保持 */
		math::Matrix4x4						mat_bat,mat_r,mat_sc,mat_t,mat_init;
		
		/** Se */
		int se_swing;
		int se_hitting;
		bool se_swing_f;

		/** 傾く最大角度 */
		float								lean_rot;

		/** 傾くスピード */
		float								lean_speed;

		/** 傾くカウント
		* @note	lean_speedと計算するのでfloat型で用意
		**/
		float								lean_cnt;

		/** 傾く条件フラグ
		 * @note	0:待機	1:傾き中	2:戻り中
		 **/
		int									lean_flag;
		
		/** 回転角度 */
		float								rot;
		
		/** 回転量 */
		float								rot_val;

		/** パワー
		 * @note	回転が戻るときに増える
		 **/
		float								pow;

		/** 配置座標 */
		math::Vector3						pos;

		/** 当たり判定の座標
		 * @note モデルのボーン位置ローカル座標を画面座標に変換する
		 **/
		math::Vector3						col_pos;			//更新前
		math::Vector3						col_pos2;			//更新後

		/** 判定を持つ */
		game::BoundingLine3D				line;

		/** 行列計算用 */
		void calMatrix();
		
		/**
		 * @param [in]	Key			key			:キー操作のためにシーンで受け取ったKeyを渡す
		 * @param [out] Ball		_ball		:ボールとの判定のためにボールクラスを渡す
		 * @param [out] Rule		rule		:ルールクラス
		 * @param [out] STATE		p1state		:プレイヤー１の攻守状態
		 **/
		void		Swing( input::Key *key,Ball *_ball,Rule *_rule,Rule::STATE &p1state  );			
	public:
		
		/** 初期化
		 * @param [in] name  : 読み込むモデルファイル名
		 * @param [in] _img  : レンダーのcreateImageで作成したデータ
		 **/
		void Init( char *model_name, graphics::IImage *_img );

		/** バットに関するメイン
		 * @param [in]	Key			key			:キー操作のためにシーンで受け取ったKeyを渡す
		 * @param [out] Ball		ball		:ボールとの判定のためにボールクラスを渡す
		 * @param [out] Rule		rule		:ルールクラス
		 * @param [out] STATE		p1state		:プレイヤー１の攻守状態
		 **/
		void Main( input::Key *key,Ball *ball,Rule *_rule,Rule::STATE &p1state );
	
		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** 配置座標取得 */
		math::Vector3 &getPos(){ return pos; }

		/** 配置座標のセット */
		void setPos( math::Vector3 &_pos ){ pos = _pos; }
		
		/** 打席のセット
		 * @param [in] BATTER_TYPE	:バッターの打席列挙体
		 **/
		void setBatter( BATTER_TYPE type );	
		
		BATTER_TYPE getBatter(){ return type; }

		/** バット自身がボールとの判定を持たす
		 * @param [out]	Ball		*ball	:ボールクラス
		 **/
		void Collision( Ball *ball);
		
		/** バットの先の座標がローカルなのでグローバルへの専用の変換 */
		void TransformVector();
		
		//----- set/get -------------------
		/** 傾く最大角度の取得
		 * @return	_lean_rot					:傾き最大角度
		 **/
		float getLeanRot(){ return lean_rot; }

		/** 傾くスピードのセット
		 * @param [in]	float	_lean_speed		:傾くスピード
		 **/
		void setLeanSpeed( float _lean_speed ){	 lean_speed = _lean_speed; }

		/** 傾くスピードの取得
		 * @return	lean_speed					:傾くスピード
		 **/
		float getLeanSpeed(){ return lean_speed; }

		/** 傾きフラグのセット
		 * @param [in]	int	state				:状態フラグの番号
		 * @note 0:待機	1:傾き中	2:戻り中
		 **/
		void setLeanFlag( int state ){ lean_flag=state; }
		
		/** 傾きフラグの取得
		 * @return lean_flag					:状態フラグ
		 **/
		int getLeanFlag(){ return lean_flag; }
	
		/** 傾くカウントの取得
		 * @return lean_cnt						:傾きのカウント
		 * @note カウントがある間"振っている"
		 **/
		float	getLeanCnt(){ return lean_cnt; }
		
		/** 傾くカウントのセット
		 * @param [in] int						:傾きのカウント
		 **/
		void setLeanCnt( float _lean_cnt ){ lean_cnt = _lean_cnt; }

	};
}}