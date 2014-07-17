/** @file
 * スタジアムクラスの実装
 * @date 2009/08/28
 * @author 杉野
 **/

#include "Stadium.h"
#include "../game/Rebound.h"
#include "../game/BoundingObject.h"
#include <algorithm>

#include "../DxLib.h"

using namespace sugino;
using namespace math;
using namespace baseball;
using namespace game;
using namespace graphics;


//塁の行列計算
void Base::calMatrix()
{
	mat_sc.Scale( scale );
	mat_r.RotateY( rot );
	mat_t.Translate( pos );
	mat_base = mat_r*mat_sc*mat_t;
}

//塁のメイン処理
void Base::Main()
{
	this->pos.y-=gravity;			//重力掛けておく
	calMatrix();
	
}

//塁の描画
/*
IRender		*render	:レンダリングクラス
Matrix4x4	&vps	:カメラ行列
*/
void Base::Draw( IRender *render, Matrix4x4 &vps )
{
	model->getObject(0)->Transform(mat_base, vps);		//取得して変換
	render->setTexture(img);
	model->getObject(0)->Draw( render );						//表示
	render->setTexture( NULL );

}

//塁の接触判定
/*
Ball *bbase	:ボールクラス
*/
void Base::Collision( Ball *bbase )
{
	//派生先のボールから当たり判定の情報を取得
	BoundingSphere3D sphere = bbase->getBoundingObj();
	//バットに当たった判定
	Vector	vret;
	float speed = bbase->getVelosity();
	if( Intersect3D::Intersect( sphere,this->bobox,&vret) )
	{
		bbase->setMass( 0 );						//質量を考えない
		Vector p;
		p = bbase->getPos();
		p += Vector( speed ,speed*1.8f,speed );	//ボールを浮かす
		bbase->setPos( p );
	};
	bbase->setMass( 1.6f );						
}

//観客席の行列計算
void Auditorium::calMatrix()
{
	mat_sc.Scale(	scale );
	mat_r.RotateY( rot );
	mat_t.Translate( pos );
	mat_audi = mat_r*mat_sc*mat_t;
}

//観客席のメイン
void Auditorium::Main()
{
	calMatrix();
}

//観客席の描画
/*
IRender		*render	:レンダリング
Matrix4x4	&vps	:カメラ行列
*/
void Auditorium::Draw( IRender *render, Matrix4x4 &vps )
{
	model->getObject(0)->Transform(mat_audi, vps);		//取得して変換
	render->setTexture(img);
	model->getObject(0)->Draw( render );						//表示
	render->setTexture( NULL );
}

//球場周りをまとめるマネージャーの初期化
void StadiumManager::Init()
{
	//ベースを"3つ"追加
	this->setStadium( new_s Base( Vector(43,1,-45),	Vector(4,3,4),10,&mdl_base,img_base,0.098f  ) );		//1st
	this->setStadium( new_s Base( Vector(0,1,-10),	Vector(4,3,4),40,&mdl_base,img_base,0.098f	 ) );		//2nd
	this->setStadium( new_s Base( Vector(-46,1,-45),Vector(4,3,4),-10,&mdl_base,img_base,0.098f ) );		//3rd
	
	//観客席を追加
	this->setStadium( new_s Auditorium( Vector(85,7,-85),Vector(20,30,20),-38.61f,&mdl_audi,img_audi) );		//1st側
	this->setStadium( new_s Auditorium( Vector(-85,7,-80),Vector(20,30,20),-36.79f,&mdl_audi,img_audi) );		//3rd側

	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//iを先頭(begin)に
	while( i != stadium.end() ){					//終わるまで
		StadiumBase *s = *i;
		s->Init();									
		++i;
	}	
}

//球場周りをまとめるマネージャーのメイン
/*
Rule	*rule	:ルールクラス
Ground	*g		:グラウンドクラス
Ball	*_ball	:ボールクラス
*/
void StadiumManager::Main( Rule *rule,Ground *g,Ball *_ball )
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//iを先頭(begin)に
	while( i != stadium.end() ){					//終わるまで
		StadiumBase *s = *i;
		g->IntersectFunction( rule,s,NULL,NULL );
		s->Main();									//派生先のMainが呼ばれる
		s->Collision( _ball);						//派生先にボールとの当たり判定処理をさせる
		++i;
	}
}

