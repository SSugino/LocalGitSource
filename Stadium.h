/** @file
 * スタジアムクラスの定義
 * @date 2009/08/28
 * @author 杉野
 **/
#pragma once

#include "../Graphics/IRender.h"
#include "../System/Template.h"
#include "../game/Object3D.h"
#include "../game/Collision.h"
#include "../game/Camera3D.hpp"
#include "Player.h"
#include "Ball.h"
#include "Rule.h"
#include "Bat.h"

namespace sugino{
/** 野球版の領域 */
namespace baseball{
	
	//仮定義
	class Ground;
	class Rule;
	class PlayerBase;
	class BallBase;
	class Bat;

	/** スタジアムの基底クラス
	 * @note	ベースや観客など
	 **/
	class StadiumBase
	{
	protected:
		/** 座標 */
		math::Vector3		pos;
		
		/** スケーリング */
		math::Vector3		scale;

		/** 回転 */
		float				rot;

		/** モデル */
		game::Model3D		*model;

		/** マネージャー管理の選手のテクスチャイメージを参照 */
		graphics::IImage	*img;

		
	public:
		/** コンストラクタ
		 * @pragma [in] _pos		: 座標
		 * @pragma [in] _scale		: 縮尺
		 * @pragma [in] _rot		: 角度
		 * @pragma [in] _model		: モデルデータ
		 * @pragma [in] _img		: テクスチャデータ
		 **/
		StadiumBase( math::Vector3 &_pos, math::Vector3 &_scale, float _rot, game::Model3D *_model, graphics::IImage *_img )
		{
			pos		= _pos;
			scale	= _scale;
			rot		= _rot;
			model	= _model;
			img		= _img;
		}

		/** デストラクタ */
		virtual ~StadiumBase(){}

		/** 初期化 */
		virtual void Init()=0;			 

		/** メイン処理 */
		virtual void Main()=0;

		/** 表示
		 * IRender	*r				:レンダー
		 * Matrix4x4	&vps		:掛け合わせたカメラ行列
		 **/
		virtual void Draw( graphics::IRender *r,math::Matrix4x4 &vps )=0;

		//--------- Set ------------
		/** 座標のセット
		 * @param [in]	Vector3	_pos	:3次元座標
		 **/
		void setPos( math::Vector3 &_pos ){ pos = _pos; }

		/** スケールのセット
		 * @param [in]	Vector3	_scale	:スケールサイズ
		 **/
		void setScale( math::Vector3 &_scale ){ scale = _scale; }
		
		/** 回転量のセット
		 * @param [in]	float _rot		:回転量
		 **/
		void setRot( float _rot ){ rot = _rot; }

		//--------- Get ------------
		/** 3次元座標の取得
		 * @return pos					:3次元座標
		 **/
		math::Vector3	getPos(){ return pos; }
		
		/** スケールサイズの取得
		 * @return scale				:スケーリングサイズ
		 **/
		math::Vector3	getScale(){ return scale; }

		/** 回転量の取得
		 * @return rot					:回転量
		 **/
		float			getRot(){ return rot; }


		/** 接触判定
		 * @param [out] BallBase *bbase	:ボールクラス
		 */
		virtual void Collision( Ball *bbase ) = 0;
	};
	//----------------------------------------------------------------------------------------------------
	
	/** スタジアム(球場）管理クラス */
	class StadiumManager{
	private:
		/** 基底クラスを保持 */
		std::list<StadiumBase *>			stadium;			

		/** ベースのモデルを保持 */
		game::Model3D						mdl_base;
		
		/** 観客席のモデルを保持 */
		game::Model3D						mdl_audi;

		/** 派生先の画像をここで管理 */
		system::AutoPtr<graphics::IImage>	img_base;
		system::AutoPtr<graphics::IImage>	img_audi;

	public:
		/** コンストラクタ */
		StadiumManager(){
			img_base	= NULL;
			img_audi	= NULL;
		}
		
		/** デストラクタ/解放 */
		~StadiumManager(){ Dispose(); }
		
		/** Baseを必要分初期化
		 * @note new-sでStadiumBase派生型を初期化していく
		 **/
		void Init();

		/** メイン
		 * @param [out]	Rule		*rule	:ルール
		 * @param [out] Ground		*g		:グラウンドクラス
		 * @param [out] Ball		*ball	:ボールクラスのクラス
		 **/
		void Main( Rule *rule,Ground *g,Ball *_ball );
		
