#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../ActorManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|HitReact";//����炢
}

PurpleDinosaurStateHit::PurpleDinosaurStateHit(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	//����
	m_owner.lock()->GetModel()->SetAnim(kAnim, false);
}

PurpleDinosaurStateHit::~PurpleDinosaurStateHit()
{
}

void PurpleDinosaurStateHit::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateHit::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = m_owner.lock();
	//���f���̃A�j���[�V�������I�������
	if (coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}