//球場周りをまとめるマネージャーの描画
/*
IRender		*render	:レンダリング
Matrix4x4	&vps	:カメラ行列
*/
void StadiumManager::Draw( IRender *render,Matrix4x4 &vps )
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();							//先頭(begin)に
	while(i != stadium.end()){						//終わるまで
		StadiumBase *s = *i;
		s->Draw( render, vps );						//派生先の描画
		++i;
	}
	render->setTexture(NULL);
}

//球場周りをまとめるマネージャーの解放
void StadiumManager::Dispose()
{
	std::list<StadiumBase *>::iterator i;
	i = stadium.begin();					//リストまわす
	while(i != stadium.end()){
		delete (*i);
		i = stadium.erase(i);
	}
}
//-----------------------------------------------------------------------------
//		 その他スタジアム（球場）に必要なクラスの実装
//-----------------------------------------------------------------------------
//球場の初期化
/*
char	*model_name	:モデルのパス
IImage	*ground_img	:地面テクスチャ
IImage	*wall_img	:壁テクスチャ
*/
void Ground::Init( char* model_name,IImage *ground_img,IImage *wall_img )
{
	model.Load( model_name );
	img_ground	= ground_img;				//地面のテクスチャ
	img_wall	= wall_img;					//壁のテクスチャ
	
	pos = math::Vector3(0,0,4);				//マップの配置座標初期化
	//グラウンド拡大させる
	math::Matrix4x4 m1;
	m1.Translate( pos );
	mat_sc.Scale(30,30,40);

	Vector local[11];

	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform( mat_sc*m1,math::Matrix::unit_matrix );
		model[i].NormalNomalize();
		model[i].PreTransform();
	}

	for( int i=1;i<11;i++ )								//ボーン座標の取得
	{
		local[i] = model.getBoneArray(i).local;
		TransformVector( i,local[i] );					
	}
	//判定も初期化	
	ResultInit();

	//ストライクゾーンの初期化
	bbox.p = Vector(-1.05f,3,-105);			//ベースの先
	bbox.s = Vector(3.005f,1.5f,0.5f);			//ストライクの判定サイズ

	one_hit = false;							//一回のみ処理させるフラグ

	se_foul = LoadSoundMem( "data/se/foul.wav" );
}

//フェンス判定の初期化(バックスクリーンなど)
void Ground::ResultInit()
{
	Vector sc( 30,30,40 );
	Matrix rr[11];
	
	rr[1].RotateY( math::Math::PI*20/180 );
	bobox1 = game::BoundingOBox3D( bobox1.p*sc ,Vector(2,2.5f,18.5f),rr[1] );	//2BH
	rr[2].RotateY( math::Math::PI*38/180 );
	bobox2 = game::BoundingOBox3D( bobox2.p*sc ,Vector(2,2.5f,19),rr[2] );	//1BH
	rr[3].RotateY( math::Math::PI*55/180 );
	bobox3 = game::BoundingOBox3D( bobox3.p*sc ,Vector(2,2.5f,11),rr[3] );	//3BH
	rr[4].RotateY( math::Math::PI*60/180 );
	bobox4 = game::BoundingOBox3D( bobox4.p*sc ,Vector(2,2.5f,6),rr[4] );	//OUT
	rr[5].RotateY( math::Math::PI*73/180 );
	bobox5 = game::BoundingOBox3D( bobox5.p*sc ,Vector(2,2.5f,17.5f),rr[5] );//HOME
	rr[6].RotateY( math::Math::PI*-75/180 );
	bobox6 = game::BoundingOBox3D( bobox6.p*sc ,Vector(2,2.5f,15.5f),rr[6] );//HOME
	rr[7].RotateY( math::Math::PI*-64/180 );
	bobox7 = game::BoundingOBox3D( bobox7.p*sc ,Vector(2,2.5f,5.2f),rr[7] ); //OUT
	rr[8].RotateY( math::Math::PI*-58/180 );
	bobox8 = game::BoundingOBox3D( bobox8.p*sc ,Vector(2,2.5f,8.5f),rr[8] ); //3BH
	rr[9].RotateY( math::Math::PI*-40/180 );
	bobox9 = game::BoundingOBox3D( bobox9.p*sc ,Vector(2,2.5f,17.5f),rr[9] );//1BH
	rr[10].RotateY( math::Math::PI*-28/180 );
	bobox10 = game::BoundingOBox3D( bobox10.p*sc ,Vector(2,2.5f,19.7f),rr[10] );//2BH
}

