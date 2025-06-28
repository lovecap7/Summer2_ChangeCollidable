#include "BossDragonStateDeath.h"
#include "BossDragon.h"
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

BossDragonStateDeath::BossDragonStateDeath(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//���S��Ԃɂ���
	coll->SetCollState(CollisionState::Dead);
	//���S
	coll->GetModel()->SetAnim(kAnim, false);
}

BossDragonStateDeath::~BossDragonStateDeath()
{
}

void BossDragonStateDeath::Init()
{
	//���̏�Ԃ����̏�ԂɍX�V
	ChangeState(shared_from_this());
}

void BossDragonStateDeath::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//�A�j���[�V�����I����
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//�폜
	}
	//����
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
