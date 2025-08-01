#include "BossMuscleStateBeam.h"
#include "BossMuscleStateIdle.h"
#include "BossMuscleStateAngry.h"
#include "BossMuscleStateDeath.h"
#include "BossMuscleStateHit.h"
#include "BossMuscle.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../../General/Effect/MyEffect.h"
#include "../../../Attack/ULT.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"

namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//右腕と右手のインデックス
	constexpr int kRightArmIndex = 41;
	constexpr int kRightHandIndex = 43;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 120;//2秒くらいの感覚で攻撃
	//怒り状態の時の速度2倍
	constexpr float kAngryMoveSpeedRate = 2.0f;
}

BossMuscleStateBeam::BossMuscleStateBeam(std::weak_ptr<Actor> owner, bool isAngry, const std::weak_ptr<ActorManager> actorManager):
	BossMuscleStateBase(owner, isAngry),
	m_attackCountFrame(0)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kBeamName);
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

BossMuscleStateBeam::~BossMuscleStateBeam()
{
}

void BossMuscleStateBeam::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossMuscleStateBeam::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossMuscle>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<BossMuscleStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossMuscleStateHit>(m_owner, m_isAngry));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossMuscleStateIdle>(m_owner,m_isAngry));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossMuscleStateBeam::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//作成と参照
	auto attack = std::dynamic_pointer_cast<ULT>(actorManager.lock()->CreateAttack(AttackType::ULT, m_owner).lock());
	//攻撃を作成
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);

	//攻撃の位置
	Vector3 startPos = owner->GetPos();
	Vector3 endPos = owner->GetPos() + (owner->GetModel()->GetDir() * 10.0f);
	//座標をセット
	attack->SetStartPos(startPos);
	attack->SetEndPos(endPos);
	//エフェクトの位置
	//必殺エフェクト
	auto eff = EffekseerManager::GetInstance().CreateEffect("UltLaserEff", m_owner.lock()->GetPos());
	eff.lock()->SetPos(startPos);
	eff.lock()->LookAt(owner->GetModel()->GetDir());
}
