/** @file
 * �L�[�{�[�h�E�}�E�X���̓N���X����
 * @date 2009/05/18
 * @author ����
 **/

#include "Input.h"
#include <Windows.h>
#include "../system/Exception.h"
#include "../CPU.hpp"

using namespace sugino;
using namespace input;
using namespace system;
using namespace math;

// �L�[���X�V
void Key::UpDate()
{

	//DWORD key_data2 = key_data;
	//key_data=0;

	/*
	//�v���C���[�ɂ���ď�������
	switch(P1)
	{
	case PLAY_CONTROL::CPU:		//�R���s���[�^�[��
		key_data2 = key_data;
		key_data=0;

		PiDataPtr= picherArray1 ;	//���؃f�[�^�擾

		key_data = PiDataPtr[ g_cpuFC++ ];		//�����f�[�^�Č�


		break;
	case PLAY_CONTROL::HUMAN:		//�l�ԑ��쎞
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//�񂷓x�r�b�g�𗧂ĂĂ���
		}

		break;
	}
	*/

	/*
		PiDataPtr= picherArray1 ;	//���؃f�[�^�擾
		key_data = PiDataPtr[ g_cpuFC++ ];		//�����f�[�^�Č�
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//�񂷓x �r�b�g�𗧂ĂĂ���
		}
	

	DWORD key_ch = key_data^key_data2;	//�ω�
	key_on=key_data&key_ch;				//�f�[�^����ON�ɂȂ����u�Ԃ̃f�[�^�����
	key_off=key_data2&key_ch;			//�f�[�^����OFF�ɂȂ����u�Ԃ̃f�[�^�����
	*/
	
	//�v���C���[�ɂ���ď�������
	
	switch(P1)
	{
	case PLAY_CONTROL::CPU:		//�R���s���[�^�[��
		PiDataPtr= picherArray1 ;	//���؃f�[�^�擾
		key_data = PiDataPtr[ g_cpuFC++ ];		//�����f�[�^�Č�
	case PLAY_CONTROL::HUMAN:		//�l�ԑ��쎞
		for( int i=0;config[i];i++ ){
			if( GetAsyncKeyState(config[i])<0 ) key_data |= (1<<i);		//�񂷓x �r�b�g�𗧂ĂĂ���
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
	/** ���݂̃}�E�X���W�擾 */
	POINT p;
	GetCursorPos( &p );
	ScreenToClient( win->getHwnd(), &p );
	ms_x = (float)p.x;
	ms_y = (float)p.y;

	DWORD ms_data2 = ms_data;ms_data=0;

	/* �r�b�g���グ�� **/
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

	this->ms_past3p = ms_v3p;						//1���[�v�O�̍��W�ۑ�

	this->ms_v3p.setVector(tv.x,tv.y,tv.z);			//�O�������W�ɕϊ�
	this->ms_v3p /=tv.w;

	this->ms_ray = ( ms_v3p-cpos ).Normalize();		//�}�E�X���W�ƃJ�������W���烌�C�����
}

/** �X�N���[�����W����N���C�A���g���W�ɕϊ� */
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
	if( ret == JOYERR_NOERROR )throw Exception( "�W���C�X�e�B�b�N���h�����Ă܂���" );
	return true;	//������Ă��Ȃ�	
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

	 �r�b�g���グ�� 
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

