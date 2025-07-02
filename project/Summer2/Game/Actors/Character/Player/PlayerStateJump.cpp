#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
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

PlayerStateJump::PlayerStateJump(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//�W�����v
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Jump);
	//�n�ʂ��痣���̂�false�ɂ��Ă���
	coll->SetIsFloor(false);
	//�͂�^����
	coll->GetRb()->SetVecY(kJumpPower);
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//���̏�Ԃ������̏�Ԃ�����
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto rb = coll->GetRb();
	//���S�������{�X���|���ĂȂ��ꍇ
	if (coll->GetHitPoints().lock()->IsDead() && !actorManager.lock()->GetBoss().expired())
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
	//�������Ă���Ȃ�
	if (rb->GetVec().y < 0.0f)
	{
		//����
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
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
	coll->GetModel()->SetDir(coll->GetStickVec());
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

