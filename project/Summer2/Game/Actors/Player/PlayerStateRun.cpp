#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
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
	//移動速度
	constexpr float kMoveSpeed = 15.0f;//地上の移動速度
	//アニメーション
	const char* kAnim = "Player|Dash";
	//重力を重めにする(坂道対策)
	const Vector3 kBigGravity = { 0.0f,-5.0f,0.0f };
}


PlayerStateRun::PlayerStateRun(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//走り状態
	m_player->GetModel()->SetAnim(kAnim, true);
	m_player->GetCollidable()->SetState(State::None);
}


PlayerStateRun::~PlayerStateRun()
{
}

void PlayerStateRun::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateRun::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
	rb->SetMoveVec(GetForwardVec(input, camera) * kMoveSpeed);
	//向きの更新
	Vector2 dir = m_player->GetStickVec();
	m_player->GetModel()->SetDir(dir);
}
