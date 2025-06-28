#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
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
	constexpr float kBulletRadius = 40.0f;
	//弾のダメージ
	constexpr int kBulletDamage = 100;
	//弾の持続フレーム
	constexpr int kBulletKeepFrame = 180;
	//弾の発生フレーム
	constexpr int kBulletFireFrame = 40;
	//弾のスピード
	constexpr float kBulletSpeed = 7.0f;
	//弾の発射角度
	constexpr float kBulletAngle = 30.0f / 180.0f * DX_PI_F;
	//弾の生成位置のY座標
	constexpr float kBulletCreatePosY = 150.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 12.0f;

	//アニメーション
	const char* kAnim = "CharacterArmature|Headbutt";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.2f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 100;
}


BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::weak_ptr<Actor> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//通常攻撃
	coll->SetCollState(CollisionState::Normal);
	//攻撃
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

BossDragonStateBreathAttack::~BossDragonStateBreathAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStateBreathAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateBreathAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//死亡
	if (coll->GetHitPoints()->IsDead())
	{
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	//ヒットリアクション
	if (coll->GetHitPoints()->IsHitReaction())
	{
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
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
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}

	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}

void BossDragonStateBreathAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack1 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	m_attack2 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	m_attack3 = std::dynamic_pointer_cast<Bullet>(actorManager.lock()->CreateAttack(AttackType::Bullet, m_owner).lock());
	//弾の設定
	SetupBullet(m_attack1, 0.0f);
	SetupBullet(m_attack2, kBulletAngle);
	SetupBullet(m_attack3, -kBulletAngle);
}

void BossDragonStateBreathAttack::SetupBullet(std::weak_ptr<Bullet> bullet, float angle)
{
	auto coll = std::dynamic_pointer_cast<BossDragon>(m_owner.lock());
	//弾の設定
	auto attack = bullet.lock();
	attack->SetRadius(kBulletRadius);
	attack->AttackSetting(kBulletDamage, kBulletKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
	//生成位置
	Vector3 bulletPos = coll->GetPos();
	bulletPos.y += kBulletCreatePosY;
	attack->SetPos(bulletPos);
	//弾の進行方向とスピード
	auto vec = coll->GetModel()->GetDir() * kBulletSpeed;
	vec = Quaternion::AngleAxis(angle, Vector3::Up()) * vec;
	attack->SetVec(vec);
}