		/** 表示
		 * @param [out]	IRender		*render	:レンダー
		 * @param [out]	Matrix4x4	&vps	:ビュー*プロジェクション*スクリーンを掛け合わせた行列　
	 	 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** リストの解放 */
		void Dispose();

		//get-------------------------------------------------------------
		/** リストの取得
		 * @return	基底クラスのリスト
		 **/
		std::list< StadiumBase *> &getStadium(){ return stadium; }
			
		//set-------------------------------------------------------------
		/** リストの追加
		 * @param [in]	Stadiumbase	*_stadium	:基底クラス
		 * @note "一体分"をリストへ追加
		 **/
		void setStadium( StadiumBase *_stadium ){ stadium.push_back(_stadium); }
		
		/** モデルのセット
		 * @param [in] char		*model_name		:モデルの名前
		 **/
		void setBaseModel( char *model_name ){ mdl_base.Load(model_name); }

		/** テクスチャの一括セット
		 * @param [out]	IRender *render			:レンダー
		 * @param [in]	char	*base_img		:塁のテクスチャ
		 * @param [in]	char	*audi_img		:観客席のテクスチャ
		 **/
		void setImage( graphics::IRender *render,char *base_img,char *audi_img )
		{
			img_base	= render->createImage( base_img );
			img_audi	= render->createImage( audi_img );
		}

		/** モデルの一括セット */
		void setModel( char *base_model,char *audi_model )
		{
			mdl_base.Load( base_model );
			mdl_audi.Load( audi_model );
		}
	};

	//---------------------------------------------------------------------------
	//		StadiumBase派生クラス
	//---------------------------------------------------------------------------
	
	/** ベース（塁）用クラス(StadiumBase派生) */
	class Base : public StadiumBase
	{
	private:

		/** 行列変換データを保持 */
		math::Matrix4x4						mat_base,mat_r,mat_sc,mat_t,mat_init;
		
		/** 回転量を保持 */
		float								rot;
		
		/** 重力値を保持 */
		float gravity;

		/** 行列の計算 */
		void calMatrix();
		
		/** 判定を保持 */
		game::BoundingOBox3D				bobox;
	public:
		/** 初期化 **/
		Base( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,game::Model3D *_model, graphics::IImage *_img, float _gravity) :
		StadiumBase( _pos,_scale,_rot,_model,_img )
		{
			rot		= _rot;
			gravity = _gravity;
			//判定
			mat_r.RotateY( _rot );
			bobox	= game::BoundingOBox3D( _pos,math::Vector3(_scale.x*0.5f,_scale.y*0.15f,_scale.z*0.5f),mat_r );			
		};
		
		void Init()
		{
			calMatrix();
		}

		/** ランナーに関するメイン */
		void Main();
	
		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		 /** 接触判定
		  * @param [out]	Ball	*bbase	:ボールクラス
		  **/
		 virtual void Collision( Ball *bbase );
	};
	//-----------------------------------------------------------------
	/** 観客席用クラス(StadiumBase派生) */
	class Auditorium : public StadiumBase
	{
	private:

		/** 行列変換データを保持 */
		math::Matrix4x4						mat_audi,mat_r,mat_sc,mat_t,mat_init;
		
		/** 回転量を保持 */
		float								rot;
		
		/** 行列の計算 */
		void calMatrix();
		
	public:
		/** 初期化 **/
		Auditorium( math::Vector3 &_pos,math::Vector3 &_scale,float _rot,game::Model3D *_model, graphics::IImage *_img) :
		StadiumBase( _pos,_scale,_rot,_model,_img )
		{
			rot = _rot;
		};
		
		/** 初期化 */
		void Init()
		{
			calMatrix();
		}

		/** メイン */
		void Main();
	
		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** 当たり判定
		 * @param [out]	Ball	*bbase	:ボールクラス
		 **/
		 virtual void Collision( Ball *bbase ){};
	};

	//-------------------------------------------------------------------------------------
	/** グラウンド（地面)クラス */
	class Ground
	{
	private:
		int se_foul;

		/** 背景のテクスチャイメージを保持 */
		system::AutoPtr<graphics::IImage>	img_ground;		//地面
		system::AutoPtr<graphics::IImage>	img_wall;		//壁
		
		/** モデルを保持 */
		game::Model3D						model;

		/** ストライクゾーンの判定情報を保持 */
		game::BoundingBox3D					bbox;

