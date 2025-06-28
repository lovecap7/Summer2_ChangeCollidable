#include "SmallDragonStateAttack.h"
#include "SmallDragonStateIdle.h"
#include "SmallDragonStateHit.h"
#include "SmallDragonStateDeath.h"
#include "SmallDragon.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../Attack/Bullet.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//弾の半径の大きさ
	constexpr float kBulletRadius = 20.0f;
	//弾のダメージ
	constexpr int kBulletDamage = 100;
	//弾の持続フレーム
	constexpr int kBulletKeepFrame = 180;
	//弾の発生フレーム
	constexpr int kBulletFireFrame = 20;
	//弾のスピード
	constexpr float kBulletSpeed = 3.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 4.0f;
	//生成位置の高さ
	constexpr float kBulletCreatePosY = 100.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Headbutt";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;//2.5秒くらいの感覚で攻撃
}

SmallDragonStateAttack::SmallDragonStateAttack(std::weak_ptr<Actor> owner) :
	SmallDragonStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

SmallDragonStateAttack::~SmallDragonStateAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void SmallDragonStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void SmallDragonStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<SmallDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<SmallDragonStateHit>(m_owner));
		return;
	}
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kBulletFireFrame)
	{
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<SmallDragonStateIdle>(m_owner));
		return;
	}
	
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void SmallDragonStateAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<SmallDragon>(m_owner.lock());
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(kBulletRadius);
	attack->AttackSetting(kBulletDamage, kBulletKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
	//生成位置
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//弾の進行方向とスピード
	attack->SetVec(coll->GetModel()->GetDir() * kBulletSpeed);
}