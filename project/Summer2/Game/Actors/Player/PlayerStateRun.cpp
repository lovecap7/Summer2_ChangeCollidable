#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
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
	//�ړ����x
	constexpr float kMoveSpeed = 15.0f;//�n��̈ړ����x
	//�A�j���[�V����
	const char* kAnim = "Player|Dash";
	//�d�͂��d�߂ɂ���(�⓹�΍�)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}


PlayerStateRun::PlayerStateRun(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//������
	m_player.lock()->GetModel()->SetAnim(kAnim, true);
	m_player.lock()->SetCollState(CollisionState::Normal);
}


PlayerStateRun::~PlayerStateRun()
{
}

void PlayerStateRun::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateRun::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	auto collidable = m_player.lock();
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
	
	//�W�����v�{�^���������Ă�Ȃ�W�����v
	if (input.IsTrigger("B") && collidable->IsFloor())
	{
		//�W�����v
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	
	//���͂��Ȃ��Ȃ�ҋ@
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = collidable->GetRb();
	//�d��
	rb->AddVec(kBigGravity);
	//�ړ�
	rb->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
	//�����̍X�V
	Vector2 dir = collidable->GetStickVec();
	collidable->GetModel()->SetDir(dir);
}
