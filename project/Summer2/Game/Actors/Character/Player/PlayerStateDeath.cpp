#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../../General/game.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Dead";
	//減速率
	constexpr float kMoveDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Actor> player) :
	PlayerStateBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//待機状態
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Dead);
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		coll->Delete();//削除
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

