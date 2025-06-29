#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateAttack.h"
#include "Bomber.h"
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
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kAttackDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V�����̖��O
	const char* kAnim = "CharacterArmature|Idle";//�ҋ@
}

BomberStateIdle::BomberStateIdle(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

BomberStateIdle::~BomberStateIdle()
{
}

void BomberStateIdle::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//���S
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}

	auto targetData = coll->GetTargetData();
	//�v���C���[���������Ȃ�
	if (targetData.isHitTarget)
	{
		//�v���C���[������
		coll->LookAtTarget();
		//�U���̋���
		if (targetData.targetDis <= kAttackDistance)
		{
			//�U���̃N�[���^�C����0�Ȃ�
			if (coll->GetAttackCoolTime() <= 0)
			{
				//�U����Ԃɂ���
				ChangeState(std::make_shared<BomberStateAttack>(m_owner));
				return;
			}
		}
		//�U���̋�������Ȃ��Ƃ��͋߂Â�
		else
		{
			//�v���C���[���ɋ߂Â�
			ChangeState(std::make_shared<BomberStateChase>(m_owner));
			return;
		}
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}