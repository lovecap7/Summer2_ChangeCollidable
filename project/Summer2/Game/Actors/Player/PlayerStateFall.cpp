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
	//アニメーション
	const char* kAnim = "Player|Fall";
	//空中移動速度
	constexpr float kMaxAirMoveSpeed = 10.00f;
	//移動速度
	constexpr float kLowAirMoveSpeed = 0.1f;//空中の小移動速度
	constexpr float kMediumAirMoveSpeed = 0.2f;//空中の中移動速度
	constexpr float kHighAirMoveSpeed = 0.5f;//空中の大移動速度
}

PlayerStateFall::PlayerStateFall(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//落下
	auto coll = m_player.lock();
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetIsFloor(false); //地面にいない
	coll->SetCollState(CollisionState::Fall);
}
PlayerStateFall::~PlayerStateFall()
{
}
void PlayerStateFall::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateFall::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	//地面に付いているなら
	if (m_player.lock()->IsFloor())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = m_player.lock()->GetRb();
	//移動の入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//空中移動
		rb->AddVec(GetForwardVec(camera) * InputValueSpeed(input));
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
	Vector2 dir = m_player.lock()->GetStickVec();
	m_player.lock()->GetModel()->SetDir(dir);
}


float PlayerStateFall::InputValueSpeed(const Input& input)
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