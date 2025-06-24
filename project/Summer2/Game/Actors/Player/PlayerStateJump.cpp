#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
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
	//ジャンプ力
	constexpr float kJumpPower = 13.0f;
	//空中移動速度
	constexpr float kMaxAirMoveSpeed = 10.00f;
	//アニメーション
	const char* kAnim = "Player|Jump";
	//移動速度
	constexpr float kLowAirMoveSpeed = 0.1f;//空中の小移動速度
	constexpr float kMediumAirMoveSpeed = 0.2f;//空中の中移動速度
	constexpr float kHighAirMoveSpeed = 0.5f;//空中の大移動速度
}

PlayerStateJump::PlayerStateJump(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//ジャンプ
	m_player->GetModel()->SetAnim(kAnim, true);
	auto collidable = m_player->GetCollidable();
	collidable->SetState(State::Jump);
	//地面から離れるのでfalseにしておく
	m_player->NoIsGround();
	//力を与える
	collidable->GetRb()->SetVecY(kJumpPower);
}

PlayerStateJump::~PlayerStateJump()
{
}
void PlayerStateJump::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateJump::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死亡
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//攻撃を受けた時
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<PlayerStateHit>(m_player));
		return;
	}
	auto rb = m_player->GetCollidable()->GetRb();
	//落下しているなら
	if (rb->GetVec().y < 0.0f)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//空中移動
		rb->AddVec(GetForwardVec(input, camera) * InputValueSpeed(input));
		//横移動速度に上限をつける
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
	
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
}

float PlayerStateJump::InputValueSpeed(const Input& input)
{
	float moveSpeed = 0.0f;
	//速度をスティック入力の深度に合わせる
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

