#include "BossMuscleStateJumpAttack.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../ActorManager.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

BossMuscleStateJumpAttack::BossMuscleStateJumpAttack(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kJumpAttackName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

BossMuscleStateJumpAttack::~BossMuscleStateJumpAttack()
{
}

void BossMuscleStateJumpAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateJumpAttack::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner, m_isAngry));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
