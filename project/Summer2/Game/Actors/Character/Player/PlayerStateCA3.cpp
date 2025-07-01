#include "PlayerStateCA3.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../Attack/Strike.h"

namespace
{
	//チャージ攻撃の段階別ダメージ
	constexpr int kCA3AnimDamage = 160.0f;
	//チャージ攻撃の持続
	constexpr float kCA3KeepFrame = 60.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//攻撃の発生
	constexpr int kStartFrame = 1;
	//アニメーション
	const char* kAnim = "Player|CA";//チャージ攻撃
	//チャージ攻撃の段階別アニメーションの速度
	constexpr float kCA3AnimSpeed = 3.0f;
	//左足の半径
	constexpr float kAttackRadius = 20.0f;
	//移動速度
	constexpr float kAttackMoveSpeed = 10.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 3;
}

PlayerStateCA3::PlayerStateCA3(std::weak_ptr<Actor> player) :
	PlayerStateChargeAttackBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Move);
	//チャージ攻撃1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, kCA3AnimSpeed);
	model->SetFixedLoopFrame(kCA3KeepFrame);//指定ループ
}


PlayerStateCA3::~PlayerStateCA3()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
}
void PlayerStateCA3::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA3::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//死亡したなら
	if (coll->GetHitPoints().lock()->IsDead())
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
	auto& input = Input::GetInstance();
	++m_attackCountFrame;
	if (m_attackCountFrame == kStartFrame)
	{
		//攻撃発生
		CreateAttack(kAttackRadius, kCA3AnimDamage, kCA3KeepFrame, kKnockBackPower, Battle::AttackWeight::Middle, actorManager);
	}
	auto model = coll->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (model->IsFinishAnim())
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//攻撃の位置更新
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * kAttackMoveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
