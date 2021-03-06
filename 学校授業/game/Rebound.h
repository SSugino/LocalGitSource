/** @file
 * 跳ね返り判定の定義
 * @date 2009/09/30
 * @author 杉野
 **/
#pragma once


#include "Collision.h"
#include "../math/Matrix.h"
#include "../baseball/Ball.h"

namespace sugino{
namespace game{

	/** 跳ね返り判定クラス */
	class Rebound
	{
	public:
		
		/** 跳ね返り判定 ↓↑
		 * @param [in]  a   : 判定したいBoundingSphere3D
		 * @param [in]  b   : 判定したいBoundingOBox3D
		 * @param [out] ret : 座標補正量を格納したVector3クラス
		 * @param [out] rif : 反射行列を格納
		 * @retval true		: 接触している 
		 * @retval false	: 接触していない
		 **/
		static bool Bound( const BoundingSphere3D &a, const BoundingOBox3D &b, math::Vector3 *ret, math::Matrix4x4 *rif );
		
		/** 反射判定
		 * @param [in]  a   : 判定したいBoundingSphere3D
		 * @param [in]  b   : 判定したいBoundingOBox3D
		 * @param [out] ret : 座標補正量を格納したVector3クラス
		 * @param [out] pow : 補正を代入した移動量に与える比率
		 * @retval true  : 接触している 
		 * @retval false : 接触していない
		 **/
		static bool Refrect( BoundingSphere3D &a, BoundingLine3D &b, math::Vector3 *mov );

		static bool Refrect( BoundingSphere3D &a, BoundingOBox3D &b, math::Vector3 *mov );
		/** 見ている先から方向から90度右軸を作る
		 * @param [in] Vector3	&dir	:方向
		 * @param [in] Vector3	&up		:上軸
		 **/
		static math::Vector3 getRightAxis( const math::Vector3 &dir, const math::Vector3 &up ); 
	};
}}