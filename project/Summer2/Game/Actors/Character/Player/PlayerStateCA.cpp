#include "PlayerStateCA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "../Enemy/EnemyBase.h"
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
#include "../../Attack/Slash.h"

namespace
{
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さ
	constexpr float kSwordHeight = 150.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//エフェクトの削除を遅らせる
	constexpr int kDeleteEffectDelayFrame = 20;
}

PlayerStateCA::PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager, AttackData data)	:
	PlayerStateBase(player),
	m_attackCountFrame(0),
	m_attackData(data)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃
	auto model = coll->GetModel();
	model->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//加算ゲージの予約
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}

PlayerStateCA::~PlayerStateCA()
{
	//攻撃判定の削除
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	if (!m_attack.expired())m_attack.lock()->Delete();
}
void PlayerStateCA::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//ボスが完全に消滅したとき
	if (actorManager.lock()->GetBoss().expired())
	{
		ChangeState(std::make_shared<PlayerStateWin>(m_owner));
		return;
	}
	//ボスの体力がなくなった場合またはアニメーションが終了したら
	if (actorManager.lock()->GetBoss().lock()->GetHitPoints().lock()->IsDead() ||
		model->IsFinishAnim())
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
	++m_attackCountFrame;
	//発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//攻撃発生
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame,
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//持続を攻撃回数で割った値に達したとき
	if (m_attackCountFrame % (m_attackData.keepFrame / m_attackData.attackNum) == 0)
	{
		m_attack.lock()->ResetHitId();
	}
	//攻撃の位置更新
	if (!m_attack.expired())
	{
		UpdateAttackPos();
	}
	//入力があるとき
	auto& input = Input::GetInstance();
	if (input.GetStickInfo().IsLeftStickInput() &&
		m_attackCountFrame <= m_attackData.moveFrame)
	{
		//移動フレーム中は前に進む
		AttackMove(m_attackData.moveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PlayerStateCA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	auto owner = m_owner.lock();
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
}

void PlayerStateCA::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//武器
	//右手の薬指と人差し指の座標から武器の座標を出す
	VECTOR ringFinger = MV1GetFramePosition(model->GetModelHandle(), kRightRingFingerIndex);//薬指
	VECTOR indexFinger = MV1GetFramePosition(model->GetModelHandle(), kRightIndexFingerIndex);//人差し指
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(indexFinger, ringFinger));
	swordDir = VScale(swordDir, kSwordHeight);//武器の長さ
	swordDir = VAdd(ringFinger, swordDir);//持ち手の座標に加算して剣先の座標を出す
	if (!m_attack.expired())
	{
		//座標をセット
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordDir);
	}
}

void PlayerStateCA::AttackMove(float speed)
{
	//ターゲットを索敵してるなら(ターゲットのほうを向く)
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto targetData = coll->GetTargetData();
	//向き
	Vector3 dir = coll->GetStickVec().XZ();
	//ターゲットが発見できた時
	if (targetData.isHitTarget)
	{
		dir = targetData.targetDirXZ;
	}
	//向きの更新
	coll->GetModel()->SetDir(dir.XZ());
	//向いてる方向に移動
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * speed);
}
