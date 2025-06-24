#include "PlayerStateWalk.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateRun.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/HurtPoint.h"

namespace
{
	//�ړ����x
	constexpr float kLowMoveSpeed = 2.0f;//�n��̏��ړ����x
	constexpr float kMediumMoveSpeed = 5.0f;//�n��̒��ړ����x
	constexpr float kHighMoveSpeed = 10.0f;//�n��̑�ړ����x
	//�A�j���[�V����
	const char* kAnim = "Player|Walk";
	//�d�͂��d�߂ɂ���(�⓹�΍�)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}

PlayerStateWalk::PlayerStateWalk(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�������
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateWalk::~PlayerStateWalk()
{
}
void PlayerStateWalk::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateWalk::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//���S
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//�U�����󂯂���
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//������
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto collidable = m_player->GetCollidable();
	//�������Ă��邩�`�F�b�N
	if (collidable->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//����{�^������������
	if (input.IsTrigger("A"))
	{
		//���
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	//�Q�[�W������Ƃ��g����
	if (input.IsTrigger("RB") && m_player->GetUltGage()->IsMaxUlt())
	{
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && m_player->IsGround())
	{
		//�W�����v
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	//��U���{�^������������
	if (input.IsTrigger("X"))
	{
		//��U��
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_player));
		return;
	}
	//�`���[�W�{�^������������
	if (input.IsTrigger("Y"))
	{
		//�`���[�W
		ChangeState(std::make_shared<PlayerStateCharge>(m_player));
		return;
	}
	//���͂��Ȃ��Ȃ�ҋ@
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//�_�b�V��
	if (input.IsPress("LS"))
	{
		ChangeState(std::make_shared<PlayerStateRun>(m_player));
		return;
	}

	auto rb = collidable->GetRb();
	//�d��
	rb->AddVec(kBigGravity);
	//�ړ�
	rb->SetMoveVec(GetForwardVec(input,camera) * InputValueSpeed(input));
	//�����̍X�V
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
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