/** @file
 * エフェクトクラスの定義
 * @date 2009/06/19
 * @author 杉野
 **/
#pragma once


#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../graphics/IRender.h"
#include "Object2D.h"

namespace sugino{
namespace game{

	/** エフェクト基底クラス */
	class EffectBase
	{
	private:

	protected:
		/** 座標 */
		math::Vector2 pos;
		
		/** サイズ */  
		math::Vector2 scale;		//xとy別々拡大
		
		/** 等倍サイズ */
		float size;
		/** 回転 */
		float rot;

		/** カウント */
		int cnt;
	public:
		virtual ~EffectBase(){}											//デストラクタ
		/** コンストラクタ
		 * @param [in] Vector2 &p	:初期座標
		 * @param [in] int	   count:消去カウント
		 * @param [in] float	r	:角度
		 **/
		EffectBase( math::Vector2 &p,int count,float r ) : pos(p),scale(1,1),size(0),rot(r),cnt(count){}	//コンストラクタ
		/** 仮想表示 */
		virtual bool Anim();
		/** 表示
		 * @param [in] IRender		*render	:表示元画像
		 * @param [in] Model2D		&model	:モデル
		 * @param [in] matrix3x3	&m		:カメラ行列
		 **/
		virtual void Draw( graphics::IRender *render,Model2D &model,math::Matrix3x3 &m );
		/** セッター関数 */
		void setScale( math::Vector2 &scal ){ scale	=scal; }
		void setSize( float s ){ size = s; }
		void setRotate( float r ){ rot = r; }
		void setCnt( int count ){ cnt = count; }
		/** ゲッター関数 */
		math::Vector2 getPos(){ return pos; }
		math::Vector2 getScale(){ return scale; }
		float getSize(){ return size; }
		float getRotate(){ return rot; }
		int getCount(){ return cnt; }
	};
	
	/**　エフェクトを管理するクラス */
	class EffectManager
	{
	private:
		/** リストをポインタ型で管理 */
		std::list< EffectBase* > effect;
		/** エフェクトに持たす */
		Model2D model;
		/** テクスチャも持たす */
		system::AutoPtr<graphics::IImage>	tex;//イメージ（テクスチャ）も外部かここかも自由
	public:
		~EffectManager() { Dispose(); }
		/** 初期化
		 * @param [in] Vector2 &p	:初期化座標
		 * @param [in] int	count	:消去カウント
		 * @param [in] float	r	:角度
		 **/
		void Init( math::Vector2 &p,int count,float r );
		/** モデルとテクスチャの読み込み
		 * 読み込む前に"ZD"でFaceにテクスチャが貼ってあるか確認
		 * @param [in] char *model_name		: モデルファイル名
		 * @param [in] const char *tex_name	: テクスチャファイル名
		 * @param [in] IRender *render		: レンダー
		 **/
		void Load( char *model_name,const char *tex_name,graphics::IRender *render );
		void Anim();
		/** 表示
		 * @param [in] IRender	*render		:表示させたい画像元
		 * @param [in] Matrix3x3	&m		:カメラ行列
		 **/
		void Draw( graphics::IRender *render,math::Matrix3x3 &m );					//モデルをManager
		/** 解放 */
		void Dispose();
	};
}}