		/** 結果が得られる壁の判定を保持 */
		game::BoundingOBox3D				bobox1;			//左翼2BH
		game::BoundingOBox3D				bobox2;			//左中間1BH
		game::BoundingOBox3D				bobox3;			//左中間3BH
		game::BoundingOBox3D				bobox4;			//左中間OUT
		game::BoundingOBox3D				bobox5;			//レフト側ホームラン	
		game::BoundingOBox3D				bobox6;			//ライト側ホームラン
		game::BoundingOBox3D				bobox7;			//右中間OUT
		game::BoundingOBox3D				bobox8;			//右中間3BH
		game::BoundingOBox3D				bobox9;			//右中間1BH
		game::BoundingOBox3D				bobox10;		//右翼2BH
		/** */
		void ResultInit();									//配置などの初期化
		/** 判定用の行列 */
		math::Matrix4x4						mat_osc[10],mat_rot[10],mat_obox[10];

		/** 行列変換データを保持 */
		math::Matrix4x4						mat_sc,mat_ground;	//sc:初期化時のscale行列 mat:Draw時の移動行列
		
		/** マップ座標を保持 */
		math::Vector3						pos;

		/** 地面の判定する為に必要なものを保持 */
		game::ModelFace						*mf;
		
		/**　判定用のレイを保持 */
		math::Vector3						ray;
		
		/** 結果の状態を保持
		 * @note 0:何もなし(通常ヒット等）	1:ファール	2:結果の壁
		 **/
		int									result_state;

		/** 接触判定
		 * @param [in]	Object3D	&map	:マップモデルのオブジェクト 
		 * @param [in]	Vector3		&pos	:判定させたいオブジェクトの座標
		 * @param [in]	Vector3		&rey	:判定させたいレイの方向
		 * @param [in]	float		size	:判定させる為の半径サイズ
		 * @param [in]	ModelFace   **f		:マップの面情報　メンバのmf
		 * @param [in]	Vector3		*ret	:レイの交点の位置
		 * @param [in]	Vector3		*norm	:法線ベクトルの位置
		 * @return		true:当たった	false:当たってない
		 **/
		bool Intersect( game::Object3D &map, math::Vector3 &pos, math::Vector3 &ray,float size,game::ModelFace **f,math::Vector3 *ret, math::Vector3 *norm );
	
		/** 球と壁との接触判定
		 * @param [in]	Object3D	&map	:マップモデルのオブジェクト
		 * @param [in]	Vector3		&pos1	:球の中心座標
		 * @param [in]  float		size	:判定させるための範囲サイズ
		 * @param [out] Vector3		*pos	:判定後の補正座標
		 * @note	関数内で座標を補正
		 **/
		bool IntersectSideWall( game::Object3D &map,math::Vector3 &pos1,float size,math::Vector3 *pos,math::Vector3 *dir);
	
		/** 球と面との位置関係判定
		 * @param [in]	Object3D	&map	:マップモデルのオブジェクト
		 * @param [in]	Vector3		&pos	:球の中心座標
		 * *	return	true:面の裏		false:面の表	
		 * @note	球が面に対してどの位置にあるかで判定を行う
		 **/
		bool IntersectFoul( game::Object3D &map,math::Vector3 &pos );
	
		bool						one_hit;			//ストライク時一回のみ判定フラグ

		/** ローカル座標をグローバル座標に変換 */
		void TransformVector( int num,math::Vector3 local );
			
		/** 球とカベとの判定
		 * @param [in]	Object3D	&map	:マップモデルのオブジェクト
		 * @param [in]	Vector3		&pos	:球の中心座標
		 * @param [in]	float		size	:判定サイズ
		 * @param [out]	ModelFace	**f		:モデルの面
		 * @param [out] Vector3		*r		:交点
		 * @param [out]	Vector3		*n		:法線 
		 **/
		bool IntersectWall( game::Object3D &map,math::Vector3 &pos,float size,game::ModelFace**f,math::Vector3 *r,math::Vector3 *n );

	public:
		Ground()
		{
			result_state = 0;
		}

		/** 初期化
		 * @param [in]	char*		model_name		: 読み込むモデルファイル名
		 * @param [in]	char*		ground_img		: レンダーのcreateImageで作成したデータ
		 * @param [in]  char*		wall_img		: レンダーのcreateImageで作成したデータ
		 **/
		void Init( char *model_name, graphics::IImage *ground_img, graphics::IImage *wall_img );

		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );
		
