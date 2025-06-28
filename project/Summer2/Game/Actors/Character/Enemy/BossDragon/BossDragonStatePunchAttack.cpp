#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/AreaOfEffectAttack.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//右手のインデックス
	constexpr int kRightHandIndex = 36;
	//パンチの当たり判定の大きさ(攻撃の大きさ)
	constexpr float kAttackRadius = 100.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 3;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 26;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 20.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Punch";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.4f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 40;
}


BossDragonStatePunchAttack::BossDragonStatePunchAttack(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

BossDragonStatePunchAttack::~BossDragonStatePunchAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
}

void BossDragonStatePunchAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStatePunchAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
	//攻撃の位置更新
	if (!m_attack.expired()) UpdateAttackPos();

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStatePunchAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(kAttackRadius);
	attack->AttackSetting(kAttackDamage, kAttackKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
}

void BossDragonStatePunchAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	VECTOR rightHand = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//右手
	//座標をセット
	m_attack.lock()->SetPos(rightHand);
}