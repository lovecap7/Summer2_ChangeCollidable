#include "BomberStateChase.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
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
	//アニメーション
	const char* kAnim = "CharacterArmature|Run";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 5.0f;
}

BomberStateChase::BomberStateChase(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//アニメーション
	m_owner->GetModel()->SetAnim(kAnim, true);
}

BomberStateChase::~BomberStateChase()
{
}

void BomberStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateChase::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
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
			//待機状態にする
			ChangeState(std::make_shared<BomberStateIdle>(m_owner));
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
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
	}
}
