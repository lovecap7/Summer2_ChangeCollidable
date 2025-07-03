#include "GameClearScene.h"
#include "TitleScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
}

GameClearScene::GameClearScene(SceneController& controller) :
	SceneBase(controller),
	m_update(&GameClearScene::AppearUpdate),
	m_draw(&GameClearScene::NormalDraw),
	m_countFrame(0)
{
}

GameClearScene::~GameClearScene()
{
}

void GameClearScene::Init()
{
	//Physics���~�߂�
	Physics::GetInstance().StopUpdate();
}

void GameClearScene::Update()
{
	(this->*m_update)();
}

void GameClearScene::Draw()
{
	(this->*m_draw)();
}
void GameClearScene::End()
{
	//Physics���J�n
	Physics::GetInstance().StartUpdate();
}

void GameClearScene::AppearUpdate()
{
	++m_countFrame;
	if (m_countFrame > kAppearInterval)
	{
		m_countFrame = kAppearInterval;
		m_update = &GameClearScene::NormalUpdate;
		return;
	}
}

void GameClearScene::NormalUpdate()
{
	auto& input = Input::GetInstance();
	//A�{�^���Ŏ���
	if (input.IsTrigger("A"))
	{
		m_update = &GameClearScene::DisappearUpdate;
		m_countFrame = kAppearInterval;
		return;
	}
}

void GameClearScene::DisappearUpdate()
{
	--m_countFrame;
	if (m_countFrame < 0)
	{
		m_countFrame = 0;
		//�����̉��ɂȂ��Ă�V�[����؂�ւ���
		m_controller.ChangeBaseScene(std::make_shared<TitleScene>(m_controller));
		m_controller.PopScene();//�����͏�����
		return;
	}
}

void GameClearScene::NormalDraw()
{
	//�w�i�𔒂��ۂ�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 7 * m_countFrame);
	DrawBox(0, 0,//����
		Game::kScreenWidth, Game::kScreenHeight,//�E��
		0xffffff,//�J���[
		true);//�h��ׂ�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}