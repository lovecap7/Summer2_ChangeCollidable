#include "UIManager.h"
#include "UIBase.h"
#include "../Actors/ActorManager.h"
#include <cassert>
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
	//ハンドルロード
	LoadHandle();
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
	//すべて削除
	AllDeleteUIs();
	//ハンドル削除
	AllDeleteHandle();
}

void UIManager::Restart()
{
	//UIのみすべて削除
	AllDeleteUIs();
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
	AddUI(std::make_shared<PlayerHPUI>(-1,player));
	AddUI(std::make_shared<PlayerUltGageUI>(-1, player));
}

void UIManager::CreateBossUI(const std::weak_ptr<EnemyBase> boss)
{
	AddUI(std::make_shared<BossHPUI>(-1, boss));
}

void UIManager::CreateEnemyUI(const std::weak_ptr<EnemyBase> enemy)
{
	AddUI(std::make_shared<EnemyHPUI>(-1, enemy));
}

void UIManager::CreateScoreUI(const std::weak_ptr<Score> score)
{
	AddUI(std::make_shared<ScoreUI>(m_handles["Score"], score));
}

void UIManager::CreateTimerUI(const std::weak_ptr<Timer> timer)
{
	AddUI(std::make_shared<TimerUI>(-1, timer));
}

void UIManager::LoadHandle()
{
	m_handles["Score"] = { LoadGraph("Data/UI/Number.png") };
	
	//ロードに成功したかチェック
	for (auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void UIManager::AllDeleteUIs()
{
	for (auto& ui : m_uis)
	{
		ui->End();
	}
	m_uis.clear();
}

void UIManager::AllDeleteHandle()
{
	for (auto& [key, value] : m_handles) {
		if (value >= 0)
		{
			auto result = DeleteGraph(value);
			assert(result == 0);
		}
	}
	m_handles.clear();
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
