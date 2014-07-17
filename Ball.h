/** @file
 * ボールクラスの定義
 * @date 2009/09/14
 * @author 杉野
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Object2D.h"
#include "../game/Collision.h"
#include "../math/Vector.h"

#include "Stadium.h"
#include "Player.h"
#include "Rule.h"

namespace sugino{
namespace baseball{
	
	//仮定義
	class Ground;
	class Rule;

	/** ボールのクラス */
	class Ball
	{
	private:
		/** ボールの判定情報を保持 */
		game::BoundingSphere3D	sphere;
		/** 終着点(バックネット)の判定情報を保持 */
		game::BoundingBox3D		bbox1;

		/** 行列 */
		math::Matrix4x4			mat_ball,mat_r,mat_sc,mat_t,mat_init;

		/** 行列計算 */
		void calMatrix();

		/** 球の半径サイズ */
		float					size;
		
		/** 球の目的地（向かう先)を保持 */
		math::Vector3			target;				//終着点座標	
		
		/** 位置座標を保持 */
		math::Vector3			pos;

		/** 向きを保持 */
		math::Vector3			direct;

		/** 球の速度 */
		float					velosity;
			
		/** 最大速度を保持 */
		float					max_velosity;

		/** 最小速度を保持 */
		float					min_velosity;

		/** 重力値を保持 */
		float					gravity;

		/** 質量を保持 */
		float					mass;

		/** 回転 */
		float					rotate;

		/** 回転度数 */
		float					rotate_val;

		/** モデルを保持 */
		game::Model3D			model;

		/** ボールのテクスチャイメージを保持 */
		system::AutoPtr<graphics::IImage>	img;

		/** 状態フラグを保持
		 * @note	0:出現していない	1:投球時	2:打ち返した時 3:結果時
		 **/
		int state_flag;
		
		/** 消える魔球フラグ
		 * true:魔球発動中 false:してない
		 **/
		bool makyu_flag;

	public:
		//デフォルト
		Ball()
		{
			direct =math::Vector3(0,0,0);
			velosity=0;
			max_velosity=2.45f;
			min_velosity=0;
			rotate =0;
			rotate_val=0.2f;
			state_flag = false;											//状態を伏せる
	
			//判定の初期化
			bbox1.p = math::Vector3( -0.587f,0,-110 );			//中心はターゲットより少し下に
			bbox1.s	= math::Vector3( 35,15,0.8f);						//ターゲットサイズ以下の判定幅
			
			target	= math::Vector3( bbox1.p.x,1.40f,bbox1.p.z );			//目的地の座標		
			img = NULL;
			makyu_flag = false;
		}

		//初期化
		void Init( char* model_name,graphics::IImage *ball_img );


		//クラスのメイン
		void Main( baseball::Ground *g, baseball::Rule *rule,input::Key *key,Rule::STATE &p1state );
		
		/** 判定の取得
		 * @reutrn sphere	:球の判定
		 **/
		game::BoundingSphere3D	&getBoundingObj(){ return sphere; }

		/** 移動 */
		void Move( input::Key &key,Rule::STATE &p1state );
		
		/** 表示
		 * @param [out]IRender	*render	:レンダー
		 * @param [out]Matrix4x4 &vps	:カメラ行列
		 **/
		void Draw( graphics::IRender *render,math::Matrix4x4 &vps );

		/** ボールの終着点判定
		 * @return	false:	未到達	true:バックネットの終着点まで着いた
		 **/
		bool BackNetCollision();
		
		//-------　Set	--------
		/** 目標地点のセット
		 * @param [in] Vector3	&_target	;目標地点
		 **/
		void setTarget( math::Vector3 &_target ){ target = _target; }

		//-------　Get	--------
		/** 目標地点の取得
		 * @return	target	目標地点
		 **/
		math::Vector3 &getTarget(){ return target; }
	
				//--------- Set ------------
		/** 位置取得
		 * @return	pos			:ローカル位置座標
		 **/
		math::Vector3 &getPos(){ return pos; }
		
		/** 最大速度の取得
		 * @return  max_speed	:最高速度のベクトル
		 **/
		const float	getMaxVelosity(){ return max_velosity; }

		/** 最低速度の取得
		 * @return	min_speed	:最低速度のベクトル
		 **/
		const float getMinVelosity(){ return min_velosity; }

		/** 方向の取得
		 * @return	direct		:方向の単位ベクトル
		 **/
		const math::Vector3	&getDirectValue()
		{
			if( !direct.Length() ) system::Exception( "向きを取得できませんでした" ); 
			return direct.Normalize();
		}
		
		/** ボールの速度の取得
		 * @return velosity		:球速
		 **/
		float getVelosity(){ return velosity; } 

		/** 重力値の取得 
		 * @return	gravity		:重力値のベクトル
		 **/
		float getGravity(){ return gravity; }
		
		/** 質量値の取得 
		 * @return	mass		:質量値のベクトル
		 **/
		float getMass(){ return mass; }

		/** 回転の取得 
		 * @return	rotate_val	:回転
		 **/
		float getRotateValue(){ return rotate_val; }


		/** 状態フラグの取得
		 * @return	state_floag	:球の状態
		 **/
		int getStateFlag(){ return state_flag; }

		//-------　Set	--------
		/** 位置座標のセット
		 * @param [in]	Vector3	 _pos	:セットする座標
		 **/
		void setPos( const math::Vector3 &_pos ){ pos = _pos; }

		/** 速度の"最大値"をセット
		 * @param [in]	Vector3	 _max	:セットする最大速度
		 **/
		void setMaxVelosity( const float &_max ){ max_velosity = _max; }

		/** 速度の"最低値"をセット
		 * @param [in]	Vector3	_min	:セットする最低速度
		 **/
		void setMinVelosity( const float &_min ){ min_velosity = _min; }
		
		/** 速度のセット
		 * @param [in]	float _velosity	:速度
		 **/
		void setVelosity( const float &_velosity ){ velosity = _velosity; }

		/** 方向のセット
		 * @param [in] Vector3	_direct	:セットする向き
		 **/
		void setDirectValue( const math::Vector3 &_direct ){ direct = _direct; }

		/** 重力値のセット
		 * @param [in] float	_gravity:セットする重力値
		 **/
		void setGravity( float _gravity ){ gravity = _gravity; }
		
		/** 質量値のセット
		 * @param [in] float	_mass	:セットする質量値
		 **/
		void setMass( float _mass ){ mass = _mass; }

		/** 回転量のセット
         * @param [in] float	rot_val	:セットする回転量
		 **/
		void setRotValue( float rot_val ){ rotate_val = rot_val; }

		/** 状態フラグのセット
		 * @param [in] bool	_state		:セットする状態
		 * @note	0:出現してない 1:ピッチング中 2:打った球
		 **/
		void setStateFlag( int state ){ state_flag = state; }

		//------- Move値、Rotate値、補正 -----------
		/** 座標更新
		 * @param [in] Vector3	dir			:行きたい方向
		 * @param [in] float	_velosity	:速度
		 * @note 勢いが無くなっていくなどの変化をつける時など
		 **/
		void addMove( const math::Vector3 &dir, const float _velosity );

		/** 回転量を加える
         * @param [in] float add_rot		:加える回転量
		 **/
		void addRotate( float add_rot );

		/** 座標の補正
		 * @param [in] Vector3	_pos		:加える補正量 
		 **/
		void revisePos( const math::Vector3 &_pos ){ pos += _pos; }

		/** 魔球フラグのセット */
		void setMakyuFlag( bool flag ){ makyu_flag = flag; }

		/** バックネット座標の取得
		 * @return Vector3 バックネット座標
		 **/
		math::Vector3 &getBackNetPos(){ return bbox1.p; }
	};
}}