//ボーンローカル座標をグローバル座標に変換
/*
int		num		:ボーンナンバー
Vector3 local	:ローカル座標
*/
void Ground::TransformVector( int num,Vector3 local )
{
	Vector &v1 = local;						//ボーン1の今の座標
	Vector vv;

	vv = math::Matrix4x4::unit_matrix.TransformVector( v1 );		//グローバル座標に

	switch(num)
	{
	case 1:
		bobox1.p = vv;break;
	case 2:
		bobox2.p = vv;break;
	case 3:
		bobox3.p = vv;break;
	case 4:
		bobox4.p = vv;break;
	case 5:
		bobox5.p = vv;break;
	case 6:
		bobox6.p = vv;break;
	case 7:
		bobox7.p = vv;break;
	case 8:
		bobox8.p = vv;break;
	case 9:
		bobox9.p = vv;break;
	case 10:
		bobox10.p = vv;break;
	default:
		system::Exception("１～１０以外があります");
	}
}

//接触判定を引数別にわける
/*
Rule		*rule	:ルールクラス
StadiumBase *base1	:スタジアムクラス	
PlayerBase	*base2	:プレイヤークラス
Ball		*base3	:ボールクラス
*/
void Ground::IntersectFunction( Rule *rule,StadiumBase *base1,PlayerBase *base2,Ball *base3 )
{
	//StadiumBaseのときの判定（ベース）
	Vector f_ray(0,-1,0);					//真下の光線ベクトル
	Vector w_ray(-1,0,0);					//真横の光線ベクトル
	Vector r1,norm1;
	if( base1 )
	{
		if( Intersect( model[0],base1->getPos(),f_ray,0.5f,&mf,&r1,&norm1 ) )
		{
			Vector p;
			p = base1->getPos();
			p = r1 - f_ray*0.5f;					//押し戻す
			base1->setPos( p );
		};
		if( Intersect( model[1],base1->getPos(),w_ray,0.5f,&mf,&r1,&norm1 ) )		//ファール壁に当たったら
		{
			Vector p;
			p = base1->getPos();
			p = r1 - w_ray*0.75f;					//押し戻す
			base1->setPos( p );
		};
	}
	//PlayerBaseの時の判定(ランナーやピッチャー）
	Vector r2,norm2;
	if( base2 )
	{
		if( Intersect( model[0],base2->getPos(),f_ray,0.5f,&mf,&r2,&norm2 ) )
		{
			Vector p;
			p = base2->getPos();
			p = r2 - f_ray*0.5f;					//押し戻す
			base2->setPos( p );
		};
		if( Intersect( model[1],base2->getPos(),w_ray,0.5f,&mf,&r2,&norm2 ) )
		{
			Vector p;
			p = base2->getPos();
			p = r2 - w_ray*0.5f;					//押し戻す
			base2->setPos( p );
		};
	
	}
	
	//Ballの時の判定
	if( base3 )
	{
		const float size = base3->getBoundingObj().s;
		Vector r3,norm3;
		float speed = base3->getVelosity();				//スピードを取っておく
		//地面判定
		if( Intersect( model[0],base3->getPos(),f_ray,size,&mf,&r3,&norm3 ) )
		{
			//常時処理
			Vector p;
			p = base3->getPos();
			p = r3 - f_ray*1.6f;						//押し戻す
			base3->setPos( p );
			//打ち返した時のみ
			if( base3->getStateFlag()>=2 )
			{
				if( speed )
				{
					speed -= ( 0.065f*(base3->getMass()*base3->getGravity()) );	//勢いを無くしていく					
					base3->setVelosity( speed );
				}
			}
		};
		//サイド壁判定
		Vector d;		
		if( IntersectSideWall( model[1],base3->getPos(),0.25f,&base3->getPos(),&d ) )
		{
			speed*=0.5f;									//低反発
			base3->setVelosity( speed );
			base3->setDirectValue( -d );
		}
		
		Vector r,n;
		if( IntersectWall( model[2],base3->getPos(),size,&mf,&r,&n ) )
		{
			Vector p;
			p = r+n.Normalize()*size;
			base3->setPos( p );			
		}
		if( IntersectWall( model[4],base3->getPos(),size,&mf,&r,&n ) )
		{
			Vector p;
			p = r+n.Normalize()*size;
			base3->setPos( p );			
		}
		//結果が得られるフェンス（ヒットなどの判定は行われない→Collision処理へ）
		if( base3->getStateFlag()>=2 )							//ボールが打たれた状態
		{
			Collision( base3 );									//フェンス前の結果が得られる判定
			//ファール処理
			if( base3->getVelosity() <= 0  && base3->getStateFlag()==2  )						//速度０のとき
			{
				//ファールゾーンとの判定。面の裏でなおかつカゴがボールを取ってないなら
				if( IntersectFoul( model[3],base3->getPos()) )
				{
					result_state	= 1;						//状態ファールに
					PlaySoundMem(se_foul, DX_PLAYTYPE_BACK );
					int st = rule->getStrike();
					if( st < 2 ) st++;							//ストライクカウント飽和処理
					rule->setStrike(st);						//ストライクカウント更新	
					rule->setResult(4);
					base3->setStateFlag( 4 );					//球の出現状態を伏せる
				};
			}
		}
	}
}

