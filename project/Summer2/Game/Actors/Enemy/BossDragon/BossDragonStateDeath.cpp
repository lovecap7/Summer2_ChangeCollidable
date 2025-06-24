#include "BossDragonStateDeath.h"
#include "BossDragon.h"
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

BossDragonStateDeath::BossDragonStateDeath(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::Dead);
	//���S
	m_owner->GetModel()->SetAnim(kAnim, false);
}

BossDragonStateDeath::~BossDragonStateDeath()
{
}

void BossDragonStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateDeath::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//�A�j���[�V�����I����
	if (m_owner->GetModel()->IsFinishAnim())
	{
		m_owner->Delete();//�폜
	}
	//����
	SpeedDown();
}

void BossDragonStateDeath::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
