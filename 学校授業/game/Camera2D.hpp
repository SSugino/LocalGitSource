/** @file 
 * ２Ｄカメラ操作クラス
 *	
 **/
//--------------------------------------------------------
#pragma once

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "Collision.h"

namespace sugino{
	namespace game{

		/** カメラ操作クラス 
		 * 位置・拡大・回転の指定が可能なカメラ
		 * このカメラから得られる行列を使って、表示するものをTransformする
		 * 
		 **/
		class Camera2D{
		private:
			/** Y 軸ベクトル */
			math::Vector2		axis;

			/** 座標位置 */
			math::Vector2		pos;

			/** 倍率 */
			float				zoom;
			
			/** 最小倍率 */
			float				min_zoom;
			
			/** 最大倍率 */
			float				max_zoom;
			
			/** カメラ行列 */
			math::Matrix3x3		camera;

			/** スクリーン行列 */
			math::Matrix3x3		screen;

			/** カメラ×スクリーン */
			math::Matrix3x3		view;

			/** 更新フラグ */
			bool				is_update;
			
			/** カメラ行列の更新 */
			void Update()
			{
				if( is_update ){		//更新フラグたってたら
					camera.View( axis*(1/zoom),pos  );
					view = camera*screen;
					is_update = false;
				}
			}
		public:

			/** コンストラクタ */
			Camera2D() : axis(0,1),pos(0,0),zoom(1) {
				setScreen( 640,480 );
				setZoomRange( 0.01f,100 );	// １/１００倍～１００倍まで
				Update();
			};
			
			/** 拡大縮小範囲の指定 */
			void setZoomRange( float min,float max ){
				min_zoom = min;
				max_zoom = max;
			}
			
			/** スクリーン行列の作成 */
			void setScreen( int w,int h ){ screen.Screen(w,h); is_update = true; }

			/** デストラクタ */
			~Camera2D(){};

			/** 画面回転
			 * @param[in]  r :  回転速度
			 **/
			void addRotation( float r ){ axis = axis.Rotate( r ); is_update = true; } 
			
			/** 画面回転指定 */
			void setRotation( float r ){ axis = math::Vector2(r).Rotate90(); is_update = true; } 
			float getRotation(){ return math::Vector2::Angle(math::Vector2(0,1),axis); }

			/** 画面拡大縮小
			 * @param[in]  z:   倍率速度
			 **/
			void addZoom( float z ){ setZoom( zoom + z ); }
			
			/** ズーム指定 */
			void setZoom( float z ){
				zoom = z;
				// クリップ処理
				if( zoom > max_zoom ) zoom = max_zoom;
				if( zoom < min_zoom ) zoom = min_zoom;
				is_update = true;
			}
			
			/** ズーム取得 */
			float getZoom(){ return zoom; }

			/** 画面移動
			 *	@param[in]	m : 移動量
			 **/
			void addPosition( math::Vector2 &m ){ setPosition( pos + m ); }
			
			/** 位置指定 
			 *	@param[in]	&p : 位置
			 **/
			void setPosition( const math::Vector2 &p ){ pos=p; is_update = true; }
			const math::Vector2 &getPos(){ return pos; }
						
			/** カメラ取得 */
			math::Matrix3x3 &getCameraMatrix(){ Update(); return camera; }
			
			/** スクリーン取得 */
			math::Matrix3x3 &getScreenMatrix(){ return screen; }
			
			/** カメラ*スクリーン取得 */
			math::Matrix3x3 &getMatrix(){ 
				Update();
				return view; 
			}

			/** カメラが回転していない時のＡＡＢＢを取得する */
			game::BoundingBox2D createAABB(){
				float aspect = (screen.m20)/(screen.m21);	//幅÷高さ
				return game::BoundingBox2D( pos,math::Vector2( zoom*aspect,zoom ) );
			}
					
			/** カメラが表示しているＯＢＢを取得する */
			game::BoundingOBox2D createOBB(){
				float aspect = (screen.m20)/(screen.m21);
				return game::BoundingOBox2D( pos,axis,math::Vector2( zoom*aspect,zoom ) );

			}
			
					

		};

	}
}