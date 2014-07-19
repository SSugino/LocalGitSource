/** @file
 * 演出クラスの定義
 * @date 2009/08/27
 * @author 杉野
 **/

#pragma once
#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

namespace sugino{
namespace game{
	/** ポリラインクラス(３Ｄ空間のライン) */
	class Poly{

	public:
		int PolyLine( graphics::IRender *render,math::Vector3 &s,math::Vector3 &e,float w,math::Vector3 &cpos, math::Matrix4x4 &vps );

		void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,
			math::Vector2 &t1,math::Vector2 &t2,math::Vector2 &t3,math::Vector2 &t4,math::Matrix4x4 &vps );
	};

	/** ポリリストクラス */
	class PolyList{
	private:
		struct PolyPoint{
			math::Vector3 v1,v2;		//点
			math::Color		c;			//色
		};
		std::list< PolyPoint >	plist;	//v1,v2 二点のリスト
	public:
		/** リストに点を追加 */
		void addPoint( math::Vector3 &p1, math::Vector3 &p2, math::Color &col );

		void Move();

		void Draw( graphics::IRender *render,math::Matrix4x4 &vps);
		

		static void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,
			math::Vector2 &t1,math::Vector2 &t2,math::Vector2 &t3,math::Vector2 &t4,math::Matrix4x4 &vps );

		static void drawSquare( graphics::IRender *render,math::Vector3 &v1,math::Vector3 &v2,math::Vector3 &v3,math::Vector3 &v4, DWORD col,math::Matrix4x4 &vps );

	};
}}