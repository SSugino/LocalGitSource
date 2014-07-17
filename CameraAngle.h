/** @file
 * ゲーム上でのカメラアングルクラスの定義
 * @date 2009/11/13
 * @author 杉野
 **/

#include "../game/Camera3D.hpp"
#include "../input/Input.h"
#include "Rule.h"


namespace sugino{
namespace baseball{
	
	/** 仮定義 */
	class Rule;
	class Camera;
	class Ball;
	
	/** カメラのアングルクラス */
	class CamAngle
	{
	private:
		/** エディット時にカメラの見てる先 */

		math::Vector3	edit_cpos;
		/** 演出時の見てる先計算用 */
		math::Vector3	at;

		/** ランナー用のカメラを用意 */
		game::Camera3D		cam1st;				//1塁を見る用のカメラを保持
		game::Camera3D		cam3rd;				//3塁を見る用のカメラを保持
		
		/** ランナーの仮想画面 */
		system::Rect		right;				//ファースト用の仮想画面
		
		system::Rect		left;				//サード用の仮想画面

		/** カメラアングルのメイン1
		 * @param [out] Camera3D	&cam	:カメラクラス
		 * @param [out]	Ball		&_ball	:ボールクラス
		 * @param [out] Rule		&_rule	:ルールクラス
		 * @param [out] Key			&key	:キークラス
		 * @param [in]	bool		mode	:エディットモード
		 * @param [out]	STATE		&p1		:プレイヤー１の攻守の状態
		 **/
		void Aspect( game::Camera3D &cam, Ball &_ball, Rule &_rule, input::Key &key,bool mode,Rule::STATE &p1 );
				
		/** カメラウェイトカウント */	
		int wait;

	public:
		/** コンストラクタ */
		CamAngle()
		{
			edit_cpos = math::Vector3(0,1,0);
			wait	=0;			
		};
		
		/** ランナー用カメラの初期化 */
		void Init();
		
		/** 処理分け用のメイン */
		void Main(  game::Camera3D &cam, Ball &_ball, Rule &_rule,PlayerManager &pm,input::Key &key,bool mode,Rule::STATE &p1 );
		
		/** ViewPortで描画範囲を作る */
		void Draw( graphics::IRender *render,PlayerManager &pm,Ground &g,Ball &_ball );

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
			
		//結果によるカメラ演出関数
		void isFoul( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isHit( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isOut( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		void isHomeRun( game::Camera3D &cam, Ball &_ball, Rule &_rule );
		
		/** 1塁ランナーのカメラ行列の取得 */
		math::Matrix4x4 get1stCamMatrix(){ return cam1st.getMatrix(); }		//1塁のカメラの掛け合わせたカメラ行列の取得
		
		/** 3塁ランナーのカメラ行列の取得 */
		math::Matrix4x4 get3rdCamMatrix(){ return cam3rd.getMatrix(); }		//3塁のカメラの掛け合わせたカメラ行列の取得
		
	};
}}