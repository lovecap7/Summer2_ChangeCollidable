#include "GameClearScene.h"
#include "TitleScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../Game/GameRule/Score.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//�Q�[����ʂ���|�[�Y��ʂ܂ł̕�
}

GameClearScene::GameClearScene(SceneController& controller, std::shared_ptr<Score> score):
	SceneBase(controller),
	m_score(score),
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

void GameClearScene::Restart()
{
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
	//�X�R�A�\��
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 100.0f, 0xff2222, "TotalScore : %5d", m_score->GetScore());
	//���j�X�R�A
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 150.0f, 0xff2222, "KillScore : %5d", m_score->GetKillScore());
	//�A�C�e���Q�b�g�X�R�A
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 200.0f, 0xff2222, "ItemScore : %5d", m_score->GetItemScore());
	//�^�C���X�R�A
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 250.0f, 0xff2222, "TimeScore : %5d", m_score->GetTimeScore());
	//�̗̓X�R�A
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 300.0f, 0xff2222, "HPScore : %5d", m_score->GetHPScore());
}