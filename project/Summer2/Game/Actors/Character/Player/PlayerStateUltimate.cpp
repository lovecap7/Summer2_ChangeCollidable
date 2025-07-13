#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../Attack/AreaOfEffectAttack.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//攻撃判定をリセットする頻度
	constexpr int kUltResetFrame = 10;
	//アニメーションの速度の変化量
	constexpr float kAddAnimSpeed = 0.2f;
	//アニメーションの最高速度
	constexpr float kMaxAnimSpeed = 2.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_animCountFrame(0)
{
	//チャージフレームが持続フレームより大きいかを比較
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kULTName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//必殺技
	coll->SetCollState(CollisionState::Normal);
	auto model = coll->GetModel();
	m_animSpeed = m_attackData.animSpeed;
	model->SetAnim(m_attackData.anim.c_str(), true, m_animSpeed);
	model->SetFixedLoopFrame(m_attackData.keepFrame);//指定ループ
	//向きの更新
	Vector2 dir = coll->GetStickVec();
	model->SetDir(dir);
	//ゲージを0に
	coll->GetUltGage().lock()->ResetUltGage();
	coll->GetUltGage().lock()->SetPendingUltGage(0);
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(true);
	//攻撃発生
	CreateAttack(actorManager);
}


PlayerStateUltimate::~PlayerStateUltimate()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//無敵
	coll->GetHitPoints().lock()->SetIsNoDamege(false);
}

void PlayerStateUltimate::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
	//必殺エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("UltEff", m_owner);
}

void PlayerStateUltimate::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//回転
	model->SetRot(VGet(0.0f, m_animCountFrame, 0.0f));
	//勝利したとき
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//攻撃判定をリセット
	if (m_animCountFrame % kUltResetFrame == 0)
	{
		if (!m_attack.expired())m_attack.lock()->ResetHitId();
	}
	//アニメーションが一周するたびに再生速度を上げる
	if (model->IsFinishAnim())
	{
		//アニメーションの速度を速くしていく
		if (m_animSpeed < kMaxAnimSpeed)
		{
			m_animSpeed += kAddAnimSpeed;
			model->SetAnimSpeed(m_animSpeed);
		}
	}
	//攻撃の位置更新
	if (!m_attack.expired())UpdateAttackPos();
	auto& input = Input::GetInstance();
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * m_attackData.moveSpeed);
	}
	else
	{
		//少しずつ減速する
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateUltimate::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<AreaOfEffectAttack>(actorManager.lock()->CreateAttack(AttackType::AreaOfEffect, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(m_attackData.radius);
	attack->AttackSetting(m_attackData.damege, m_attackData.keepFrame, 
		m_attackData.knockBackPower, m_attackData.attackWeight);
}

void PlayerStateUltimate::UpdateAttackPos()
{
	auto coll = m_owner.lock();
	m_attack.lock()->SetPos(coll->GetPos());
}
