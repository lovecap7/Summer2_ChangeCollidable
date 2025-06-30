#include "PlayerStateRun.h"
#include "PlayerStateIdle.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "PlayerStateAttackN1.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//移動速度
	constexpr float kMoveSpeed = 15.0f;//地上の移動速度
	//アニメーション
	const char* kAnim = "Player|Dash";
}


PlayerStateRun::PlayerStateRun(std::weak_ptr<Actor> player):
	PlayerStateBase(player)
{
	//走り状態
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Move);
	//走りキープ
	coll->SetIsRunKeep(true);
}


PlayerStateRun::~PlayerStateRun()
{
}

void PlayerStateRun::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateRun::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//死亡したなら
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//落下しているかチェック
	if (coll->GetRb()->GetVec().y <= Gravity::kChangeStateFall)
	{
		//落下
		ChangeState(std::make_shared<PlayerStateFall>(m_owner));
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("A"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	
	//ジャンプボタンを押してるならジャンプ
	if (input.IsTrigger("B") && coll->IsFloor())
	{
		//ジャンプ
		ChangeState(std::make_shared<PlayerStateJump>(m_owner));
		return;
	}
	if (input.IsTrigger("X"))
	{
		//攻撃
		ChangeState(std::make_shared<PlayerStateAttackN1>(m_owner));
		return;
	}
	if (input.IsTrigger("Y"))
	{
		//チャージ
		ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
		return;
	}
	//入力がないなら待機
	if (!input.GetStickInfo().IsLeftStickInput())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	auto rb = coll->GetRb();
	//移動
	rb->SetMoveVec(GetForwardVec(camera) * kMoveSpeed);
	//向きの更新
	coll->GetModel()->SetDir(coll->GetStickVec());
}
