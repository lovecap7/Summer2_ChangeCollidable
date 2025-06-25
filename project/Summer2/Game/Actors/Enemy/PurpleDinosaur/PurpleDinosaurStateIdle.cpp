#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"

#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//プレイヤー戦闘状態になる距離
	constexpr float kBattleDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Idle";//待機
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	//待機状態
	m_owner.lock()->GetModel()->SetAnim(kAnim, true);
	m_owner.lock()->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<Camera> camera)
{
	
}

void PurpleDinosaurStateIdle::SpeedDown()
{
	auto collidable = m_owner.lock();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
