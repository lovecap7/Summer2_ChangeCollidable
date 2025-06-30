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
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../ActorManager.h"
#include "../../../Attack/Strike.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 13;
	constexpr int kLeftHandIndex = 17;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kAttackRadius = 30.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 4;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 40;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 3.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Weapon";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 120;//2秒くらいの感覚で攻撃
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
	coll->SetCollState(CollisionState::Move);
	//攻撃
	coll->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//相手のほうを向く
	coll->LookAtTarget();
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//攻撃のクールタイム
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->SetAttackCoolTime(kAttackCoolTime);
	if (!m_attack.expired())m_attack.lock()->Delete();
}

void PurpleDinosaurStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
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
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		CreateAttack(actorManager);
	}
	//アニメーション終了後
	if (coll->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}
	//攻撃の位置更新
	if(!m_attack.expired()) UpdateAttackPos();

	//移動フレーム中は前に進む
	if (m_attackCountFrame <= kMoveFrame)
	{
		//前進
		AttackMove();
	}
	else
	{
		//減速
		coll->GetRb()->SpeedDown(kMoveDeceRate);
	}
}

void PurpleDinosaurStateAttack::CreateAttack(const std::weak_ptr<ActorManager> actorManager)
{
	//作成と参照
	m_attack = std::dynamic_pointer_cast<Strike>(actorManager.lock()->CreateAttack(AttackType::Strike, m_owner).lock());
	//攻撃を作成
	auto attack = m_attack.lock();
	attack->SetRadius(kAttackRadius);
	attack->AttackSetting(kAttackDamage, kAttackKeepFrame, kKnockBackPower, Battle::AttackWeight::Middle);
}

void PurpleDinosaurStateAttack::UpdateAttackPos()
{
	auto model = m_owner.lock()->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	//座標をセット
	m_attack.lock()->SetStartPos(leftArm);
	m_attack.lock()->SetEndPos(leftHand);
}

void PurpleDinosaurStateAttack::AttackMove()
{
	//向いてる方向に移動
	auto coll = std::dynamic_pointer_cast<PurpleDinosaur>(m_owner.lock());
	coll->GetRb()->SetMoveVec(coll->GetModel()->GetDir() * kMoveSpeed);
}