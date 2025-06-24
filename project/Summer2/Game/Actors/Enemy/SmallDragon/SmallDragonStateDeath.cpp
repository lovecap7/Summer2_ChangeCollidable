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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Death";
}


SmallDragonStateDeath::SmallDragonStateDeath(std::shared_ptr<SmallDragon> owner):
	SmallDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::Dead);
	//���S
	m_owner->GetModel()->SetAnim(kAnim, false);
}

SmallDragonStateDeath::~SmallDragonStateDeath()
{
}

void SmallDragonStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateDeath::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		m_owner->Delete();//�폜
	}
	//����
	SpeedDown();
}

void SmallDragonStateDeath::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
