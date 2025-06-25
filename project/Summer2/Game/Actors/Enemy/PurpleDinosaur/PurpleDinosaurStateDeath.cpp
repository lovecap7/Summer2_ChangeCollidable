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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Death";
}

PurpleDinosaurStateDeath::PurpleDinosaurStateDeath(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	m_owner.lock()->SetCollState(CollisionState::Dead);
	//���S
	m_owner.lock()->GetModel()->SetAnim(kAnim, false);
}

PurpleDinosaurStateDeath::~PurpleDinosaurStateDeath()
{
}

void PurpleDinosaurStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateDeath::Update(const std::weak_ptr<Camera> camera)
{
	//�A�j���[�V�����I����
	if (m_owner.lock()->GetModel()->IsFinishAnim())
	{
		m_owner.lock()->Delete();//�폜
	}
	//����
	SpeedDown();
}

void PurpleDinosaurStateDeath::SpeedDown()
{
	auto collidable = m_owner.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
