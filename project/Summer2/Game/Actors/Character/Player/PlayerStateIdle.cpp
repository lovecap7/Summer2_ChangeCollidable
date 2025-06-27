#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateRolling.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateDeath.h"
#include "PlayerStateHit.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
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
	const char* kAnim = "Player|Idle";//�ҋ@
}

PlayerStateIdle::PlayerStateIdle(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//�ҋ@���
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	Vector3 vec = coll->GetRb()->GetVec();
	//���S�����Ȃ�
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//�������Ă��邩�`�F�b�N
	if (vec.y <= Gravity::kChangeStateFall)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//����{�^������������
	if (input.IsTrigger("A"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//�Q�[�W������Ƃ��g����
	if (input.IsTrigger("RB") && coll->GetUltGage()->IsMaxUlt())
	{
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_owner));
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && coll->IsFloor())
	{
		//�W�����v
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	if (input.IsTrigger("X"))
	{
		//�U��
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_owner));
		return;
	}
	if (input.IsTrigger("Y"))
	{
		//�`���[�W
		ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
		return;
	}
	//���͂�����Ȃ�ړ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		ChangeState(std::make_shared<PlayerStateWalk>(m_owner));
		return;
	}
	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
