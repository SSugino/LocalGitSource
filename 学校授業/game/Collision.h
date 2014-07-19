/** @file
 * バウンティング(衝突)クラスの定義
 * @date 2009/06/05
 * @author 杉野
 **/

#pragma once

#include "../graphics/IRender.h"
#include "../math/Vector.h"
#include "../math/Float.h"
#include "../math/Matrix.h"
#include "../math/Color.h"

namespace sugino{
namespace game{
	/** バウンディングオブジェクトの定義 */

	/** バウンディングサークル */
	struct BoundingCircle2D
	{
		/** 中心点 */
		math::Vector2 p;
		/** 半径 */
		float	s;
		BoundingCircle2D(){}
		/** 初期化
		 * @param[in] Vector2	&v	:位置ベクトルを中心点をして渡す
		 * @param[in] float		_s	:半径
		 **/
		BoundingCircle2D( math::Vector2 &v,float _s ) : p(v),s(_s) {}
	};
	
	/** バウンディングライン */
	struct BoundingLine2D
	{
		/** 始点・終点 */
		math::Vector2 s,e;

		BoundingLine2D(){};
		/** 初期化
		 * @param[in] Vector2	&st	:始点ベクトル
		 * @param[in] Vector2	&ed	:終点ベクトル
		 **/
		BoundingLine2D( math::Vector2 &st,math::Vector2 &ed ){ s=st; e=ed ;}
	};

	/** バウンディングボックス( Axis_Aligh_Bounding_Box ) AABB */
	struct BoundingBox2D
	{
		/** 中心点 */
		math::Vector2 p;
		/** 大きさ */
		math::Vector2 s;

		BoundingBox2D(){}
		/** 初期化
		 * @param[in] Vector2	&center	:中心ベクトル
		 * @param[in] Vector2	&size	:サイズベクトル
		 **/
		BoundingBox2D( math::Vector2 &center,math::Vector2 &size ){ p=center, s=size; }
	};
	
	/** バウンディング回転ボックス( Oriented_Bounding_Box ) OBB */
	struct BoundingOBox2D
	{
		/** 中心点 */
		math::Vector2 p;
		/** 大きさ */
		math::Vector2 s;
		/** 回転軸 Y軸になる */
		math::Vector2 r;
		
		BoundingOBox2D(){}
		/** 初期化
		 * @param[in] Vector2	&center	:中心ベクトル
		 * @param[in] float		rot		:Y軸(単位ベクトル)
		 * @param[in] Vector2	&size	:サイズのベクトル
		 **/
		BoundingOBox2D( math::Vector2 &center,float rot,math::Vector2 &size )
		{ 
			p = center;
			r = math::Vector2( rot ).Rotate90();
			s = size;
		}

		/** 初期化
		 * @param[in] Vector2	&center	:中心ベクトル
		 * @param[in] Vector2	rot		:Y軸ベクトル ( [0,1]とかで渡す)	(単位ベクトル)
		 * @param[in] Vector2	&size	:サイズのベクトル
		 **/
		BoundingOBox2D( math::Vector2 &center,math::Vector2 rot,math::Vector2 &size )
		{
			p = center;
			s = size;
			r = rot;
		}
	};
	
