#include "EnemyManager.h"
#include "../../../General/Collision/CollisionChecker.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../Player/Player.h"
#include "../Actor.h"
#include "EnemyBase.h"
#include "../../../General/Collidable.h"
#include "../../../General/Rigidbody.h"

EnemyManager::EnemyManager(std::shared_ptr<Player> player) :
	m_player(player),
	m_collChecker(std::make_shared<CollisionChecker>())
{
}

void EnemyManager::Entry(std::shared_ptr<EnemyBase> enemy)
{
	m_enemies.emplace_back(enemy);
}

void EnemyManager::Exit(std::shared_ptr<EnemyBase> enemy)
{
	auto it = std::find(m_enemies.begin(), m_enemies.end(), enemy);
	//見つかった場合
	if (it != m_enemies.end())
	{
		//削除
		m_enemies.erase(it);
	}
}

void EnemyManager::Update()
{
	//索敵
	SearchPlayer();
}

void EnemyManager::SearchPlayer()
{
	//空なら処理しない
	if (m_enemies.empty())return;

	//範囲内にプレイヤーがいるか判定をチェック
	for (auto& enemy : m_enemies)
	{
		//トリガー
		auto trigger = enemy->GetSearchTrigger();
		//プレイヤー
		auto collP = m_player->GetCollidable();
		//範囲内にプレイヤーがいるかチェック
		if (m_collChecker->CheckCollCS(collP, trigger->GetCollidable()))
		{
			//当たった時の処理
			trigger->OnHitTarget(collP->GetRb()->GetPos());
		}
	}
}