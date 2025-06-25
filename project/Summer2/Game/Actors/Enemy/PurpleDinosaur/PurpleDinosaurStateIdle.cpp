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
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBattleDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	//�ҋ@���
	m_owner.lock()->GetModel()->SetAnim(kAnim, true);
	m_owner.lock()->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<Camera> camera)
{
	
}

void PurpleDinosaurStateIdle::SpeedDown()
{
	auto collidable = m_owner.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
