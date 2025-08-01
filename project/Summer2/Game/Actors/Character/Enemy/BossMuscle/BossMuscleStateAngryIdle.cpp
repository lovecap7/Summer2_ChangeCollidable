#include "BossMuscleStateAngryIdle.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscle.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "Boss|Idle2";//待機
}

BossMuscleStateAngryIdle::BossMuscleStateAngryIdle(std::weak_ptr<Actor> owner) :
	BossMuscleStateBase(owner)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

BossMuscleStateAngryIdle::~BossMuscleStateAngryIdle()
{
}

void BossMuscleStateAngryIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateAngryIdle::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//プレイヤーを見つけたなら
	if (targetData.isHitTarget)
	{
		//プレイヤーを見る
		coll->LookAtTarget();
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
