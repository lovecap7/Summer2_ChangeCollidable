#include "EnemyBase.h"
#include "../CharacterStateBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/HitPoints.h"
#include "../../ActorManager.h"

namespace
{
	//プレイヤーとの距離が遠いときに当たり判定を停止する距離
	const float kStopCollisionDistance = 5000.0f;
	//最初のクールタイム
	constexpr int kAttackCoolTime = 100;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

EnemyBase::EnemyBase(Shape shape, EnemyGrade grade) :
	CharacterBase(shape),
	m_attackCoolTime(kAttackCoolTime),
	m_enemyGrade(grade),
	m_isActive(true),
	m_searchDistance(0.0f),
	m_searchAngle(0.0f)
{
}

void EnemyBase::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//プレイヤーから遠いなら処理をしない
	if (IsStopActiveCollision(actorManager))return;
	//アクティブ状態じゃないなら
	if (!m_isActive)
	{
		//減速
		m_rb->SpeedDown(kMoveDeceRate);
		//アニメーションの更新
		m_model->Update();
		return;
	}
	//攻撃のクールタイムを減らす
	UpdateAttackCoolTime();
	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		TargetSearch(m_searchDistance, m_searchAngle, target.lock()->GetPos());
	}
	//状態に合わせた更新
	m_state->Update(camera, actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//体力クラスのフラグリセット
	m_hitPoints->ResetHitFlags();
}

void EnemyBase::UpdateAttackCoolTime()
{
	//攻撃できないなら
	if (!m_canAttack)return;
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}

void EnemyBase::SetActive(bool isActive)
{
	m_isActive = isActive;
	if (!m_isActive)
	{
		//無敵
		m_hitPoints->SetIsNoDamege(true);
	}
	else
	{
		//無敵解除
		m_hitPoints->SetIsNoDamege(false);
	}
}

bool EnemyBase::IsStopActiveCollision(const std::weak_ptr<ActorManager> actorManager)
{
	//停止したか
	bool isStop = false;
	//プレイヤーとの距離
	auto player = actorManager.lock()->GetPlayer();
	if (player.expired())
	{
		return true;
	}
	auto dis = (player.lock()->GetPos() - m_rb->m_pos).Magnitude();
	//プレイヤーとの距離が遠いときは当たり判定を停止する
	if (dis > kStopCollisionDistance)
	{
		m_isThrough = true;				//当たり判定を停止
		m_rb->m_isGravity = false;		//重力を無効化
		m_rb->SetVec(Vector3::Zero());	//動きを止める
		isStop = true;					//停止した
	}
	else
	{
		m_isThrough = false;			//当たり判定を有効化
		m_rb->m_isGravity = true;		//重力を有効化
	}
	return isStop;
}
