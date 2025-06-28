#include "BossDragonStateDeath.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../Game/Camera/Camera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Death";
}

BossDragonStateDeath::BossDragonStateDeath(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡状態にする
	coll->SetCollState(CollisionState::Dead);
	//死亡
	coll->GetModel()->SetAnim(kAnim, false);
}

BossDragonStateDeath::~BossDragonStateDeath()
{
}

void BossDragonStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateDeath::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//削除
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
