#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"
#include "../../ActorManager.h"

namespace
{
	//プレイヤーとの距離が遠いときに当たり判定を停止する距離
	const float kStopCollisionDistance = 1500.0f;
}

EnemyBase::EnemyBase(Shape shape, EnemyGrade grade) :
	CharacterBase(shape),
	m_attackCoolTime(0),
	m_enemyGrade(grade)
{
}

void EnemyBase::UpdateAttackCoolTime()
{
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}

bool EnemyBase::IsStopActive(const std::weak_ptr<ActorManager> actorManager)
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
