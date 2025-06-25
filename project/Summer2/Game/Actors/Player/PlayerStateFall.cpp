#include "PlayerStateFall.h"
#include "PlayerStateIdle.h"

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
	//�A�j���[�V����
	const char* kAnim = "Player|Fall";
	//�󒆈ړ����x
	constexpr float kMaxAirMoveSpeed = 10.00f;
	//�ړ����x
	constexpr float kLowAirMoveSpeed = 0.1f;//�󒆂̏��ړ����x
	constexpr float kMediumAirMoveSpeed = 0.2f;//�󒆂̒��ړ����x
	constexpr float kHighAirMoveSpeed = 0.5f;//�󒆂̑�ړ����x
}

PlayerStateFall::PlayerStateFall(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//����
	auto coll = m_player.lock();
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetIsFloor(false); //�n�ʂɂ��Ȃ�
	coll->SetCollState(CollisionState::Fall);
}
PlayerStateFall::~PlayerStateFall()
{
}
void PlayerStateFall::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateFall::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	//�n�ʂɕt���Ă���Ȃ�
	if (m_player.lock()->IsFloor())
	{
		//�ҋ@
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = m_player.lock()->GetRb();
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


float PlayerStateFall::InputValueSpeed(const Input& input)
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