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
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
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
	//Physicsを止める
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
	//Physicsを開始
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
	//Aボタンで次へ
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
		//自分の下になってるシーンを切り替える
		m_controller.ChangeBaseScene(std::make_shared<TitleScene>(m_controller));
		m_controller.PopScene();//自分は消える
		return;
	}
}

void GameClearScene::NormalDraw()
{
	//背景を白っぽくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 7 * m_countFrame);
	DrawBox(0, 0,//左上
		Game::kScreenWidth, Game::kScreenHeight,//右下
		0xffffff,//カラー
		true);//塗り潰す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//スコア表示
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 100.0f, 0xff2222, "TotalScore : %5d", m_score->GetScore());
	//撃破スコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 150.0f, 0xff2222, "KillScore : %5d", m_score->GetKillScore());
	//アイテムゲットスコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 200.0f, 0xff2222, "ItemScore : %5d", m_score->GetItemScore());
	//タイムスコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 250.0f, 0xff2222, "TimeScore : %5d", m_score->GetTimeScore());
	//体力スコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 300.0f, 0xff2222, "HPScore : %5d", m_score->GetHPScore());
}