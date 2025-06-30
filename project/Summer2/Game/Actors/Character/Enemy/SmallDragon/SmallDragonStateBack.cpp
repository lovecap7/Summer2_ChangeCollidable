#include "SmallDragonStateBack.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
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
	//�v���C���[�퓬��ԂɂȂ鋗��
	constexpr float kBackDistance = 300.0f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//�v���C���[���痣��鑬�x
	constexpr float kBackSpeed = 1.0f;
}

SmallDragonStateBack::SmallDragonStateBack(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Move);
}

SmallDragonStateBack::~SmallDragonStateBack()
{
}

void SmallDragonStateBack::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateBack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
		//�����鋗��
		if (targetData.targetDis <= kBackDistance)
		{
			//������
			coll->GetRb()->SetMoveVec(targetData.targetDirXZ * -kBackSpeed);
		}
		else
		{
			//�ҋ@
			ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
			return;
		}
	}
	else
	{
		//�ҋ@
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}
}