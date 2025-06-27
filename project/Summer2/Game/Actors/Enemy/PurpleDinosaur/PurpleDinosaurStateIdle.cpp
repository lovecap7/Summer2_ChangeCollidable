#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../ActorManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//プレイヤーを発見する距離
	constexpr float kSearchDistance = 500.0f;
	//プレイヤーに攻撃する距離
	constexpr float kAttackDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーションの名前
	const char* kAnim = "CharacterArmature|Idle";//待機
}

PurpleDinosaurStateIdle::PurpleDinosaurStateIdle(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner)
{
	//待機状態
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, true);
	coll->SetCollState(CollisionState::Normal);
}

PurpleDinosaurStateIdle::~PurpleDinosaurStateIdle()
{
}

void PurpleDinosaurStateIdle::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateIdle::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//プレイヤー
	auto player = actorManager.lock()->GetPlayer();
	//距離を確認
	auto dis = coll->GetDistanceToPlayer(player);
	//プレイヤーを見つける距離
	if (dis <= kSearchDistance)
	{
		//プレイヤーを見る
		coll->LookAtPlayer(player);
		//攻撃の距離
		if (dis <= kAttackDistance)
		{
			//攻撃のクールタイムが0なら
			if (coll->GetAttackCoolTime() <= 0)
			{
				//攻撃状態にする
				ChangeState(std::make_shared<PurpleDinosaurStateAttack>(m_owner));
				return;
			}
		}
		//攻撃の距離じゃないときは近づく
		else
		{
			//プレイヤーをに近づく
			ChangeState(std::make_shared<PurpleDinosaurStateChase>(m_owner));
			return;
		}
	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}