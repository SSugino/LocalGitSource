/** @file
 * キーボード・マウス入力クラス実装
 * @date 2009/05/18
 * @author 杉野
 **/

#include "Input.h"
#include <Windows.h>
#include "../system/Exception.h"
#include "../CPU.hpp"

using namespace sugino;
using namespace input;
using namespace system;
using namespace math;

// キー情報更新
void Key::UpDate()
{

	//DWORD key_data2 = key_data;
	//key_data=0;

	/*
	//プレイヤーによって処理分け
	switch(P1)
	{
	case PLAY_CONTROL::CPU:		//コンピューター時
		key_data2 = key_data;
		key_data=0;

		PiDataPtr= picherArray1 ;	//球筋データ取得

		key_data = PiDataPtr[ g_cpuFC++ ];		//投球データ再現


		break;
	case PLAY_CONTROL::HUMAN:		//人間操作時
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//回す度ビットを立てていく
		}

		break;
	}
	*/

	/*
		PiDataPtr= picherArray1 ;	//球筋データ取得
		key_data = PiDataPtr[ g_cpuFC++ ];		//投球データ再現
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//回す度 ビットを立てていく
		}
	

	DWORD key_ch = key_data^key_data2;	//変化
	key_on=key_data&key_ch;				//データからONになった瞬間のデータを取る
	key_off=key_data2&key_ch;			//データからOFFになった瞬間のデータを取る
	*/
	
	//プレイヤーによって処理分け
	
	switch(P1)
	{
	case PLAY_CONTROL::CPU:		//コンピューター時
		PiDataPtr= picherArray1 ;	//球筋データ取得
		key_data = PiDataPtr[ g_cpuFC++ ];		//投球データ再現
	case PLAY_CONTROL::HUMAN:		//人間操作時
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//回す度 ビットを立てていく
		}
		break;
	}

 	DWORD key_ch = key_data^key_data2;
	key_on=key_data&key_ch;
	key_off=key_data2&key_ch;

}
void Key::gFc_Init()
{
	g_cpuFC=0;
}

void Mouse::Update(sugino::system::Window *win)
{
	/** 現在のマウス座標取得 */
	POINT p;
	GetCursorPos( &p );
	ScreenToClient( win->getHwnd(), &p );
	ms_x = (float)p.x;
	ms_y = (float)p.y;

	DWORD ms_data2 = ms_data;ms_data=0;

	/* ビットを上げる **/
	if( GetAsyncKeyState( VK_LBUTTON )<0 ) ms_data |= LCLICK;
	if( GetAsyncKeyState( VK_MBUTTON )<0 ) ms_data |= MCLICK;
	if( GetAsyncKeyState( VK_RBUTTON )<0 ) ms_data |= RCLICK;
	
	DWORD ms_ch = ms_data^ms_data2;
	ms_on=ms_data&ms_ch;
	ms_off=ms_data2&ms_ch;
}

void Mouse::TransformPosition3D( Matrix4x4 &vps, Vector3 &cpos )
{
	math::Matrix4x4 ivps = math::Matrix4x4::Inverse( vps );
	math::Vector4 tv = ivps.Transform( math::Vector3( this->ms_x,this->ms_y,0) );

	this->ms_past3p = ms_v3p;						//1ループ前の座標保存

	this->ms_v3p.setVector(tv.x,tv.y,tv.z);			//三次元座標に変換
	this->ms_v3p /=tv.w;

	this->ms_ray = ( ms_v3p-cpos ).Normalize();		//マウス座標とカメラ座標からレイを作る
}

/** スクリーン座標からクライアント座標に変換 */
void Mouse::Scaling( system::Window*win,int wid,int hei )
{
	int w,h;
	win->getSize( &w,&h );
	ms_x = (ms_x * wid )/w;
	ms_y = (ms_y * hei )/h;
}


bool Joy::Check()
{
	MMRESULT ret;
	JOYINFO joy;

	ret = joyGetPos(0,&joy);
	if( ret == JOYERR_NOERROR )throw Exception( "ジョイスティックが刺さってません" );
	return true;	//続されていない	
}

bool Joy::isJoyDown( DWORD bit )
{
	return ( ( joy_data&bit ) == bit );
}

bool Joy::isJoyUp( DWORD bit )
{
	return ( ( joy_data&bit ) == bit );
}
/*
void Joy::UpDate()
{
	JOYINFO joy;
	JoyGetPos(0,&joy);
	DWORD joy_data2 = joy_data;joy_data=0;
	
	joy_data=joy.wButtons<<4;

	 ビットを上げる 
	if( joy.wYpos < 10000 ) joy_data |= UP;
	if( joy.wYpos < 10000 ) joy_data |= DOWN;
	if( joy.wXpos > 10000 ) joy_data |= LEFT;
	if( joy.wXpos > 10000 ) joy_data |= RIGHT;
	
	DWORD joy_ch = joy_data^joy_data2;
	joy_on=joy_data&joy_ch;
	joy_off=joy_data2&joy_ch;
}
*/

int MouseWheelEvent::Event( system::Window *w,UINT msg,WPARAM wp,LPARAM lp )
{
	if( msg==0x20A ){ 
		mouse->setWheel( ((short)(wp>>16))/WHEEL_DELTA );
	}
	return 0;
}

