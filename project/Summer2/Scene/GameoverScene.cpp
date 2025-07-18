#include "GameoverScene.h"
#include "StageScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"

namespace {
	constexpr int kAppearInterval = 20;
}

GameoverScene::GameoverScene(SceneController& controller):
	SceneBase(controller),
	m_update(&GameoverScene::AppearUpdate),
	m_draw(&GameoverScene::NormalDraw),
	m_countFrame(0)
{
}

GameoverScene::~GameoverScene()
{
}

void GameoverScene::Init()
{
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
}

void GameoverScene::Update()
{
	(this->*m_update)();
}

void GameoverScene::Draw()
{
	(this->*m_draw)();
}
void GameoverScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
}

void GameoverScene::Restart()
{
}

void GameoverScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		m_countFrame = kAppearInterval;
		m_update = &GameoverScene::NormalUpdate;
		return;
	}
}

void GameoverScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//A�{�^���Ŏ���
	if (input.IsTrigger("A")) 
	{
		m_update = &GameoverScene::DisappearUpdate;
		m_countFrame = kAppearInterval;
		return;
	}
}

void GameoverScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0)
	{
		m_countFrame = 0;
		//�����̉��ɂȂ��Ă�V�[����������
		m_controller.RestartBaseScene();
		m_controller.PopScene();//�����͏�����
		return;
	}
}

void GameoverScene::NormalDraw()
{
	//�w�i��Ԃ��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 10 * m_countFrame);
	DrawBox(0, 0,//����
		Game::kScreenWidth, Game::kScreenHeight,//�E��
		0xff5555,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}