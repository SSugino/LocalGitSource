/** @file
 * �V�[���N���X�̎���
 * @date 2009/08/24
 * @author ����
 **/

#include "Scene.h"
using namespace sugino;
using namespace game;
using namespace math;
using namespace graphics;


void SceneManager::setNextScene()
{
	if( now ){ delete now; }//���̃V�[�����J��
	now = next;	//���̃V�[�������̃V�[����
	next = NULL;//���̃V�[���͖���
	now->setManager( this );
	now->Init();	//�V�[���J�n
	if( se_in )		//�t�F�[�h�C������������Ȃ�
	{
		se_in->Init();		//����������
	}
}

void SceneManager::Init()
{
	now->Init();
}

void SceneManager::Main()
{
	if( now )now->Main();			//�����݂̃V�[�����J�n����
	if( se_out ) se_out->Main();	//�A�E�g�G�t�F�N�g������Ȃ�
	else if( se_in ) se_in->Main();		//�C���G�t�F�N�g������Ȃ�
}

void SceneManager::Draw()
{
	now->Draw();
	if( se_out )//�t�F�[�h�A�E�g����������
	{
		se_out->Draw();
		if( se_out->isEnd() )	//�t�F�[�h�A�E�g�����I���Ȃ�
		{
			delete se_out;		//���
			se_out=NULL;
			setNextScene();		//���̃V�[���ֈڍs������
		}
	}else if( se_in )
	{		//�t�F�[�h�C��
		se_in->Draw();
		if( se_in->isEnd() )
		{
			delete se_in;
			se_in=NULL;
		}
	}
}


