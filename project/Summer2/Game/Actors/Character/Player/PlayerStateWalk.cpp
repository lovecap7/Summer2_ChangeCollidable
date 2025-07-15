#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateNA.h"
#include "PlayerStateRolling.h"
#include "PlayerStateRun.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"


namespace
{
	//�ړ����x
	constexpr float kLowMoveSpeed = 2.0f;//�n��̏��ړ����x
	constexpr float kMediumMoveSpeed = 5.0f;//�n��̒��ړ����x
	constexpr float kHighMoveSpeed = 10.0f;//�n��̑�ړ����x
	//�A�j���[�V����
	const char* kAnim = "Player|Walk";
}

PlayerStateWalk::PlayerStateWalk(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//�������
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Move);
}

PlayerStateWalk::~PlayerStateWalk()
{
}
void PlayerStateWalk::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateWalk::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//�{�X�����S�ɏ��ł����Ƃ�
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//�{�X�̗̑͂��Ȃ��Ȃ����ꍇ�܂��͓��͂��Ȃ��Ȃ�
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead() ||
		!input.GetStickInfo().IsLeftStickInput())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//�U�����󂯂��Ȃ�
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//�������Ă��邩�`�F�b�N
	if (coll->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
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
	if (input.IsTrigger("RB") && coll->GetUltGage().lock()->IsMaxUlt())
	{
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_owner, actorManager));
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
		ChangeState(std::make_shared<PlayerStateNA>(m_owner, actorManager));
		return;
	}
	if (input.IsTrigger("Y"))
	{
		//�`���[�W
		ChangeState(std::make_shared<PlayerStateCharge>(m_owner, actorManager));
		return;
	}
	//�_�b�V��
	if (input.IsPress("LS") || coll->IsRunKeep())
	{
		ChangeState(std::make_shared<PlayerStateRun>(m_owner));
		return;
	}

	auto rb = coll->GetRb();
	//�ړ�
	rb->SetMoveVec(GetForwardVec(camera) * InputValueSpeed(input));
	//�����̍X�V
	Vector2 dir = coll->GetStickVec();
	coll->GetModel()->SetDir(dir);
}


float PlayerStateWalk::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//���x���X�e�B�b�N���͂̐[�x�ɍ��킹��
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighMoveSpeed;
	}
	return moveSpeed;
}