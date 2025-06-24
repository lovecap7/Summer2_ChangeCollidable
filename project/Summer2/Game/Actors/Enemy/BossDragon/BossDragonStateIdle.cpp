#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateChase.h"
#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateSweepAttack.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//プレイヤー戦闘状態になる距離
	constexpr float kBackDistance = 300.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Flying_Idle";//待機
}

BossDragonStateIdle::BossDragonStateIdle(std::shared_ptr<BossDragon> owner):
	BossDragonStateBase(owner)
{
	//待機状態
	m_owner->GetModel()->SetAnim(kAnim, true);
	m_owner->GetCollidable()->SetState(State::None);
}

BossDragonStateIdle::~BossDragonStateIdle()
{
}

void BossDragonStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}

	//減速
	SpeedDown();
	//プレイヤーを発見したとき
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//プレイヤーを向く
		LookPlayer();
		//攻撃のクールタイムが0なら
		if (m_owner->GetAttackCoolTime() <= 0)
		{
			//どの攻撃をするか考える
			ThinkAttack();
			return;
		}
	}
}

void BossDragonStateIdle::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void BossDragonStateIdle::ThinkAttack()
{
	//ランダムに決定
	auto rand = GetRand(2);

	switch (rand)
	{
	case 0:
		//プレイヤーをに近づく
		ChangeState(std::make_shared<BossDragonStateChase>(m_owner));
		break;
	case 1:
		//薙ぎ払い
		ChangeState(std::make_shared<BossDragonStateSweepAttack>(m_owner));
		break;
	case 2:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner));
		break;
	default:
		//ブレス
		ChangeState(std::make_shared<BossDragonStateBreathAttack>(m_owner));
		break;
	}
	return;
}