//フェンスによる結果判定
/*
Ball *bbase	:ボールクラス
*/
int Ground::Collision( Ball *bbase )
{
	BoundingSphere3D sphere = bbase->getBoundingObj();		//引数から当たり判定の情報を取得
	int ret = -1;												//戻り値 初期化はわざと関係ない値を入れている

	//各フェンス前のOBox判定をする
	if( Collision3D::Collision( sphere,bobox1 ))	ret =  2;		//レフト2BH
	else if( Collision3D::Collision( sphere,bobox2 )) ret = 1;		//レフト1BH
	else if( Collision3D::Collision( sphere,bobox3 )) ret = 3;		//レフト3BH
	else if( Collision3D::Collision( sphere,bobox4 )) ret = 0;		//レフトOUT
	else if( Collision3D::Collision( sphere,bobox5 )) ret = 4;		//レフトホームラン
	else if( Collision3D::Collision( sphere,bobox6 )) ret = 4;		//ライトホームラン
	else if( Collision3D::Collision( sphere,bobox7 )) ret = 0;		//ライトOUT
	else if( Collision3D::Collision( sphere,bobox8 )) ret = 3;		//ライト3BH
	else if( Collision3D::Collision( sphere,bobox9 )) ret = 1;		//ライト1BH
	else if( Collision3D::Collision( sphere,bobox10 ))ret = 2;		//ライト2BH
	
	return ret;
}

//球場のメイン
/*
Ball	*_ball	:ボールクラス
Bat		*_bat	:バットクラス
Rule	*_rule	:ルールクラス
*/
void Ground::Main( Ball *_ball,Bat *_bat,Rule *_rule )
{
	if( _ball->getStateFlag()==0 || _rule->getChangeFlag()  )	//交代した瞬間も
	{
		one_hit = false;										//ボールの状態が0のとき
		result_state = 0;										//ボールの結果状態を0に
	}
	else if( _ball->getStateFlag()==1 )							//ボールが投球中なら
	{
		if( one_hit ) return;
		//ストライクゾーンに当たった時
		if( StrikeJudge( _ball ) == true )		
		{
			int a = 0;

			//ストライク加算フラグ
			_rule->setStrikeAddFlag( true );
			if(!one_hit)one_hit=true;						//フラグを立て二回目の処理をしないようにさせる	
		}else{												//ゾーン外（ボールのとき）
			//ストライクゾーン判定座標からバックネット判定座標まで長さを得る
			const float len = (bbox.p-_ball->getBackNetPos() ).Length2();
			const float cnt = ( 0 + (_bat->getLeanCnt()*2) )*len - (_bat->getLeanCnt()*2);	//振っていたか　バットを振っている間はカウントがある
			//空振り処理  キャッチャーに到着//
			if( _ball->BackNetCollision()==1 )				//終着点まで付いた時に	
			{
				if( cnt ){									//カウントが残っていたら(０～１の間しかカウントが無いのでバックネットからストライク判定までの距離ぶん増加)
					_rule->setBallAddFlag( false );			//ボールフラグは伏せる
					_rule->setStrikeAddFlag( true );		//変わりにストライクフラグを立てる
				}else _rule->setBallAddFlag( true );
			}
		}
	}
}