		/** メイン
		 * @param [in]	BallBase	*bbase		:ボール派生先
		 * @param [out]	Bat			*_bat		:バットクラス
		 * @param [in]	Rule		*_rule		:ルールクラス
		 **/
		void Main( Ball *_ball,Bat *_bat, Rule *_rule );
		
		/** ストライクゾーン判定
		 * @param [out]	BallBase	*bbase		:ボール派生先	
		 * @return	true:	ストライクゾーン通過	false:通過してない＝ボール
		 **/
		bool StrikeJudge( Ball *bbase );
		
		/** 各基底クラスとその派生先関数の判定処理
		 * @param [out]	Rule		*rule		:ルールクラス
		 * @param [out]	StadiumBase	*base1		:スタジアム基底クラス
		 * @param [out]	PlayerBase	*base2		:プレイヤー基底クラス
		 * @param [out] BallBase	*base3		:ボールクラス
		 * @note	各マネージャのメインで呼ばれる関数なので引数は一気に渡さずに各基底以外はＮＵＬＬで渡す
		 **/
		void IntersectFunction( Rule *rule,StadiumBase *base1,PlayerBase *base2,Ball	*base3);

		/** 配置座標取得(カメラの初期化時に使用) */
		math::Vector3 &getPos(){ return pos; }
		
		//------- set	/	get ------------
		/** 一回処理フラグの取得 */
		bool getOnehitFlag(){ return one_hit; }
		
		/** 一回処理フラグのセット */
		void setOnehitFlag( bool _one_hit ){ one_hit = _one_hit; }
		
		/** グラウンドで結果状態のセット
		 * @param [in]	int		state	:状態
		 * @note	0:何もない	1:ファール状態
		 **/
		void setResultState( int state ){ result_state = state; }
		
		/** グラウンドで得られる結果の取得
		 * @return	result_state	:グラウンドで得られる状態	0:なにもない	1:ファール
		 **/
		int getResultState(){ return result_state; }
			
		/** フェンス判定の取得
		 * @param [out]	int	num	:どのフェンスの判定か
		 * @note 0:使用しない	1:レフト2BH判定 2:レフト1BH判定 3:レフト3BH判定
		 * 4:レフトアウト判定	5:レフトホームラン判定 6:ライトホームラン判定
		 * 7:ライトアウト判定	8:ライト3BH判定 9:ライト1BH判定
		 * 10:ライト2BH
		 * @return	フェンスの判定
		 **/
		game::BoundingOBox3D	getBoundingObj( int num );
		
		/** フェンスの接触判定
		 * @param [out] Ball	*bbase	:ボールクラス
		 * @return	進塁条件 0:アウトに当たった 1:シングルヒットに当たった 
		 * 2:ツーベースヒットに当たった 3:スリーベースヒットに当たった
		 * 4:ホームランに当たった
		 **/
		int Collision( Ball *bbase );

	};

	//-----------------------------------------------------------------------------
	/** スカイドームクラス */
	class SkyDome
	{
	private:
		/** テクスチャを保持 */
		system::AutoPtr<graphics::IImage>	img_sky;		//空
		
		/** モデルを保持 */
		game::Model3D						model;
	
		/** 行列変換データを保持 */
		math::Matrix4x4						mat_sc,mat_rot;	//mat_sc:初期化時のscale行列/mat_rot回転
		
		/** マップ座標を保持 */
		math::Vector3						pos;

		/** 回転値(結果)を保持 */
		float								rot;
		
		/** 回転量を保持 */
		float								rot_val;

	public:
		/** 初期化
		 * @param [in] name			: 読み込むモデルファイル名
		 * @param [in] sky_img	: レンダーのcreateImageで作成したデータ
		 * @param [in] sky_img2	: レンダーのcreateImageで作成したデータ
		 **/
		void Init( char *model_name, graphics::IImage *sky_img );
		
		/** 計算など */
		void Main();
		/** 表示
		 * @param [in] render : IRenderクラスを派生したクラス
		 * @param [in] camera : ビュー*プロジェクション*スクリーンを掛け合わせた行列
		 **/
		void Draw( graphics::IRender *render, math::Matrix4x4 &vps );

		/** 回転度数にセットしなおす
		 *	@param [in] float speed		:回転速さ 0～1 
		 **/
		void setRotate( float speed ){ rot_val = speed; }
	};
}}