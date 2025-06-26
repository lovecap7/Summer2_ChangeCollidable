#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateRolling.h"
#include "Player.h"
#include "UltGage.h"
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
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Player|Idle";//待機
}

PlayerStateIdle::PlayerStateIdle(std::weak_ptr<Player> player):
	PlayerStateBase(player)
{
	//待機状態
	auto coll = m_player.lock();
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = m_player.lock();
	Vector3 vec = coll->GetRb()->GetVec();
	//落下しているかチェック
	if (vec.y <= Gravity::kChangeStateFall)
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
	//ゲージがあるとき使える
	if (input.IsTrigger("RB") && coll->GetUltGage()->IsMaxUlt())
	{
		//必殺技
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player));
		return;
	}

	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("B") && coll->IsFloor())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	
	//入力があるなら移動
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		ChangeState(std::make_shared<PlayerStateWalk>(m_player));
		return;
	}
	//だんだん減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
