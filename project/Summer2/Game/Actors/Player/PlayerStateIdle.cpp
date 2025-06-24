#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
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
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "Player|Idle";//�ҋ@
}

PlayerStateIdle::PlayerStateIdle(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//�ҋ@���
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
	Vector3 vec = collidable->GetRb()->GetVec();
	//�������Ă��邩�`�F�b�N
	if (vec.y <= Gravity::kChangeStateFall)
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
	//���͂�����Ȃ�ړ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�ړ�
		ChangeState(std::make_shared<PlayerStateWalk>(m_player));
		return;
	}
	//��������������
	SpeedDown();
}

void PlayerStateIdle::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
