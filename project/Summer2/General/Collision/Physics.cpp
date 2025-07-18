#include "Physics.h"
#include "CollisionChecker.h"
#include "FixNextPosition.h"
#include "../Rigidbody.h"
#include "../game.h"
#include <cassert>

namespace
{
	//確認回数
	constexpr int kTryNum = 30;
}

void Physics::Init()
{
	m_isUpdate = true;
	m_delayFrame = 0;
	m_collChecker = std::make_shared<CollisionChecker>();
	m_collProcessor = std::make_shared<FixNextPosition>();
}

void Physics::Entry(std::shared_ptr<Collidable> collidable)
{
	//すでに登録されているならしない
	auto it = std::find(m_collidables.begin(), m_collidables.end(), collidable);
	if (it != m_collidables.end())return;
	//コライダーの追加
	m_collidables.emplace_back(collidable);
}

void Physics::Exit(std::shared_ptr<Collidable> collidable)
{
	//登録されていないならしない
	auto it = std::find(m_collidables.begin(), m_collidables.end(), collidable);
	if (it == m_collidables.end())return;
	m_collidables.erase(it);
}

void Physics::Update()
{
	//遅延処理
	if (m_delayFrame > 0)
	{
		--m_delayFrame;
		if (m_delayFrame <= 0)
		{
			m_isUpdate = true;
		}
	}
	//更新をしないなら
	if (!m_isUpdate || m_delayFrame > 0)return;
	//重力
	Gravity();
	//床と壁のとの当たったフラグを初期化
	for (auto& collidable : m_collidables)
	{
		collidable->ResetHitFlag();
	}
	//遅延処理用
	std::list<OnCollideInfo> onCollideInfo;
	//一度も当たっていないのならループを終了する
	bool isOneMore = false;
	//補正したことで別のオブジェクトに当たる可能性があるので一定回数チャックする
	for (int i = 0;i < kTryNum;++i)
	{
		isOneMore = false;
		//当たり判定をチェック
		for (auto& collA : m_collidables)
		{
			//当たり判定を行わないなら飛ばす
			if (collA->GetGameTag() == GameTag::None)continue;
			if (collA->m_isThrough)continue;
			for (auto& collB : m_collidables)
			{
				//自分とは当たり判定をしない
				if (collA == collB)continue;
				//当たり判定を行わないなら飛ばす
				if (collB->GetGameTag() == GameTag::None)continue;
				if (collB->m_isThrough)continue;
				//当たってるなら
				if (m_collChecker->IsCollide(collA, collB))
				{
					//どちらもトリガーなではないなら
					if (!collA->m_isTrigger && !collB->m_isTrigger)
					{
						//衝突処理
						m_collProcessor->FixNextPos(collA, collB);
						//変更後の位置で再度ほかのコライダブルに当たる可能性があるので
						//もう一度チェックする必要がある
						isOneMore = true;
					}
					//これまでにこの組み合わせで当たった情報があるかをチェック
					bool isCollInfo = false;
					for (const auto& item : onCollideInfo)
					{
						// 既に通知リストに含まれていたら呼ばない
						if (item.owner == collA && item.colider == collB ||
							item.owner == collB && item.colider == collA)
						{
							isCollInfo = true;
						}
					}
					//ない場合
					if (!isCollInfo)
					{
						onCollideInfo.emplace_back(OnCollideInfo{ collA, collB });
						onCollideInfo.emplace_back(OnCollideInfo{ collB, collA });
					}
				}
			}
		}
		//チェックの必要がないなら
		if (!isOneMore)break;
	}
	//位置確定
	for (auto& coll : m_collidables)
	{
		//位置を確定
		coll->Complete();
	}
	// 当たり通知
	for (auto& collInfo : onCollideInfo)
	{
		collInfo.OnCollide();
	}
}

void Physics::Reset()
{
	m_collidables.clear();
}

void Physics::DelayUpdate(int frame)
{
	m_delayFrame = frame;
	m_isUpdate = false;
}

std::list<std::weak_ptr<Collidable>> Physics::GetAreaXCollidable(float startX, float endX)
{
	std::list<std::weak_ptr<Collidable>> collList;
	for (auto& collidable : m_collidables)
	{
		if (collidable->GetGameTag() == GameTag::None)continue;
		auto collPos = collidable->m_rb->m_pos;
		//範囲内にいたら
		if (collPos.x > startX && collPos.x < endX)
		{
			collList.emplace_back(collidable);
		}
	}
	return collList;
}

void Physics::Gravity()
{
	for (auto& collidable : m_collidables)
	{
		//重力を受けるか
		if (!collidable->m_rb->m_isGravity)continue;
		auto rb = collidable->m_rb;
		//地上にいるときと空中にいるときで重力の大きさを変える
		auto gravity = Gravity::kGroundGravity;
		if (!collidable->IsFloor())//地上にいない場合
		{
			gravity = Gravity::kAirGravity;
		}
		//重力をかける
		rb->m_vec += gravity;
		//重力に上限をつける
		if (rb->m_vec.y < Gravity::kMaxGravityY)
		{
			//上限を超えないようにする
			rb->m_vec.y = Gravity::kMaxGravityY;
		}
	}
}