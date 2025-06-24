#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateChase.h"
#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateSweepAttack.h"
#include "BossDragon.h"
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

BossDragonStateIdle::BossDragonStateIdle(std::shared_ptr<BossDragon> owner):
	BossDragonStateBase(owner)
{
	//�ҋ@���
	m_owner->GetModel()->SetAnim(kAnim, true);
	m_owner->GetCollidable()->SetState(State::None);
}

BossDragonStateIdle::~BossDragonStateIdle()
{
}

void BossDragonStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//����ł�Ȃ�
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//���S
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
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
			//�ǂ̍U�������邩�l����
			ThinkAttack();
			return;
		}
	}
}

void BossDragonStateIdle::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void BossDragonStateIdle::ThinkAttack()
{
	//�����_���Ɍ���
	auto rand = GetRand(2);

	switch (rand)
	{
	case 0:
		//�v���C���[���ɋ߂Â�
		ChangeState(std::make_shared<BossDragonStateChase>(m_owner));
		break;
	case 1:
		//�ガ����
		ChangeState(std::make_shared<BossDragonStateSweepAttack>(m_owner));
		break;
	case 2:
		//�u���X
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner));
		break;
	default:
		//�u���X
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner));
		break;
	}
	return;
}
