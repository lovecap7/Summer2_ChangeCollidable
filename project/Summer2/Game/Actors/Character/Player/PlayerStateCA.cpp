#include "PlayerStateCA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../../ActorManager.h"
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
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../Attack/Strike.h"

namespace
{
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 60;
	constexpr int kToeIndex = 64;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//エフェクトの削除を遅らせる
	constexpr int kDeleteEffectDelayFrame = 20;
}

PlayerStateCA::PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager, int chargeFrame)	:
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
	//チャージフレームが持続フレームより大きいかを比較
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
	if (chargeFrame > m_attackData.keepFrame)
	{
		//2段階目
		m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
		if (chargeFrame > m_attackData.keepFrame)
		{
			//3段階目
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA3Name);
		}
	}
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃
	auto model = coll->GetModel();
	model->SetAnim(m_attackData.anim.c_str(), true, m_attackData.animSpeed);
	model->SetFixedLoopFrame(m_attackData.keepFrame);//指定ループ
	//加算ゲージの予約
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);

	//キックエフェクト
	m_eff = EffekseerManager::GetInstance().CreateTrackActorEffect("CATornade", m_owner.lock());
}

PlayerStateCA::~PlayerStateCA()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
	//エフェクトを数フレーム後削除
	m_eff.lock()->SpecificFrame(kDeleteEffectDelayFrame);
}
void PlayerStateCA::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
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
	++m_attackCountFrame;
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//攻撃発生
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
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
	auto& input = Input::GetInstance();
	//入力があるなら
	if (input.GetStickInfo().IsLeftStickInput())
	{
		//移動
		coll->GetRb()->SetMoveVec(GetForwardVec(camera) * m_attackData.moveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateCA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}

void PlayerStateCA::UpdateAttackPos()
{
	//左足の状態を更新したら攻撃も更新される
	auto model = m_owner.lock()->GetModel();
	//左足
	//付け根と足先
	VECTOR root = MV1GetFramePosition(model->GetModelHandle(), kRootIndex);//付け根
	VECTOR toe = MV1GetFramePosition(model->GetModelHandle(), kToeIndex);//足先
	//座標をセット
	m_attack.lock()->SetStartPos(root);
	m_attack.lock()->SetEndPos(toe);
}