	/** 2Dの接触判定クラスの定義 */
	class Collision2D
	{
	public:
		/** 接触判定 円と円
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingCircle2D &b );
		/** 接触判定 四角と四角
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox2D &a,BoundingBox2D &b );
		/** 接触判定 線と線
		 * return true : Hit
		 **/
		static bool Collision( BoundingLine2D &a,BoundingLine2D &b );
		/** 接触判定 円と線
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingLine2D &b );
		/** 接触判定 円と四角 (AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingBox2D &b );
		/** 接触判定 円と回転箱 (OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingCircle2D &a,BoundingOBox2D &b );
		/** 接触判定 回転箱と回転箱 (OBB*OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox2D &a,BoundingOBox2D &b );
		/** 接触判定 回転箱と四角(OBB*AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox2D &a,BoundingBox2D &b );
		
	};

	/** 交差(押し返す)判定クラスの定義 */
	class Intersect2D
	{
	public:
		/** 交差判定 円と円
		 * return true : Hit
		 * @param[out] :
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingCircle2D &b,math::Vector2 *ret );
		/** 交差判定 四角と四角
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox2D &a,BoundingBox2D &b,math::Vector2 *ret );
		/** 交差判定 円と線
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingLine2D &b,math::Vector2 *ret );
		/** 交差判定 円と四角(AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingBox2D &b,math::Vector2 *ret );
		/** 交差判定 円と回転箱(OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingCircle2D &a,BoundingOBox2D &b,math::Vector2 *ret );
		/** 交差判定 回転箱と回転箱(OBB*OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox2D &a,BoundingOBox2D &b,math::Vector2 *ret );
		/** 交差判定 回転箱と四角(OBB*AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox2D &a,BoundingBox2D &b,math::Vector2 *ret );

	};

	//--------------------------------------------------------------------------------------
	/** 球体の3D当たり判定 */
	class BoundingSphere3D
	{
	public:
		math::Vector3	p;	//中心点
		float			s;	//大きさ
		BoundingSphere3D(){}
		BoundingSphere3D( math::Vector3 &center,float size ){ p=center; s=size; }
	};

	/** 箱の3D当たり判定 */
	class BoundingBox3D
	{
	public:
		math::Vector3 p;	//中心点
		math::Vector3 s;	//大きさ
		BoundingBox3D(){}
		BoundingBox3D( math::Vector3 &center,math::Vector3 &size ){ p=center;s=size; }
	};

	/** 回転箱の３Ｄ当たり判定 */
	class BoundingOBox3D
	{
	public:
		math::Vector3 p,s;	//中心点、大きさ
		math::Vector3 ax,ay,az;	//各軸
		BoundingOBox3D(){}
		BoundingOBox3D( math::Vector3 &c,math::Vector3 &size,math::Matrix4x4 &rot ){
			p = c;s = size;
			ax.x = rot.m00;ax.y = rot.m01; ax.z = rot.m02;
			ay.x = rot.m10;ay.y = rot.m11; ay.z = rot.m12;
			az.x = rot.m20;az.y = rot.m21; az.z = rot.m22;
		}
	};

	/** 線（カプセル型)の３Ｄ当たり判定 */
	class BoundingLine3D
	{
	public:
		math::Vector3 s,e;	//始点　終点
		float		width;	//幅

		BoundingLine3D(){}
		BoundingLine3D( math::Vector3 &st,math::Vector3 &ed,float w )
		{
			s = st;e = ed;width = w;
		}
	};
	
	//----------------------------------------------------------------------------

	/** 3Dの接触判定クラスの定義 */
	class Collision3D
	{
	public:
		/** 接触判定 球と球
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingSphere3D &b );
		/** 接触判定 四角と四角
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox3D &a,BoundingBox3D &b );
		/** 接触判定 球と線
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingLine3D &b );

		/** 接触判定 球とレイ（光線)
		 * @param [in] BoundingSphere &a,	判定用球体
		 * @param [in] Vector3		  &pos,	光線開始位置
		 * @param [in] Vector3		  &ray,	光線
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,math::Vector3 &pos,math::Vector3 &ray );
		
		/** 接触判定 球と四角 (AABB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingBox3D &b );
		
		/** 接触判定 球と回転箱 (OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingSphere3D &a,BoundingOBox3D &b );
		
		/** 接触判定 回転箱と回転箱 (OBB*OBB)
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox3D &a,BoundingOBox3D &b );
		
		/** 接触判定 四角とレイ(AABB*Ray)
		 * @param [out]	Vevtor3	&ray	:レイの方向
		 * return true : Hit
		 **/
		static bool Collision( BoundingBox3D &a,math::Vector3 &pos,math::Vector3 &ray );

		/** 接触判定 回転四角とレイ(OBB*Ray)
		 * @param [out]	Vector3	&pos1	:光線の座標
		 * @param [out] Vector3	&ray1	:レイの方向
		 * return true : Hit
		 **/
		static bool Collision( BoundingOBox3D &a,math::Vector3 &pos1,math::Vector3 &ray1 );
	
