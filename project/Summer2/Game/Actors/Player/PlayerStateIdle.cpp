#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateRolling.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
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
	auto coll = m_player.lock();
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
	auto coll = m_player.lock();
	Vector3 vec = coll->GetRb()->GetVec();
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
	if (input.IsTrigger("RB") && coll->GetUltGage()->IsMaxUlt())
	{
		//�K�E�Z
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player));
		return;
	}

	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && coll->IsFloor())
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
	//���񂾂񌸑�
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
