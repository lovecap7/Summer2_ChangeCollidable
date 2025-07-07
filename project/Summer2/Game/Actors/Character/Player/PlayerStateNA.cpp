#include "PlayerStateNA.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCharge.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateWin.h"
#include "Player.h"
#include "UltGage.h"
#include "../../ActorManager.h"
#include "../../Attack/Slash.h"
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
#include "../../../../General/Effect/MyEffect.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//武器の座標と当たり判定の情報
	//右手の薬指のインデックス
	constexpr int kRightRingFingerIndex = 55;
	constexpr int kRightIndexFingerIndex = 43;
	//武器の長さ
	constexpr float kSwordHeight = 150.0f;
	//キャンセル
	constexpr int kAttackCancelFrame = 20;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//連続攻撃の最大数
	constexpr int kMaxComboNum = 3;
}

PlayerStateNA::PlayerStateNA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager) :
	PlayerStateBase(player),
	m_comboNum(1),
	m_attackCountFrame(0)
{
	//攻撃データの初期化
	InitAttackData(actorManager);
}

PlayerStateNA::~PlayerStateNA()
{
}
void PlayerStateNA::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateNA::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	auto& input = Input::GetInstance();
	//カウント
	++m_attackCountFrame;
	auto model = coll->GetModel();
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame)
	{
		//攻撃作成
		CreateAttack(m_attackData.radius, m_attackData.damege, m_attackData.keepFrame, 
			m_attackData.knockBackPower, m_attackData.attackWeight, actorManager);
	}
	//モデルのアニメーションが終わったら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーションのラスト数フレーム以内で入力があるなら2段回目の攻撃
	if (model->GetTotalAnimFrame() - kAttackCancelFrame <= model->GetNowAnimFrame())
	{
		//回避ボタンを押したら
		if (input.IsTrigger("A"))
		{
			//回避
			ChangeState(std::make_shared<PlayerStateRolling>(m_owner));
			return;
		}
		//まだ攻撃できるなら
		if (input.IsTrigger("X") && m_comboNum < kMaxComboNum)
		{
			//次の攻撃
			++m_comboNum;
			//初期化
			InitAttackData(actorManager);
			m_attackCountFrame = 0;
			return;
		}
		//チャージボタンを押したら
		if (input.IsTrigger("Y"))
		{
			//チャージ
			ChangeState(std::make_shared<PlayerStateCharge>(m_owner));
			return;
		}
	}

	//攻撃の位置更新
	UpdateAttackPos();
	//移動フレーム中は前に進む
	if (m_attackCountFrame <= m_attackData.moveFrame)
	{
		AttackMove(m_attackData.moveSpeed);
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}
void PlayerStateNA::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Slash>(actorManager.lock()->CreateAttack(AttackType::Slash, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(radius);
	attack->AttackSetting(damage, keepFrame, knockBackPower, aw);
}
void PlayerStateNA::UpdateAttackPos()
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
	//エフェクトの位置更新
	if (!m_eff.expired())
	{
		m_eff.lock()->SetPos(swordDir);
	}
	if (!m_attack.expired())
	{
		//座標をセット
		m_attack.lock()->SetStartPos(ringFinger);
		m_attack.lock()->SetEndPos(swordDir);
	}
}
void PlayerStateNA::AttackMove(float speed)
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

void PlayerStateNA::InitAttackData(const std::weak_ptr<ActorManager> actorManager)
{
	//斬撃エフェクト
	m_eff = EffekseerManager::GetInstance().CreateEffect("SwordTest", m_owner.lock()->GetPos());
	std::string attackName;
	switch (m_comboNum)
	{
	case 1:
		attackName = kNA1Name;
		break;
	case 2:
		attackName = kNA2Name;
		break;
	case kMaxComboNum:
		attackName = kNA3Name;
		break;
	default:
		break;
	}
	m_attackData = actorManager.lock()->GetAttackData(kPlayerName, attackName);
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//通常攻撃1
	coll->SetCollState(CollisionState::Move);
	//攻撃1
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//加算ゲージの予約
	coll->GetUltGage().lock()->SetPendingUltGage(m_attackData.addUltGage);
}