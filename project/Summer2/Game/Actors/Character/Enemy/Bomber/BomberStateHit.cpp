#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "BomberStateDeath.h"
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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|HitReact";//����炢
}

BomberStateHit::BomberStateHit(std::weak_ptr<Actor> owner) :
	BomberStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//����
	coll->GetModel()->SetAnim(kAnim, false);
}

BomberStateHit::~BomberStateHit()
{
}

void BomberStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BomberStateHit::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Bomber>(m_owner.lock());
	//���S
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	auto model = coll->GetModel();
	//�q�b�g���A�N�V����
	if (coll->GetHitPoints()->IsHitReaction())
	{
		//���߂���
		model->ReplayAnim();
	}
	//���f���̃A�j���[�V�������I�������
	if (model->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
