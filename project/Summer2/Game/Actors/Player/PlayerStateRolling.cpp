#include "PlayerStateRolling.h"
#include "PlayerStateIdle.h"
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
	//回避速度
	constexpr float kRollingMoveSpeed = 10.0f;
	//アニメーション
	const char* kAnim = "Player|Rolling";
	//回避モーションの速度
	constexpr float kAnimSpeed = 1.2f;
	//重力を重めにする(坂道対策)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}

PlayerStateRolling::PlayerStateRolling(std::shared_ptr<Player> player) :
	PlayerStateBase(player)
{
	//回避状態
	m_player->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	m_player->GetCollidable()->SetState(State::None);
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
	//無敵に
	m_player->GetHurtPoint()->SetIsNoDamege(true);
}

PlayerStateRolling::~PlayerStateRolling()
{
	//無敵解除
	m_player->GetHurtPoint()->SetIsNoDamege(false);
}
void PlayerStateRolling::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateRolling::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	auto collidable = m_player->GetCollidable();
	//モデルのアニメーションが終わったら
	if (m_player->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	auto rb = collidable->GetRb();
	//重力
	rb->AddVec(kBigGravity);
	//向いてる方向に移動
	rb->SetMoveVec(m_player->GetModel()->GetDir() * kRollingMoveSpeed);
}
