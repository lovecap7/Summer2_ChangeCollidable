#include "PlayerManager.h"
#include "Player.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../Game/Actors/Actor.h"
#include "../../../General/Collidable.h"
#include "../../../General/Collision/CollisionChecker.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../../../General/Math/MyMath.h"

PlayerManager::PlayerManager(std::shared_ptr<Player> player):
	m_player(player),
	m_triggerChecker(std::make_shared<CollisionChecker>())
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//索敵
	SearchTarget(actors);
}

void PlayerManager::SearchTarget(std::vector<std::shared_ptr<Actor>> actors)
{
	//プレイヤーのトリガー
	auto triggerP = m_player->GetSearchTrigger();
	//初期化
	triggerP->Init();
	//最短距離
	float shortDis = 0.0f;
	//ターゲット
	Vector3 target = Vector3::Zero();
	for (auto& actor : actors)
	{
		//描画用のクラスは無視
		if (actor->IsDrawOnly())continue;
		if (actor->GetCollidable()->GetGameTag() == GameTag::Enemy)
		{
			//ヒットしたか
			bool isHit = false;
		
			//コライダブル
			auto collP = triggerP->GetCollidable();
			//敵のコライダブル
			auto collE = actor->GetCollidable();
			//球なら
			if (collE->GetColl()->GetShape() == Shape::Sphere)
			{
				//範囲内に敵がいるかチェック
				if (m_triggerChecker->CheckCollSS(collP, collE))
				{
					isHit = true;
				}
			}
			//カプセル
			else if (collE->GetColl()->GetShape() == Shape::Capsule)
			{
				//範囲内に敵がいるかチェック
				if (m_triggerChecker->CheckCollCS(collE, collP))
				{
					isHit = true;
				}
			}
			//ヒットしたなら視野角内にいるかをチェック
			if (!isHit)continue;
			//前ベクトルと相対ベクトルで角度を確認
			auto forward = triggerP->GetViewForward();
			auto pToE = collE->GetRb()->GetNextPos() - collP->GetRb()->GetPos();
			float dis = pToE.Magnitude();
			//角度計算
			auto angle = Vector2::Theata(forward.XZ(), pToE.XZ());
			//視野角内にいたなら
			if (abs(angle) > triggerP->GetViewAngle())continue;
			//最短距離なら
			if (shortDis == 0.0f || shortDis > dis)
			{
				shortDis = dis;
				//ターゲット
				target = collE->GetRb()->GetNextPos();
				//ターゲットの座標を保存
				triggerP->OnHitTarget(target);
			}
		}
	}
}
