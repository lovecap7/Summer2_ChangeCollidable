#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../Actor.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Death";
}


SmallDragonStateDeath::SmallDragonStateDeath(std::shared_ptr<SmallDragon> owner):
	SmallDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::Dead);
	//死亡
	m_owner->GetModel()->SetAnim(kAnim, false);
}

SmallDragonStateDeath::~SmallDragonStateDeath()
{
}

void SmallDragonStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateDeath::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		m_owner->Delete();//削除
	}
	//減速
	SpeedDown();
}

void SmallDragonStateDeath::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
