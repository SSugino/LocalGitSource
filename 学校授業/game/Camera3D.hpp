/** @file 
 * ３Ｄカメラ操作クラス
 * @date 2009/07/27
 * @author 杉野
 **/
//--------------------------------------------------------
#pragma once

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "Collision.h"

namespace sugino{
	namespace game{
		
		class Camera3D;						//仮定義
		typedef Camera3D	Camera;			//省略
		/** カメラ操作クラス 
		 * 位置・拡大・回転の指定が可能なカメラ
		 * このカメラから得られる行列を使って、表示するものをTransformする
		 **/
		class Camera3D{
		private:
			/** Z 軸ベクトル */
			math::Vector		z_axis;
			/** X 軸ベクトル */
			math::Vector		x_axis;
			/** Y 軸ベクトル */
			math::Vector		y_axis;
					
			/** 座標位置 */
			math::Vector		pos;
			/** 見ている物（見ている場所) LookAt関数のAtの部分 */
			math::Vector		at;
			/** スクリーンサイズ */
			int					wid;		//スクリーン幅
			int					hei;		//スクリーン高さ

			/** 移動量 */
			math::Vector3		move;

			/** 表示範囲の手前 */
			float				camera_near;
				
			/** 表示範囲の奥 */
			float				camera_far;

			/** カメラ行列 */
			math::Matrix4x4		camera;
			/** スクリーン行列 */
			math::Matrix4x4		screen;
			/** プロジェクション  */
			math::Matrix4x4		proj;
			
			/** 掛け合わせたもの受け取る用 */
			math::Matrix4x4		view;

			/** 回転行列 */
			math::Matrix4x4		rotate;

			/** 更新フラグ */
			bool				is_update;

			/** カメラ行列の更新 掛けあわせ行う
			 * camera * proj * screen
			 **/
			void Update()
			{
				if( is_update ){										//更新フラグたってたら
					
					math::Vector3 vec = (at - pos).Normalize();				//見ている方向ベクトル
					camera.View( vec, math::Vector(0,1,0),this->pos );		//ビュー行列
					view = camera*proj*screen;							//View行列変数に掛け合わせた行列を入れ更新する
					is_update = false;
				}
			}
		public:

			/** コンストラクタ */
			Camera3D() : z_axis(0,0,1),x_axis(1,0,0),y_axis(0,1,0),pos(0,0,0),at(0,0,1),move(0,0,0),wid(640),hei(480),camera_near(0),camera_far(0)
			{};

			/** デストラクタ */
			~Camera3D(){};

			/** 引数つきコンストラクタ */
			Camera3D( math::Vector3 &_pos,math::Vector3 &at,int sc_wid,int sc_hei,float per_wid, float per_hei,float _near, float _far ) :
			pos(_pos),wid(sc_wid),hei(sc_hei),camera_near(_near),camera_far(_near),z_axis(math::Vector(0,0,1)),x_axis(math::Vector(1,0,0)),y_axis(math::Vector(0,1,0))
			{		
				//Updateの行列掛け合わせ時に必要な初期化
				this->setPosition( _pos );
				this->setAt( at );
				
				setScreen(sc_wid,sc_hei);
				camera.View( z_axis,y_axis,_pos  );				//View行列作る
				camera.LookAt( _pos,at,math::Vector( 0,1,0 ) );		//LookAt行列作る
				proj.Perspective( per_wid,per_hei,_near,_far );	//Persective作る
				is_update = true;					
			}

			//----------------------初期化関数--------------------------------------------
			/** 初期化
			 * @attention 各行列( Camera/Screen/Persective/LookAt )を初期化していく
			 * @param [in]	Vector3		pos		;カメラの座標
			 * @param [in]	Vector3		at		:見ている対象
			 * @param [in]	int			sc_wid	:スクリーン幅
			 * @param [in]	int			sc_hei	:スクリーン高さ
			 * @param [in]	float		per_wid	:パースの幅
			 * @param [in]	float		per_hei	:パースの高さ
			 * @param [in]	float		_near	:描画範囲の手前
			 * @param [in]	float		_far	:描画範囲の奥行き
			 **/
			void Initialize( math::Vector3 &pos,math::Vector3 &_at,int sc_wid,int sc_hei,float per_wid, float per_hei,float _near, float _far )
			{
				camera.View( z_axis,y_axis,pos  );				//View行列作る
				camera.LookAt( pos,at,math::Vector( 0,1,0 ) );	//LookAt行列作る
				screen.Screen( sc_wid,sc_hei );					//Screen行列作る
				proj.Perspective(per_wid,per_hei,_near,_far );	//Persective作る
				setPosition( pos );								//カメラの座標初期化
				camera_near = _near;							//描画範囲を持っておく
				camera_far  = _far;
				at		= _at;									//見ている対象も持っておく
				is_update = true;
			}

			/** LookAt行列初期化(メンバ変数Matrix4x4 camera)
			 * @param [in] Vector3 &pos		 : カメラ場所座標
			 * @param [in] Vector3 &at		 : 見てる先（対象）
			 * @param [in] Vector3 &up		 : 仮の上方向ベクトル
			 **/ 
			void setLookAt( math::Vector3 &_pos,math::Vector3 &_at, math::Vector3 &_up ){
				at = _at;									//メンバ変数に値持たす
				pos = _pos;
								//メンバ変数cameraでLookAt行列
				is_update = true;
			}

