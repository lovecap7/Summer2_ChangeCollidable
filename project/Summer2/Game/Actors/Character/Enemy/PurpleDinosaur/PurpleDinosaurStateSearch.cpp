#include "PurpleDinosaurStateSearch.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../ActorManager.h"
#include "../../../SearchPlace.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//アニメーション
	const char* kAnim = "CharacterArmature|Walk";
	//速度
	constexpr float kMoveSpeed = 1.0f;
	//移動フレーム
	constexpr int kMoveFrame = 300;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 5;
}

PurpleDinosaurStateSearch::PurpleDinosaurStateSearch(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner),
	m_moveFrame(kMoveFrame)
{
	if (m_owner.expired())return;
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//アニメーション
	coll->GetModel()->SetAnim(kAnim, true);
	//状態
	coll->SetCollState(CollisionState::Move);
	//ランダムな方向に移動
	m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
	//警戒はしていない
	coll->SetIsWarning(false);
}

PurpleDinosaurStateSearch::~PurpleDinosaurStateSearch()
{
}

void PurpleDinosaurStateSearch::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateSearch::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//コライダブル
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints().lock()->IsDead())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints().lock()->IsHitReaction())
	{
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
		return;
	}
	//壁に当たったら
	if (coll->IsWall() || m_moveFrame < 0.0f)
	{
		m_moveFrame = kMoveFrame;
		//ランダムな方向に移動
		m_moveVec = Vector3::GetRandVecXZ() * kMoveSpeed;
	}
	//索敵範囲外に出た時
	if (!coll->GetSearchPlace().expired())
	{
		std::shared_ptr<SearchPlace> searchPlace = coll->GetSearchPlace().lock();
		if (!searchPlace->IsInSearchPlace(coll->GetPos()))
		{
			//範囲内に向かって動く
			m_moveVec = searchPlace->GetPos() - coll->GetPos();
			if (m_moveVec.SqMagnitude() > 0.0f)
			{
				m_moveVec = m_moveVec.Normalize() * kMoveSpeed;
			}
		}
	}
	//移動フレーム
	--m_moveFrame;
	//ターゲット
	auto targetData = coll->GetTargetData();
	//プレイヤーを見つけた
	if (targetData.isHitTarget)
	{
		//待機状態
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}
	//移動
	coll->GetRb()->SetMoveVec(m_moveVec);
	//モデルの向き
	coll->GetModel()->SetDir(m_moveVec.XZ());
}
