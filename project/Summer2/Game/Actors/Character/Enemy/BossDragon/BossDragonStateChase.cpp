#include "BossDragonStateChase.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
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
	constexpr float kAttackDistance = 120.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 8.0f;
}


BossDragonStateChase::BossDragonStateChase(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
}

BossDragonStateChase::~BossDragonStateChase()
{
}

void BossDragonStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateChase::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//�v���C���[
	auto player = actorManager.lock()->GetPlayer();

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
				ChangeState(std::make_shared<BossDragonStatePunchAttack>(m_owner, actorManager));
				return;
			}
		}
		//�˒��͈͊O�Ȃ̂�
		else
		{
			//�v���C���[���ɋ߂Â�
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * kChaseSpeed);
			return;
		}
	}
	//�ҋ@���
	ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
	return;
}
