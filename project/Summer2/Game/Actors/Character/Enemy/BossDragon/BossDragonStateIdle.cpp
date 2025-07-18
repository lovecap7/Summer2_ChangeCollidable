#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateChase.h"
#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateSweepAttack.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Flying_Idle";//待機
}

BossDragonStateIdle::BossDragonStateIdle(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

BossDragonStateIdle::~BossDragonStateIdle()
{
}

void BossDragonStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	auto targetData = coll->GetTargetData();
	//プレイヤーを見つけたなら
	if (targetData.isHitTarget)
	{
		//プレイヤーを見る
		coll->LookAtTarget();
		//攻撃のクールタイムが0なら
		if (coll->GetAttackCoolTime() <= 0)
		{
			//攻撃状態にする
			ThinkAttack(actorManager);
			return;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStateIdle::ThinkAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//ランダムに決定
	auto rand = GetRand(2);

	switch (rand)
	{
	case 0:
		//プレイヤーをに近づく
		ChangeState(std::make_shared<BossDragonStateChase>(m_owner));
		break;
	case 1:
		//薙ぎ払い
		ChangeState(std::make_shared<BossDragonStateSweepAttack>(m_owner, actorManager));
		break;
	case 2:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	default:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner, actorManager));
		break;
	}
	return;
}