			/** スクリーン行列の作成 */
			void setScreen( int w,int h ){
				wid = w;hei = h;		//値をとっておく
				screen.Screen(w,h);		//メンバ変数screenでScreen行列を作る
				is_update = true;
			}

			/** スクリーン行列の作成（Rectで渡す) */
			void setScreen( system::Rect &rc ){
				wid = rc.right - rc.left;
				hei = rc.bottom - rc.top;
				if( wid<0 ) wid = -wid;	//符号反転
				if( hei<0 ) hei = -hei;
				screen.Screen( rc );
				is_update = true;
			}
			
			/** パーセクティブ行列の作成 */
			void setPercective( float per_wid,float per_hei,float _near,float _far ){
				camera_near = _near;							//描画範囲を持っておく
				camera_far  = _far;
				//proj.Perspective(per_wid,per_hei,_near,_far );
				proj.Perspective(2*(per_wid/(float)per_hei ),2,_near,_far );
				//proj.Perspective( 2*1.333333f,2,_near,_far );
				is_update = true;
			}
			


			//---------------------行列取得------------------------------------
			/** カメラ行列( View行列呼んでいるもの )取得
			 * @return 掛け合わされたcamera行列
			 **/
			math::Matrix4x4 &getCameraMatrix(){ Update(); return camera; }
			
			/** スクリーン( Screen行列呼んでいるもの)取得
			 * @return 掛け合わされたScreen行列
			 **/
			math::Matrix4x4 &getScreenMatrix(){ return screen; }
			
			/** プロジェクション( Perspective行列呼んでいるもの)取得
			 * @return 掛け合わされたPerspective行列
			 **/
			math::Matrix4x4 &getProjectionMatrix(){ return proj; }

			/** カメラ*プロジェクション*スクリーン取得
			 * @return 更新し終わったカメラ行列
			 **/
			math::Matrix4x4 &getMatrix(){ 
				Update();				//掛け合わせ
				return view;
			}



			//----------------カメラの回転-----------------------------
			/** 任意点によるカメラのX回転
			 * @param [in] float		r		:回転度数
			 * @param [in] Vector3		&target	:任意点
			 **/
			void RotateXPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateX( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			/** 任意点によるカメラのY回転
			 * @param [in] float		r		:回転度数
			 * @param [in] Vector3		&target	:任意点
			 **/
			void RotateYPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateY( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			/** 任意点によるカメラのZ回転
			 * @param [in] float		r		:回転度数
			 * @param [in] Vector3		&target	:任意点
			 **/
			void RotateZPosition( float r,math::Vector3 &target ){
				math::Matrix m;
				m.RotateZ( r );
				rotate.RotatePos( m,target );	
				pos = rotate.TransformVector( pos );

				is_update = true; 
			};
			
			/** "軸"による任意"点"の回転
			 * @param [in] Vector3	axis	:指定軸 Vector(0,1,0)でY軸 (1,0,0)でX軸 (1,1,0)でXY軸など
   			 * @param [in] Vector3	target	:任意点
			 * @param [in] float	r		:回転度数
			 **/
			void Rotate( math::Vector3 &axis,math::Vector3 &target,float r ) {
				math::Matrix m;
				if( axis.x>1 || axis.y>1 || axis.z>1 ) m.RotateAxis( axis.Normalize(), r );	//大きい値は正規化してやる

				m.RotateAxis( axis, r );
				rotate.RotatePos( m, target );

				pos = rotate.TransformVector( pos );	//座標更新
				at = rotate.TransformRotation( at );
			
				is_update = true;						//行列更新
			}

					
			/** カメラが見ている先の設定
			 * @param [in] const Vector3 &_at	:カメラが見ている対象座標
			 **/
			void setAt( const math::Vector &_at ){ at = _at; 	is_update = true; }

			/** カメラ座標の設定
			 * @param [in] const Vector3 &p	:カメラの座標
			 **/
			void setPosition( const math::Vector3 &p ){ pos=p; is_update = true; }
			
			/** 表示範囲の指定 */
			/** 手前の設定
			 * @param [in] float _near	:カメラで見る手前の表示範囲
			 **/
			void setNear( float _near ){ camera_near = _near; 	is_update = true;} //手前
			
			/** 奥行きの設定
			 * @param [in] float _far	:カメラで見る奥行きの表示範囲
			 **/
			void setFar( float _far ){ camera_far = _far; 	is_update = true;} //奥

			//--------------------get----------------------------------------
			/** カメラの座標取得
			 * @return pos		:位置
			 **/
			math::Vector3 &getPosition(){ return pos; }
			
			/** 表示範囲の取得 */
			/** 手前取得
			 * @return camera_near	:手前
			 **/
			float getNear(){ return camera_near; }
			
			/** 奥行き取得
			 * @return camera_far	:奥行き
			 **/
			float getFar(){ return camera_far; }
			
			//移動量の取得
			math::Vector3 &getMove(){ return move; }

			/** 見ている先の取得 */
			math::Vector3 &getAt(){ return at; } 
			
			/** 行列の"Z軸"取得 */
			math::Vector3 getView_axis(){ return math::Vector3( camera.m02,camera.m12,camera.m22 ); }
			
			/** 行列の"Y軸"取得 */
			math::Vector3 getUp_axis(){ return math::Vector3( camera.m01,camera.m11,camera.m21 ); }
			
			/** 行列の"X軸"取得 */
			math::Vector3 getRight_axis(){ return math::Vector3( camera.m00,camera.m10,camera.m20 ); }

		};
	}
}