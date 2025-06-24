#include "BomberStateIdle.h"
#include "BomberStateHit.h"
#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateAttack.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//プレイヤー戦闘状態になる距離
	constexpr float kBattleDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Idle";//待機
}

BomberStateIdle::BomberStateIdle(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	//待機状態
	m_owner->GetModel()->SetAnim(kAnim, true);
	m_owner->GetCollidable()->SetState(State::None);
}

BomberStateIdle::~BomberStateIdle()
{
}

void BomberStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateIdle::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}

	//減速
	SpeedDown();
	//プレイヤーを発見したとき
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//プレイヤーを向く
		LookPlayer();
		//距離をチェック
		float dist = m_owner->GetPlayerVec().Magnitude();
		//戦闘状態距離なら
		if (dist <= kBattleDistance)
		{
			//攻撃のクールタイムが0なら
			if (m_owner->GetAttackCoolTime() <= 0)
			{
				//攻撃状態にする
				ChangeState(std::make_shared<BomberStateAttack>(m_owner));
			}
		}
		//射程範囲外なので
		else
		{
			//プレイヤーをに近づく
			ChangeState(std::make_shared<BomberStateChase>(m_owner));
		}
	}
}

void BomberStateIdle::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
