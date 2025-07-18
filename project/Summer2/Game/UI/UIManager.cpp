#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
//プレイヤー
#include "PlayerUI/PlayerHPUI.h"
#include "PlayerUI/PlayerUltGageUI.h"
//ボス
#include "EnemyUI/BossHPUI.h"
//敵
#include "EnemyUI/EnemyHPUI.h"
//スコア
#include "ScoreUI.h"
//タイマー
#include "TimerUI.h"
UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::Init()
{
}

void UIManager::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//削除予定のUI削除
	CheckDelete();
	//更新
	for (auto& ui : m_uis)
	{
		ui->Update(actorManager);
	}
}

void UIManager::Draw() const
{
	for (auto& ui : m_uis)
	{
		ui->Draw();
	}
}

void UIManager::End()
{
	for (auto& ui : m_uis)
	{
		ui->End();
	}
	m_uis.clear();
}

void UIManager::Restart()
{
	End();
}

void UIManager::AddUI(std::shared_ptr<UIBase> ui)
{
	//すでに登録されているならしない
	auto it = std::find(m_uis.begin(), m_uis.end(), ui);
	if (it != m_uis.end())return;
	//UIの初期化
	ui->Init();
	//追加
	m_uis.emplace_back(ui);
}

void UIManager::CreatePlayerUI(const std::weak_ptr<Player> player)
{
	AddUI(std::make_shared<PlayerHPUI>(player));
	AddUI(std::make_shared<PlayerUltGageUI>(player));
}

void UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	AddUI(std::make_shared<BossHPUI>(boss));
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	AddUI(std::make_shared<EnemyHPUI>(enemy));
}

void UIManager::CreateScoreUI(const std::weak_ptr<Score> score)
{
	AddUI(std::make_shared<ScoreUI>(score));
}

void UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	AddUI(std::make_shared<TimerUI>(timer));
}

void UIManager::CheckDelete()
{
	auto remIt = std::remove_if(m_uis.begin(), m_uis.end(), [](std::shared_ptr<UIBase> ui) {
		bool isDead = ui->IsDelete();//チェック
		if (isDead)
		{
			//UIの終了処理
			ui->End();
		}
		return isDead;
		});
	m_uis.erase(remIt, m_uis.end());//削除
}
