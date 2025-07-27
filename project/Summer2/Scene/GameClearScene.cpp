#include "GameClearScene.h"
#include "TitleScene.h"
#include "../General/Input.h"
#include "SceneController.h"
#include<DxLib.h>
#include "../General/game.h"
#include "../General/Collision/Physics.h"
#include "../Game/GameRule/Score.h"
#include "../General/Fader.h"
#include "../Game/UI/UIManager.h"
#include "../General/CSVDataLoader.h"
#include "../Game/UI/Result/ResultScoreUI.h"

namespace {
	constexpr int kAppearInterval = 20;
	constexpr int kFrameMargin = 10;//ゲーム画面からポーズ画面までの幅
}

GameClearScene::GameClearScene(SceneController& controller, std::shared_ptr<Score> score, Stage::StageIndex index):
	SceneBase(controller),
	m_score(score),
	m_stageIndex(index),
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
	auto& uiManager = UIManager::GetInstance();
	//UI表示
	uiManager.StartDraw();
	uiManager.StartUpdate();
	//一度UIをリセット
	uiManager.Reset();
	//Physicsを止める
	Physics::GetInstance().StopUpdate();
	//スコア更新
	m_score->UpdateScore(m_stageIndex);
	//ハイスコアを保存
	m_score->SaveHighScore();
	//リザルトUI
	//ローダー
	auto loader = std::make_shared<CSVDataLoader>();
	auto uiData = loader->LoadResultScoreUIDataCSV();
	//データの数だけUIを用意
	for (auto& data : uiData)
	{
		std::shared_ptr<ResultScoreUI> scoreUI;
		float score = 0.0f;
		if (data.name == "TotalScore")
		{
			score = m_score->GetScore();
		}
		else if (data.name == "KillScore")
		{
			score = m_score->GetKillScore();
		}
		else if (data.name == "ItemScore")
		{
			score = m_score->GetItemScore();
		}
		else if (data.name == "TimeScore")
		{
			score = m_score->GetTimeScore();
		}
		else if (data.name == "HPScore")
		{
			score = m_score->GetHPScore();
		}
		else if (data.name == "HighScore1")
		{
			score = m_score->GetHighScore(m_stageIndex)[0];
		}
		else if (data.name == "HighScore2")
		{
			score = m_score->GetHighScore(m_stageIndex)[1];
		}
		else if (data.name == "HighScore3")
		{
			score = m_score->GetHighScore(m_stageIndex)[2];
		}

		scoreUI = std::make_shared<ResultScoreUI>(score, data.pos, data.scale, data.margin);
		scoreUI->Init();
	}
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
	//UIをリセット
	UIManager::GetInstance().Reset();
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
		auto& fader = Fader::GetInstance();
		//だんだん暗く
		fader.FadeOut();
		//次の状態
		m_update = &GameClearScene::DisappearUpdate;
		return;
	}
}

void GameClearScene::DisappearUpdate()
{
	auto& fader = Fader::GetInstance();
	//暗くなったら
	if (fader.IsFinishFadeOut())
	{
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
	auto totalScore = m_score->GetScore();
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 100.0f, 0x55555, L"TotalScore : %5d", totalScore);
	//撃破スコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 150.0f, 0x55555, L"KillScore : %5d", m_score->GetKillScore());
	//アイテムゲットスコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 200.0f, 0x55555, L"ItemScore : %5d", m_score->GetItemScore());
	//タイムスコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 250.0f, 0x55555, L"TimeScore : %5d", m_score->GetTimeScore());
	//体力スコア
	DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 300.0f, 0x55555, L"HPScore : %5d", m_score->GetHPScore());
	//ハイスコア
	auto highScore = m_score->GetHighScore(m_stageIndex);
	for (int i = 0;i < 3;++i)
	{
		DrawFormatString((Game::kScreenWidth / 2.0f) - 100.0f, 350.0f + 50 * i, 0x55555, L"Ranking : %d",i + 1 );
		DrawFormatString((Game::kScreenWidth / 2.0f) + 100.0f, 350.0f + 50 * i, 0x55555, L"HighScore : %5d", highScore[i]);
		//ハイスコア更新
		if (totalScore == highScore[i])
		{
			DrawString((Game::kScreenWidth / 2.0f) + 100.0f, 325.0f + 50 * i, L"New Record!!!", 0xff0000);
		}
	}
	
}