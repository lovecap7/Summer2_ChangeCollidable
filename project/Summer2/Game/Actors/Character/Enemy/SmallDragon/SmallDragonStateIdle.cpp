#include "SmallDragonStateIdle.h"
#include "SmallDragonStateAttack.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateBack.h"
#include "SmallDragon.h"
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
	//��ނ��鋗��
	constexpr float kBackDistance = 300.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Flying_Idle";//�ҋ@
}

SmallDragonStateIdle::SmallDragonStateIdle(std::weak_ptr<Actor> owner):
	SmallDragonStateBase(owner)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

SmallDragonStateIdle::~SmallDragonStateIdle()
{
}

void SmallDragonStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
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
			//�U��
			ChangeState(std::make_shared<SmallDragonStateAttack>(m_owner,actorManager));
			return;
		}
		//�����鋗��
		if (targetData.targetDis <= kBackDistance)
		{
			//������
			ChangeState(std::make_shared<SmallDragonStateBack>(m_owner));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}