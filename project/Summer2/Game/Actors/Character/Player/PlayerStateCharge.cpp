#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Charge";//チャージ
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//回避状態
	coll->SetCollState(CollisionState::Normal);
	//チャージ
	coll->GetModel()->SetAnim(kAnim, true);
}

PlayerStateCharge::~PlayerStateCharge()
{
}
void PlayerStateCharge::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCharge::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//勝利したとき
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//死亡したかつボスが倒せてない場合
	if (coll->GetHitPoints().lock()->IsDead() && !actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_owner));
		return;
	}
	//攻撃を受けたなら
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_owner));
		return;
	}
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
		return;
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
	//向きの更新
	Vector2 dir = coll->GetStickVec();
	coll->GetModel()->SetDir(dir);
	//溜めてる時
	if (input.IsPress("Y"))
	{
		//タメ攻撃チャージ
		++m_chargeFrame;
	}
	//ボタンを離す
	else
	{
		ChangeState(std::make_shared<PlayerStateCA>(m_owner, actorManager, m_chargeFrame));
		return;
	}
}
