/** @file
 * シーンのクラスの定義
 * @date 2009/06/21
 * @author 杉野
 **/

#pragma once


#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "../graphics/IRender.h"
#include "../input/Input.h"

namespace sugino{
namespace game{

	class SceneManager;
	class ISceneEffect;

	/** シーンの基底クラス */
	class SceneBase
	{
	protected:
		/** どのシーンでも共通はどんどん追加していく
		 *	例キーとか　メインから必要な変数とかをここでも受け取る */
		/** 画像元 */
		graphics::IRender	*render;
		/** シーン */
		SceneManager		*scene;
		/** キー */
		input::Key			*key;
		/** マウス */
		input::Mouse		*mouse;
	public:
		/** コンストラクタで画像元代入 */
		SceneBase( graphics::IRender *rend,input::Key *k,input::Mouse *m )
		{
			render = rend;
			key	   =  k;
			mouse  =  m;
		}
		virtual ~SceneBase( ){}
		/** 初期化 */
		virtual void Init()=0;
		/** 計算などメイン */
		virtual void Main()=0;
		/** 表示 */
		virtual void Draw()=0;

		void setManager( SceneManager *sm){ scene = sm; }
	};
	
	/** シーン切り替えの基底クラス */
	class ISceneEffect
	{
	public:
		//派生先で書き換えるので純粋で用意
		virtual ~ISceneEffect(){}
		virtual void Init()=0;
		virtual void Main()=0;
		virtual void Draw()=0;
		/** 演出が終了したか? */	
		virtual bool isEnd()=0;
	};

	/** シーンの管理クラス */
	class SceneManager
	{
	private:
		/** 今のシーン */
		SceneBase *now;
		/** 次のシーン */
		SceneBase *next;
		/** シーンに入ってくるエフェクト */
		ISceneEffect *se_in;
		/** シーンが終わるときのエフェクト */
		ISceneEffect *se_out;
		
		void setNextScene();
	public:
		/** NULL初期化 */
		SceneManager(){ now=NULL;next=NULL;se_in=NULL;se_out=NULL; }
		~SceneManager()
		{
			if( now ) delete now; now=NULL;
			if( next ) delete next; next=NULL;
			if( se_in ) delete se_in; se_in=NULL;
			if( se_out ) delete se_out; se_out=NULL;
		}
		
		/** 各シーンの初期化 */
		void Init();
		/** メイン */
		void Main();
		/** 表示 */
		void Draw();
		/** シーンの変更 初期化も呼ぶ */
		void setScene( SceneBase *sb,ISceneEffect *out,ISceneEffect *in )
		{
			/** 移行中はセットできない/メンバを一度delete */
			if( next ){ delete sb;delete out;delete in;return ; }
			
			if( se_out ){ delete se_out;}	//一度解放
			if( se_in ){ delete se_in;}
			se_out=out;				//フェードアウト処理を
			se_in=in;				//フェードイン処理を
			next  = sb;				//現在のシーンを代入
			if( se_out==NULL )		//フェード処理がないなら
			{
				setNextScene();
			}else se_out->Init();
		}
		/** フェードイン状態を調べる（終了したか？） */
		bool isEffectIn(){ return ( se_out==NULL&&se_in ); }
		/** フェードアウト状態を調べる（終了したか？） */
		bool isEffectOut(){ return ( se_out!=NULL ); }
		/** エフェクト中は～の処理/処理しない */
		bool isEffect(){ return isEffectIn()||isEffectOut() ; } 
	};

	
	/** フェードアウトクラス(透明→濃い色) */
	class FadeOut : public ISceneEffect
	{
	private:
		/** 画像元 */
		graphics::IRender *render;
		/** 塗りつぶしたい色 */
		DWORD col;
		/** アルファ値 */
		float alpha;
		/** 塗りつぶし速度 */
		float speed;
		/** 描画範囲 */
		system::Rect rc;
	public:
		/** 初期化
		 * @param [in] IRender *r	:画像元
		 * @param [in] DWORD	c	:色 0x
		 * @param [in] float   sp	:アルファ値変動スピード	
		 **/
		FadeOut( graphics::IRender *r,DWORD c,float sp )
		{
			render	=r;
			col		=c;
			speed	=sp;
			alpha	=0.0f;	//フェードアウトなので初期値は0
			render->getViewport( &rc );
		}
		/** 初期化 */
		void Init(){}
		/** メイン */
		void Main()
		{
			alpha += speed;
			if( alpha >=1.0 ) alpha = 1.0;	
		}

		/** 表示 */
		void Draw()
		{
			DWORD c = (DWORD)(alpha*255);
			render->drawFill( rc,col|(c<<24) );
		}

		/** 終了したかどうか */
		bool isEnd()
		{
			return alpha >= 1.0;
		}
	};

	/** フェードインクラス（濃い色→透明） */
	class FadeIn : public ISceneEffect
	{
		private:
		/** 画像元 */
		graphics::IRender *render;
		/** 塗りつぶしたい色 */
		DWORD col;
		/** アルファ値 */
		float alpha;
		/** 塗りつぶし速度 */
		float speed;
		/** 描画範囲 */
		system::Rect rc;
	public:
		/** 初期化
		 * @param [in] IRender *r	:画像元
		 * @param [in] DWORD	c	:色 0x
		 * @param [in] float   sp	:アルファ値変動スピード	
		 **/
		FadeIn( graphics::IRender *r,DWORD c,float sp )
		{
			render	=r;
			col		=c;
			speed	=sp;
			alpha	=1.0f;	//フェードアウトなので初期値は0
			render->getViewport( &rc );
		}
		
		/** 初期化 */
		void Init(){}
		/** メイン */
		void Main()
		{
			alpha-= speed;
			if( alpha <=0.0 ) alpha = 0;
		}
		/** 表示 */
		void Draw()
		{
			DWORD c = (DWORD)(alpha*255);
			render->drawFill( rc,col|(c<<24) );
		}
		/** 終了したかどうか */
		bool isEnd()
		{
			return alpha <= 0.0;
		}
	};
}}