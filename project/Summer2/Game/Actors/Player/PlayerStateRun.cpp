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
	//移動速度
	constexpr float kMoveSpeed = 15.0f;//地上の移動速度
	//アニメーション
	const char* kAnim = "Player|Dash";
	//重力を重めにする(坂道対策)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}


PlayerStateRun::PlayerStateRun(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//走り状態
	m_player.lock()->GetModel()->SetAnim(kAnim, true);
	m_player.lock()->SetCollState(CollisionState::Normal);
}


PlayerStateRun::~PlayerStateRun()
{
}

void PlayerStateRun::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateRun::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	auto collidable = m_player.lock();
	//落下しているかチェック
	if (collidable->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_player));
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("A"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("B") && collidable->IsFloor())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	
	//入力がないなら待機
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = collidable->GetRb();
	//重力
	rb->AddVec(kBigGravity);
	//移動
	rb->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
	//向きの更新
	Vector2 dir = collidable->GetStickVec();
	collidable->GetModel()->SetDir(dir);
}
