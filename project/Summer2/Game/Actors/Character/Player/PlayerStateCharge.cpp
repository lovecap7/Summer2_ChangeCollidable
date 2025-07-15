#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../Enemy/EnemyBase.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//アニメーション
	const char* kAnim = "Player|Charge";//チャージ
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左足のインデックス
	constexpr int kLeftLegIndex = 64;
	//硬直
	constexpr int kStopFrame = 30;
	//チャージフレーム
	constexpr int kChargeLevel2Frame = 60;
	constexpr int kChargeLevel3Frame = 120;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//回避状態
	coll->SetCollState(CollisionState::Normal);
	//チャージ
	coll->GetModel()->SetAnim(kAnim, true);
	//左足の状態を更新したら攻撃も更新される
	auto model = m_owner.lock()->GetModel();
	//左足
	VECTOR leg = MV1GetFramePosition(model->GetModelHandle(), kLeftLegIndex);//付け根
	//チャージエフェクト
	m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel1Eff"), m_owner);
	//1段目
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA1Name);
}

PlayerStateCharge::~PlayerStateCharge()
{
	//エフェクトを数フレーム後削除
	m_levelEff.lock()->Delete();
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
	//ボスが完全に消滅したとき
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//死亡したかつボスが倒せてない場合
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
	if (input.IsPress("Y") || m_chargeFrame < kStopFrame)
	{
		//タメ攻撃チャージ
		++m_chargeFrame;
		//2段階目
		if (m_chargeFrame == kChargeLevel2Frame)
		{
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA2Name);
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel2Eff"), m_owner);
		}
		//3段階目
		else if(m_chargeFrame == kChargeLevel3Frame)
		{
			//1段目
			m_attackData = actorManager.lock()->GetAttackData(kPlayerName, kCA3Name);
			m_levelEff.lock()->Delete();
			m_levelEff = EffekseerManager::GetInstance().CreateTrackActorEffect(std::string("ChargeLevel3Eff"), m_owner);
		}
	}
	//ボタンを離す
	else
	{
		ChangeState(std::make_shared<PlayerStateCA>(m_owner, actorManager, m_attackData));
		return;
	}
}
