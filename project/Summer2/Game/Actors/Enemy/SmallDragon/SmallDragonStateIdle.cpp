#include "SmallDragonStateIdle.h"
#include "SmallDragonStateAttack.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateBack.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBackDistance = 300.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Flying_Idle";//�ҋ@
}

SmallDragonStateIdle::SmallDragonStateIdle(std::shared_ptr<SmallDragon> owner):
	SmallDragonStateBase(owner)
{
	//�ҋ@���
	m_owner->GetModel()->SetAnim(kAnim, true);
	m_owner->GetCollidable()->SetState(State::None);
}

SmallDragonStateIdle::~SmallDragonStateIdle()
{
}

void SmallDragonStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}

	//����
	SpeedDown();
	//�v���C���[�𔭌������Ƃ�
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//�v���C���[������
		LookPlayer();
		//�U���̃N�[���^�C����0�Ȃ�
		if (m_owner->GetAttackCoolTime() <= 0)
		{
			//�U����Ԃɂ���
			ChangeState(std::make_shared<SmallDragonStateAttack>(m_owner));
		}
		//�������`�F�b�N
		float dist = m_owner->GetPlayerVec().Magnitude();
		//�G�Ƃ̋������߂��Ƃ�
		if (dist <= kBackDistance)
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<SmallDragonStateBack>(m_owner));
		}
	}
}

void SmallDragonStateIdle::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
