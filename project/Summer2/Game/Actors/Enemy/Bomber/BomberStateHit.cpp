#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "BomberStateDeath.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../Attack/HurtPoint.h"
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
	const char* kAnim = "CharacterArmature|HitReact";//����炢
}

BomberStateHit::BomberStateHit(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	//����
	m_owner->GetModel()->SetAnim(kAnim, false);
}

BomberStateHit::~BomberStateHit()
{
}

void BomberStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}

	//���ꃊ�A�N�V�������ɍU����H�������A�j���[�V���������߂���
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//���v���C
		m_owner->GetModel()->ReplayAnim();
	}

	//���f���̃A�j���[�V�������I�������
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//����
	SpeedDown();
}

void BomberStateHit::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