		/** 判定する軸と軸上の距離を指定して、2つのOBBが衝突しているか調べる
		 * @param [in] Vector3	&axis		:任意の軸
		 * @param [in] Vector3	&dist		:長さ
		 * @param [in] BoundingOBox3D &a	:ＯＢＢ１
		 * @param [in] BoundingOBox3D &b	:ＯＢＢ２
		 * @param [out] float *length		:補正量
		 * @param [out]	float *sign			:補正方向
		 * @return length					:補正量
		 **/
		static bool AxisLength( math::Vector3 &axis, math::Vector3 &dist, BoundingOBox3D &a, BoundingOBox3D &b,float *length,float *sign );

	};

	/** 交差(押し返す)判定クラスの定義 */
	class Intersect3D
	{
	public:
		/** 交差判定 球と球
		 * @param [in] Vector3	*ret	:補正量	
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingSphere3D &b,math::Vector3 *ret );
		
		/** 交差判定 四角と四角
		 * @param [in] Vector3	*ret	:補正量
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox3D &a,BoundingBox3D &b,math::Vector3 *ret );
		
		/** 交差判定 球と線
		 * @param [in] Vector3	*ret	:補正量
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingLine3D &b,math::Vector3 *ret );
		
		/** 交差判定 球と線
		 * @param [in]	Vector3	*ret			:補正量
		 * @param [in]	Vector3	*cross_pos		:交点
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingLine3D &b,math::Vector3 *ret,math::Vector3 *cross_pos );

		/** 交差判定 球とレイ(光線)
		 * @param [in] BoundingSphere3D		&a			:判定対象の球
		 * @param [in] Vector3				&pos		:光線開始座座標
		 * @param [in] Vector3				&ray		:光線
		 * @param [in] Vector3				*cross_pos	:交点	※補正座標でない
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,math::Vector3 &pos,math::Vector3 &ray,math::Vector3 *cross_pos );
		
		/** 交差判定 円と箱(AABB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingBox3D &b,math::Vector3 *ret );
		
		/** 交差判定 球と回転箱(OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingSphere3D &a,BoundingOBox3D &b,math::Vector3 *ret );
		/** 交差判定 回転箱と回転箱(OBB*OBB)
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox3D &a,BoundingOBox3D &b,math::Vector3 *ret );
		
		/** 交差判定 四角とレイ
		 * return true : Hit
		 **/
		static bool Intersect( BoundingBox3D &a,math::Vector3 &pos,math::Vector3 &ray,math::Vector3 *ret );
		
		/** 交差判定 回転四角とレイ
		 * return true : Hit
		 **/
		static bool Intersect( BoundingOBox3D &a,math::Vector3 &pos1,math::Vector3 &ray1,math::Vector3 *ret );

	};

	/** 視錐台カリング（画面内にオブジェクトがあるか？/デバッグでの範囲確認用） */
	class Frustum{
	public:
	
		math::Vector3	p[8];		//nearとfarの4点×2
		math::Plane pnear,pfar;		//手前、奥
		math::Plane left,right;		//左、右
		math::Plane top,bottom;		//上、下
		
		//Frustum( math::Vector3 &point,
		
		/** 視錐台の台形作成
		 * @param [in] Matrix4x4	&view		:カメラの見てる行列
		 * @param [in] float		wid			:幅
		 * @param [in] float		hei			:高さ
		 * @param [in] float	near_len		:手前の長さ
		 * @param [in] float	far_len			:奥の長さ
		 **/
		void Create( math::Matrix4x4 &view,float wid,float hei,float near_len,float far_len );	//初期化

		void Draw( graphics::IRender *r,math::Matrix4x4 &vps,math::Vector3 &dir,math::Color &col );
		
		/** フラスタムの接触判定 */
		bool Collision( BoundingSphere3D &sp );
		

		bool Collision( BoundingOBox3D &obox );

	};
}}

