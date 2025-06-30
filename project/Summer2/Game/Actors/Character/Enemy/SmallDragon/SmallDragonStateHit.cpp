#include "SmallDragonStateHit.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateDeath.h"
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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|HitReact";//����炢
}

SmallDragonStateHit::SmallDragonStateHit(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//����
	coll->GetModel()->SetAnim(kAnim, false);
}

SmallDragonStateHit::~SmallDragonStateHit()
{
}

void SmallDragonStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateHit::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//���S
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	auto model = coll->GetModel();
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		//���߂���
		model->ReplayAnim();
	}
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}