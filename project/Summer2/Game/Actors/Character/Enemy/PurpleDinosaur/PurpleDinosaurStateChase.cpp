#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
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
	//�v���C���[�ɍU�����鋗��
	constexpr float kAttackDistance = 200.0f;
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Run";
	//�v���C���[��ǂ������鑬�x
	constexpr float kChaseSpeed = 5.0f;
}

PurpleDinosaurStateChase::PurpleDinosaurStateChase(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//�A�j���[�V����
	coll->GetModel()->SetAnim(kAnim, true);
}

PurpleDinosaurStateChase::~PurpleDinosaurStateChase()
{
}

void PurpleDinosaurStateChase::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateChase::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�R���C�_�u��
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
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
				ChangeState(std::make_shared<PurpleDinosaurStateAttack>(m_owner));
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
	ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
	return;
}