//ストライクゾーンの判定
/*
Ball *bbase	:ボールクラス
*/
bool Ground::StrikeJudge( Ball *bbase )
{
	//派生先のボールから当たり判定の情報を取得
	BoundingSphere3D sphere = bbase->getBoundingObj();
	sphere.s*=0.5f;										//球の元のサイズより小さくして判定を厳しくする
	
	//すでにあたっていたなら
	if( one_hit ) return false;
	//ストライクゾーン判定
	Vector	vret;
	//一回も当たってないフラグが立ってないときに当たった
	if( Intersect3D::Intersect( sphere,this->bbox,&vret)  ) 
	{	
		one_hit = true;
		return true;
	}else  return false;
}

//球場の描画
/*
IRender		*render	:レンダリングクラス
Matrix4x4	&vps	:カメラ行列
*/
void Ground::Draw( IRender *render, Matrix4x4 &vps )
{	
	//地面描画
	render->setTexture( img_ground );
	model[0].Transform( math::Matrix4x4::unit_matrix,vps );
	model[0].Draw( render );

	//壁描画
	render->setTexture( img_wall );
	model[1].Transform( math::Matrix4x4::unit_matrix,vps );
	model[1].Draw( render );
	
	//結果の壁描画
	model[2].Transform( math::Matrix4x4::unit_matrix,vps );
	model[2].Draw( render );
	
	//結果の壁を分ける壁
	model[4].Transform( math::Matrix4x4::unit_matrix,vps );
	model[4].Draw( render );
}

//球場の接触判定
/*
Object3D	&map	:マップモデルのオブジェクト
Vector3		&pos	:球の中心座標
float		size	:判定サイズ
ModelFace	**f		:モデルの面
Vector3		*ret	:交点
Vector3		*norm	:法線 
*/
bool Ground::Intersect( game::Object3D &map,Vector3 &pos, Vector3 &ray, float size, ModelFace **f, Vector3 *ret, Vector3 *norm )
{
	Vector cros;				//交点戻り値用
	
	//全面判定
	int i;
	for( i=0;i<map.getFaceCount();i++ )
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex( mf[0] );		//面0
		Vertex3D  &v2 = map.getVertex( mf[1] );		//面1
		Vertex3D  &v3 = map.getVertex( mf[2] );		//面2
		Plane pl( v1.p,v2.p,v3.p );					//位置関係
		if( pl.IntersectRay( pos,ray,&cros )==1 )	//レイの方向に面があるなら
		{
			if( pl.Distance( pos ) < size && pl.Inside( v1.p,v2.p,v3.p,cros )){
				*f		=&mf;
				*ret	=cros;		//交点位置
				*norm	=pl.n;		//法線の位置
				
				return true;
			}
		}
	}
	return false;
}

//複数同時の面判定 壁判定
/*
Object3D	&map	:マップモデルのオブジェクト
Vector3		&pos1	:球の中心座標
float		size	:判定させるための範囲サイズ
Vector3		*pos	:判定後の補正座標
Vector3		*dir	:判定後の補正軸
*/
bool Ground::IntersectSideWall( game::Object3D &map,math::Vector3 &pos1,float size,math::Vector3 *pos,Vector3 *dir )
{
	Vector ret;				//交点戻り値用
	*pos = pos1;			//今の座標をコピーして持っておく
	int hit=0;				//面との当たった回数

	//全面判定基本
	int i;
	for(i=0;i<map.getFaceCount();i++)
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex( mf[0] );		//面0
		Vertex3D  &v2 = map.getVertex( mf[1] );		//面1
		Vertex3D  &v3 = map.getVertex( mf[2] );		//面2
		Plane pl( v1.p,v2.p,v3.p );					//位置関係		
		Vector ray = -pl.n;							//各面のPlane法線をrayとして考える(扱う)

		float side = pl.Side( *pos );
		if( side - size < 0 )
		if( pl.IntersectRay( *pos,ray,&ret )>0 )	//レイ方向に面があるなら
		{
			if(	pl.Inside(v1.p,v2.p,v3.p,ret) )		//点と面の位置を調べ、尚且つポリゴン内
			{
				hit++;		//補正
				*dir = ret;
				if( hit>=2 ) return true;	//2枚以上同時にはあたらない
			}
		}
	}
	return hit!=0;
}

