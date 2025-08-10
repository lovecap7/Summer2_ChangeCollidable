#include "BossKingStateRapidFire.h"
#include "BossKingStateIdle.h"
#include "BossKingStateHit.h"
#include "BossKingStateDeath.h"
#include "BossKingStateChase.h"
#include "BossKingStateChange.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../Player/Player.h"
#include "../../../ActorManager.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
#include "../../../Attack/Bullet.h"
#include "../../../../../General/Effect/EffekseerManager.h"

namespace
{
	//アニメーション
	const char* kAnim = "Boss3|Run";
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
	//右手のハンドル
	constexpr int kRightHandIndex = 63;
	//右肩のハンドル
	constexpr int kRightShoulderIndex = 44;
	//弾の高さ
	constexpr float kBulletPosY = 150.0f;
}


BossKingStateRapidFire::BossKingStateRapidFire(std::weak_ptr<Actor> owner, const std::weak_ptr<ActorManager> actorManager):
	BossKingStateBase(owner, true),
	m_attackCountFrame(0),
	m_shotNum(1)
{
	m_attackData = actorManager.lock()->GetAttackData(kOwnerName, kRapidFireName);
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(m_attackData.anim.c_str(), false, m_attackData.animSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

BossKingStateRapidFire::~BossKingStateRapidFire()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void BossKingStateRapidFire::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossKingStateRapidFire::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		//第二形態なら
		if (m_isTransformSecond)
		{
			//死亡
			ChangeState(std::make_shared<BossKingStateDeath>(m_owner));
			return;
		}
		else
		{
			//変身
			ChangeState(std::make_shared<BossKingStateChange>(m_owner));
			return;
		}
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossKingStateHit>(m_owner, m_isTransformSecond));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == m_attackData.startFrame + m_shotNum)
	{
		//打った数をカウント
		++m_shotNum;
		m_shotNum = MathSub::ClampInt(m_shotNum, 0, m_attackData.attackNum);
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner, m_isTransformSecond));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossKingStateRapidFire::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	auto attack = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//弾の設定
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	auto data = m_attackData;
	//大きさ
	attack->SetRadius(data.radius);
	//ダメージ、持続フレーム、ノックバックの大きさ、攻撃の重さ、ヒットストップの長さ、カメラの揺れ
	attack->AttackSetting(data.damege, data.keepFrame,
		data.knockBackPower, data.attackWeight, data.hitStopFrame, data.shakePower);
	//右手と右肩
	auto model = coll->GetModel();
	auto rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);
	auto rightShoulderPos = MV1GetFramePosition(model->GetModelHandle(), kRightShoulderIndex);
	//生成位置
	Vector3 bulletPos = rightHandPos;
	//高さ
	bulletPos.y = kBulletPosY;
	attack->SetPos(bulletPos);
	//弾の進行方向とスピード
	auto dir = Vector3(rightHandPos) - Vector3(rightShoulderPos);
	dir.y = 0.0f;
	if (dir.SqMagnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	auto vec = dir * data.moveSpeed;
	attack->SetVec(vec);
	//エフェクト
	EffekseerManager::GetInstance().CreateTrackActorEffect("BreathEff", attack);
}


