#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaur.h"
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

PurpleDinosaurStateDeath::PurpleDinosaurStateDeath(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	m_owner.lock()->SetCollState(CollisionState::Dead);
	//死亡
	m_owner.lock()->GetModel()->SetAnim(kAnim, false);
}

PurpleDinosaurStateDeath::~PurpleDinosaurStateDeath()
{
}

void PurpleDinosaurStateDeath::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateDeath::Update(const std::weak_ptr<Camera> camera)
{
	//アニメーション終了後
	if (m_owner.lock()->GetModel()->IsFinishAnim())
	{
		m_owner.lock()->Delete();//削除
	}
	//減速
	SpeedDown();
}

void PurpleDinosaurStateDeath::SpeedDown()
{
	auto collidable = m_owner.lock();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
