#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../Actor.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../Game/Camera/Camera.h"

namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "CharacterArmature|Death";
}

SmallDragonStateDeath::SmallDragonStateDeath(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//���S��Ԃɂ���
	coll->SetCollState(CollisionState::Dead);
	//���S
	coll->GetModel()->SetAnim(kAnim, false);
}

SmallDragonStateDeath::~SmallDragonStateDeath()
{
}

void SmallDragonStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void SmallDragonStateDeath::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//�폜
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

