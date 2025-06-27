#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../../General/game.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 13;
	constexpr int kLeftHandIndex = 17;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kLeftArmRadius = 20.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 2;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 40;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Weapon";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;//2.5秒くらいの感覚で攻撃
	//移動フレーム
	constexpr int kMoveFrame = 5;
	//移動量
	constexpr float kMoveSpeed = 10.0f;
}

PurpleDinosaurStateAttack::PurpleDinosaurStateAttack(std::weak_ptr<Actor> owner):
	PurpleDinosaurStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void PurpleDinosaurStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
