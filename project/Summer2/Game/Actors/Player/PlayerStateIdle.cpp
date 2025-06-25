#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"

#include "PlayerStateRolling.h"

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


namespace
{
	//������
	constexpr float kMoveDeceRate = 0.8f;
	//�A�j���[�V����
	const char* kAnim = "Player|Idle";//�ҋ@
}

PlayerStateIdle::PlayerStateIdle(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//�ҋ@���
	m_player.lock()->GetModel()->SetAnim(kAnim, true);
	m_player.lock()->SetCollState(CollisionState::Normal);
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	auto collidable = m_player.lock();
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
	if (input.IsTrigger("RB") && m_player.lock()->GetUltGage()->IsMaxUlt())
	{
		//�K�E�Z
		//ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}

	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && m_player.lock()->IsFloor())
	{
		//�W�����v
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
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
	auto collidable = m_player.lock();
	//����
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
