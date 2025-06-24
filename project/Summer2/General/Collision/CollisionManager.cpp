#include "CollisionManager.h"
#include "../../Game/Actors/Actor.h"
#include "CollisionChecker.h"
#include "CollisionProcess.h"
#include "../Rigidbody.h"
#include "ColliderBase.h"
#include "../Collidable.h"

namespace
{
	//確認回数
	constexpr int kTryNum = 30;
}

CollisionManager::CollisionManager():
	m_collChecker(std::make_shared<CollisionChecker>()),
	m_collProcessor(std::make_shared<CollisionProcess>())
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::Update(std::vector<std::shared_ptr<Actor>> actors)
{
	//一度も当たっていないのならループを終了する
	bool isOneMore = false;
	//補正したことで別のオブジェクトに当たる可能性があるので一定回数チャックする
	for (int i = 0;i < kTryNum;++i)
	{
		isOneMore = false;
		//当たり判定をチェック
		for (auto& actorA : actors)
		{
			//描画用のクラスは無視
			if (actorA->IsDrawOnly())continue;
			auto collA = actorA->GetCollidable();

			//当たり判定を行わないなら飛ばす
			if (collA->GetGameTag() == GameTag::None)continue;
			if (!collA->IsCollide())continue;

			for (auto& actorB : actors)
			{
				//描画用のクラスは無視
				if (actorB->IsDrawOnly())continue;
				auto collB = actorB->GetCollidable();

				//当たり判定を行わないなら飛ばす
				if (collB->GetGameTag() == GameTag::None)continue;
				if (!collB->IsCollide())continue;

				//自分とは当たり判定をしない
				if (actorA == actorB)continue;

				//当たってるかをチェック
				bool isHit = false;

				//球と
				if (collA->GetColl()->GetShape() == Shape::Sphere)
				{
					//球
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSS(collA, collB);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSS(collA, collB);
						}
					}
					//カプセル
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCS(collB, collA);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCS(collB, collA);
						}
					}
					//ポリゴン
					else if (collB->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollSP(collA, collB);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSP(collA, collB);
						}
					}
				}
				//カプセルと
				else if (collA->GetColl()->GetShape() == Shape::Capsule)
				{
					//球
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollCS(collA, collB);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCS(collA, collB);
						}
					}
					//カプセル
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCC(collA, collB);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCC(collA, collB);
						}
					}
					//ポリゴン
					else if (collB->GetColl()->GetShape() == Shape::Polygon)
					{
						isHit = m_collChecker->CheckCollCP(collA, collB);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCP(collA, collB);
						}
					}
				}
				//ポリゴンと
				else if (collA->GetColl()->GetShape() == Shape::Polygon)
				{
					//球
					if (collB->GetColl()->GetShape() == Shape::Sphere)
					{
						isHit = m_collChecker->CheckCollSP(collB, collA);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessSP(collB, collA);
						}
					}
					//カプセル
					else if (collB->GetColl()->GetShape() == Shape::Capsule)
					{
						isHit = m_collChecker->CheckCollCP(collB, collA);
						if (isHit)
						{
							//ベクトルを補正する
							m_collProcessor->ProcessCP(collB, collA);
						}
					}
				}

				//当たってるなら
				if (isHit)
				{
					//当たった時の処理
					actorA->OnHitColl(collB);
					actorB->OnHitColl(collA);
					isOneMore = true;
				}
			}
		}
		
		//チェックの必要がないなら
		if (!isOneMore)break;
	}
}
