#include "BossDragonStateChase.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
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
	constexpr float kBattleDistance = 120.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 8.0f;
}

BossDragonStateChase::BossDragonStateChase(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//アニメーション
	m_owner->GetModel()->SetAnim(kAnim, true);
}

BossDragonStateChase::~BossDragonStateChase()
{
}

void BossDragonStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateChase::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}

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
			//パンチ状態にする
			ChangeState(std::make_shared<BossDragonStatePunchAttack>(m_owner));
			return;
		}
		//射程範囲外なので
		else
		{
			//プレイヤーをに近づく
			Vector3 chaseVec = m_owner->GetPlayerNomVecXZ();
			m_owner->GetCollidable()->GetRb()->SetMoveVec(chaseVec * kChaseSpeed);
		}
	}
	//見失ったとき
	else
	{
		//待機状態にする
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
}
