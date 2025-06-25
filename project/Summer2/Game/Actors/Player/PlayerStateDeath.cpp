#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Dead";
	//減速率
	constexpr float kDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::weak_ptr<Player> player) :
	PlayerStateBase(player)
{
	auto coll = m_player.lock();
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
void PlayerStateDeath::Update(const std::weak_ptr<Camera> camera)
{
	//アニメーション終了後
	if (m_player.lock()->GetModel()->IsFinishAnim())
	{
		m_player.lock()->Delete();//削除
	}
	//減速
	SpeedDown();
}

void PlayerStateDeath::SpeedDown()
{
	auto collidable = m_player.lock();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}

