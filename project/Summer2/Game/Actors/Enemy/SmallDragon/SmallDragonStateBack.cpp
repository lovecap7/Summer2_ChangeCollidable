#include "SmallDragonStateBack.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../Actor.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
namespace
{
	//プレイヤー戦闘状態になる距離
	constexpr float kBackDistance = 300.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Fast_Flying";
	//プレイヤーから離れる速度
	constexpr float kBackSpeed = 1.0f;
}

SmallDragonStateBack::SmallDragonStateBack(std::shared_ptr<SmallDragon> owner):
	SmallDragonStateBase(owner)
{
	m_owner->GetCollidable()->SetState(State::None);
	//アニメーション
	m_owner->GetModel()->SetAnim(kAnim, true);
}

SmallDragonStateBack::~SmallDragonStateBack()
{
}

void SmallDragonStateBack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateBack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}

	//プレイヤーを発見したとき
	if (m_owner->GetSearchTrigger()->IsTargetHit())
	{
		//プレイヤーを向く
		LookPlayer();
		//距離をチェック
		float dist = m_owner->GetPlayerVec().Magnitude();
		//近いなら距離なら
		if (dist <= kBackDistance)
		{
			//プレイヤーから離れる
			Vector3 chaseVec = m_owner->GetPlayerNomVecXZ();
			m_owner->GetCollidable()->GetRb()->SetMoveVec(chaseVec * -kBackSpeed);
		}
		else
		{
			//待機状態にする
			ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		}
	}
	//見失ったら
	else
	{
		//待機状態にする
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
	}
}
