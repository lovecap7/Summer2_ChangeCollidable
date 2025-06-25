#include "PlayerStateJump.h"
#include "PlayerStateFall.h"

#include "Player.h"
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
	//�W�����v��
	constexpr float kJumpPower = 13.0f;
	//�󒆈ړ����x
	constexpr float kMaxAirMoveSpeed = 10.00f;
	//�A�j���[�V����
	const char* kAnim = "Player|Jump";
	//�ړ����x
	constexpr float kLowAirMoveSpeed = 0.1f;//�󒆂̏��ړ����x
	constexpr float kMediumAirMoveSpeed = 0.2f;//�󒆂̒��ړ����x
	constexpr float kHighAirMoveSpeed = 0.5f;//�󒆂̑�ړ����x
}

PlayerStateJump::PlayerStateJump(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//�W�����v
	auto collidable = m_player.lock();
	collidable->GetModel()->SetAnim(kAnim, true);
	collidable->SetCollState(CollisionState::Jump);
	//�n�ʂ��痣���̂�false�ɂ��Ă���
	collidable->SetIsFloor(false);
	//�͂�^����
	collidable->GetRb()->SetVecY(kJumpPower);
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	auto rb = m_player.lock()->GetRb();
	//�������Ă���Ȃ�
	if (rb->GetVec().y < 0.0f)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//�ړ��̓��͂�����Ȃ�
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//�󒆈ړ�
		rb->AddVec(GetForwardVec(camera) * InputValueSpeed(input));
		//���ړ����x�ɏ��������
		float speed = rb->GetMoveVec().Magnitude();
		if (speed > 0.0f)
		{
			speed = MathSub::ClampFloat(speed, 0.0f, kMaxAirMoveSpeed);
			Vector3 moveVec = rb->GetMoveVec();
			if (moveVec.Magnitude() > 0.0f)
			{
				moveVec = moveVec.Normalize();
			}
			rb->SetMoveVec(moveVec * speed);
		}
	}
	
	//�����̍X�V
	Vector2 dir = m_player.lock()->GetStickVec();
	m_player.lock()->GetModel()->SetDir(dir);
}

float PlayerStateJump::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//���x���X�e�B�b�N���͂̐[�x�ɍ��킹��
	if (input.IsLowPowerLeftStick())
	{
		moveSpeed = kLowAirMoveSpeed;
	}
	else if (input.IsMediumPowerLeftStick())
	{
		moveSpeed = kMediumAirMoveSpeed;
	}
	else if (input.IsHighPowerLeftStick())
	{
		moveSpeed = kHighAirMoveSpeed;
	}
	return moveSpeed;
}

