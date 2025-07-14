#include "PlayerStateRolling.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../ActorManager.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//��𑬓x
	constexpr float kRollingMoveSpeed = 10.0f;
	//�A�j���[�V����
	const char* kAnim = "Player|Rolling";
	//������[�V�����̑��x
	constexpr float kAnimSpeed = 1.2f;
}

PlayerStateRolling::PlayerStateRolling(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//������
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	coll->SetCollState(CollisionState::Move);
	//�����̍X�V
	coll->GetModel()->SetDir(coll->GetStickVec());
	//���G
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
}

PlayerStateRolling::~PlayerStateRolling()
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//���G����
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}
void PlayerStateRolling::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateRolling::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X�����S�ɏ��ł����Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��̓��f���̃A�j���[�V�������I�������
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead() ||
		coll->GetModel()->IsFinishAnim())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	auto rb = coll->GetRb();
	//�����Ă�����Ɉړ�
	rb->SetMoveVec(coll->GetModel()->GetDir() * kRollingMoveSpeed);
}
