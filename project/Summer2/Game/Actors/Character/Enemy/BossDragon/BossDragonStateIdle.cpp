#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateChase.h"
#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateSweepAttack.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Flying_Idle";//�ҋ@
}

BossDragonStateIdle::BossDragonStateIdle(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

BossDragonStateIdle::~BossDragonStateIdle()
{
}

void BossDragonStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	auto targetData = coll->GetTargetData();
	//�v���C���[���������Ȃ�
	if (targetData.isHitTarget)
	{
		//�v���C���[������
		coll->LookAtTarget();
		//�U���̃N�[���^�C����0�Ȃ�
		if (coll->GetAttackCoolTime() <= 0)
		{
			//�U����Ԃɂ���
			ThinkAttack();
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
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
