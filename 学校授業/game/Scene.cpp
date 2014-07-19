/** @file
 * シーンクラスの実装
 * @date 2009/08/24
 * @author 杉野
 **/

#include "Scene.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;


void SceneManager::setNextScene()
{
	if( now ){ delete now; }//今のシーンを開放
	now = next;	//次のシーンを今のシーンに
	next = NULL;//次のシーンは無し
	now->setManager( this );
	now->Init();	//シーン開始
	if( se_in )		//フェードイン処理があるなら
	{
		se_in->Init();		//処理させる
	}
}

void SceneManager::Init()
{
	now->Init();
}

void SceneManager::Main()
{
	if( now )now->Main();			//今現在のシーンを開始する
	if( se_out ) se_out->Main();	//アウトエフェクトがあるなら
	else if( se_in ) se_in->Main();		//インエフェクトがあるなら
}

void SceneManager::Draw()
{
	now->Draw();
	if( se_out )//フェードアウト処理がある
	{
		se_out->Draw();
		if( se_out->isEnd() )	//フェードアウト処理終了なら
		{
			delete se_out;		//解放
			se_out=NULL;
			setNextScene();		//次のシーンへ移行させる
		}
	}else if( se_in )
	{		//フェードイン
		se_in->Draw();
		if( se_in->isEnd() )
		{
			delete se_in;
			se_in=NULL;
		}
	}
}


