#include "PlayerStateRolling.h"
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
	//回避速度
	constexpr float kRollingMoveSpeed = 10.0f;
	//アニメーション
	const char* kAnim = "Player|Rolling";
	//回避モーションの速度
	constexpr float kAnimSpeed = 1.2f;
	//重力を重めにする(坂道対策)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}

PlayerStateRolling::PlayerStateRolling(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	//回避状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	coll->SetCollState(CollisionState::Normal);
	//向きの更新
	coll->GetModel()->SetDir(coll->GetStickVec());
}

PlayerStateRolling::~PlayerStateRolling()
{
	
}
void PlayerStateRolling::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateRolling::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//モデルのアニメーションが終わったら
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	auto rb = coll->GetRb();
	//重力
	rb->AddVec(kBigGravity);
	//向いてる方向に移動
	rb->SetMoveVec(coll->GetModel()->GetDir() * kRollingMoveSpeed);
}
