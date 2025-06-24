#include "PlayerStateIdle.h"
#include "PlayerStateWalk.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
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
#include "../../Attack/HurtPoint.h"


namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Player|Idle";//待機
}

PlayerStateIdle::PlayerStateIdle(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//待機状態
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateIdle::~PlayerStateIdle()
{
}
void PlayerStateIdle::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
	auto collidable = m_player->GetCollidable();
	Vector3 vec = collidable->GetRb()->GetVec();
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
	if (input.IsTrigger("RB") && m_player->GetUltGage()->IsMaxUlt())
	{
		//必殺技
		ChangeState(std::make_shared<PlayerStateUltimate>(m_player, actorManager));
		return;
	}

	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("B") && m_player->IsGround())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_player));
		return;
	}
	//弱攻撃ボタンを押したら
	if (input.IsTrigger("X"))
	{
		//弱攻撃
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_player));
		return;
	}
	//チャージボタンを押したら
	if (input.IsTrigger("Y"))
	{
		//チャージ
		ChangeState(std::make_shared<PlayerStateCharge>(m_player));
		return;
	}
	//入力があるなら移動
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		ChangeState(std::make_shared<PlayerStateWalk>(m_player));
		return;
	}
	//少しずつ減速する
	SpeedDown();
}

void PlayerStateIdle::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