//カベ判定
/*
Object3D	&map	:マップモデルのオブジェクト
Vector3		&pos	:球の中心座標
float		size	:判定サイズ
ModelFace	**f		:モデルの面
Vector3		*r		:交点
Vector3		*n		:法線 
*/
bool Ground::IntersectWall( Object3D &map,Vector &pos,float size,ModelFace**f,Vector *r,Vector *n )
{
	Vector ret;
	int i;
	for(i=0;i<map.getFaceCount();++i)
	{
		ModelFace &mf = map.getFace(i);
		Vertex3D  &v1 = map.getVertex(mf[0]);
		Vertex3D  &v2 = map.getVertex(mf[1]);
		Vertex3D  &v3 = map.getVertex(mf[2]);
		Plane pl( v1.p,v2.p,v3.p );
		Vector ray = -pl.n;

		if( pl.IntersectRay( pos,ray,&ret ) == 1 )
		{
			if( pl.Distance(pos)< size && pl.Inside( v1.p,v2.p,v3.p,ret ))
			{
				*f = &mf;
				*r = ret;
				*n = pl.n;
				return true;
			}
		}
	}
	return false;
}

//ファール判定
/*
Object3D	&map	:マップモデルのオブジェクト
Vector3		&pos	:球の中心座標
*/
bool Ground::IntersectFoul( Object3D &map, Vector3 &pos )
{

	ModelFace &mf = map.getFace(0);				//面0番目
	Vertex3D  &v1 = map.getVertex( mf[0] );		//頂点
	Vertex3D  &v2 = map.getVertex( mf[1] );		
	Vertex3D  &v3 = map.getVertex( mf[2] );		
	Plane p1( v1.p,v2.p,v3.p );
	
	ModelFace  &mf2  = map.getFace(3);
	Vertex3D  &vv1 = map.getVertex( mf2[0] );
	Vertex3D  &vv2 = map.getVertex( mf2[1] );
	Vertex3D  &vv3 = map.getVertex( mf2[2] );
	Plane p2( vv1.p,vv2.p,vv3.p );
	
	//ファールゾーン
	if( p1.Side(pos) < 0 || p2.Side(pos)<0 )
	{
		return true;
	}
	
	return false;
}

//フェンス判定の取得
/*
int num	:オブジェクト番号
*/
game::BoundingOBox3D Ground::getBoundingObj( int num )
{
	game::BoundingOBox3D	ret;		//戻り値用
	switch( num )
	{
		case 1:
			ret  = bobox1;break;
		case 2:
			ret  = bobox2;break;
		case 3:
			ret  = bobox3;break;
		case 4:
			ret  = bobox4;break;
		case 5:
			ret  = bobox5;break;
		case 6:
			ret  = bobox6;break;
		case 7:
			ret  = bobox7;break;
		case 8:
			ret  = bobox8;break;
		case 9:
			ret  = bobox9;break;
		case 10:
			ret  = bobox10;break;
		default: system::Exception("1～10を選択してください");
	};
	return ret;
}



//-------------------------------------------------------------------------
//空の初期化
/*
char	*model_name	:モデルのパス
IImage	*sky_img	:空のテクスチャ
*/
void SkyDome::Init( char* model_name,IImage *sky_img )
{
	model.Load( model_name );
	img_sky	= sky_img;						//空のテクスチャ

	pos = math::Vector3(0,-10,0);			//マップの配置座標初期化
	rot = 0;								//回転度数
	rot_val	= 0.0003f;						//初期回転スピード
	//空を拡大させる
	math::Matrix4x4 m1;
	m1.Translate( pos );
	const float ss = 130;					//スケーリングサイズ
	mat_sc.Scale(ss,ss,ss);
	for( int i=0;i<model.getObjectCount();i++)
	{
		model[i].Transform( mat_sc*m1,math::Matrix::unit_matrix );
		model[i].NormalNomalize();
		model[i].PreTransform();
	}	
}

//空のメイン
void SkyDome::Main()
{
	rot-=rot_val;				//度数を与えて	
	mat_sc.RotateY( rot );		//回転させる
}

//空（スカイドーム）の描画
/*
IRender		*render	:レンダリング
Matrix4x4	&vps	:カメラ行列
*/
void SkyDome::Draw( IRender *render, Matrix4x4 &vps )
{			
	for(int i=0;i<model.getObjectCount();i++)		//モデル個数分描画
	{
		//空描画
		render->setTexture( img_sky );
		model[0].Transform( mat_sc,vps );
		model[0].Draw( render );		
	}
	render->setTexture( NULL );	
}

