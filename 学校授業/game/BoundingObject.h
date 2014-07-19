/** @file
 * 各バウンティング(衝突)オブジェクトの表示クラスの定義
 * @date 2009/06/05
 * @author 杉野
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../math/Matrix.h"
#include "../math/Color.h"
#include "Collision.h"
#include "Object3D.h"

namespace sugino{
/** ゲームの領域 */
namespace game{

	class Object3D;//仮定義

	/** バウンディングオブジェクト仮表示クラス */
	class BoundingObject{
	private:
		static Object3D box,sphere;

	public:
		/** 円描画
		 * @param [out]		IRender			*r	:表示したいレンダリング 
		 * @param [in] BoundingCircle2D		&b	:衝突判定元
		 * @param [in]		Matrix3x3		&m	:行列
		 * @param [in]		DWORD			col	:色
		 **/
		static	void Draw( graphics::IRender *render,BoundingCircle2D &bobj,math::Matrix3x3 &m,DWORD col );
		/**	線描画
		 * @param [out]		IRender			*r	:表示したいレンダリング 
		 * @param [in] BoundingCircle2D		&b	:衝突判定元
		 * @param [in]		Matrix3x3		&m	:行列
		 * @param [in]		DWORD			col	:色
		 **/
		static	void Draw( graphics::IRender *render,BoundingLine2D &bobj,math::Matrix3x3 &m,DWORD col );
		/** AABB描画
		 * @param [out]		IRender			*r	:表示したいレンダリング 
		 * @param [in] BoundingCircle2D		&b	:衝突判定元
		 * @param [in]		Matrix3x3		&m	:行列
		 * @param [in]		DWORD			col	:色
		 **/
		static	void Draw( graphics::IRender *render,BoundingBox2D &bobj,math::Matrix3x3 &m,DWORD col );
		/** OBB描画
		 * @param [out]		IRender			*r	:表示したいレンダリング 
		 * @param [in] BoundingCircle2D		&b	:衝突判定元
		 * @param [in]		Matrix3x3		&m	:行列
		 * @param [in]		DWORD			col	:色
		 **/
		static	void Draw( graphics::IRender *render,BoundingOBox2D &bobj,math::Matrix3x3 &m,DWORD col );
	
		//----------------------------------------------------------------------------------
		
		static  void Draw( BoundingSphere3D &sp,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingBox3D &bbox,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingOBox3D &obox,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );
		static  void Draw( BoundingLine3D &bl,graphics::IRender *render,math::Matrix4x4 &vps, math::Vector3 &dir,math::Color &col );

	};

}}