#include "PurpleDinosaurStateAttack.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateHit.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../Attack/AttackManager.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/MeleeAttack.h"
#include "../../ActorManager.h"
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

PurpleDinosaurStateAttack::PurpleDinosaurStateAttack(std::shared_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//攻撃
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//攻撃判定の準備
	CreateAttack();
	//モデルの向きをプレイヤーに向ける
	LookPlayer();
}

PurpleDinosaurStateAttack::~PurpleDinosaurStateAttack()
{
	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void PurpleDinosaurStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//削除
		DeleteAttack(attackManager);
		//死亡状態
		ChangeState(std::make_shared<PurpleDinosaurStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//削除
		DeleteAttack(attackManager);
		//やられ状態
		ChangeState(std::make_shared<PurpleDinosaurStateHit>(m_owner));
		return;
	}
	//攻撃の位置更新
	UpdateAttack();
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//攻撃を入れる
		AppearAttack(m_attack, attackManager);
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//削除
		DeleteAttack(attackManager);
		//待機状態に戻す
		ChangeState(std::make_shared<PurpleDinosaurStateIdle>(m_owner));
		return;
	}

	//減速
	SpeedDown();
	//攻撃時に前進する
	AttackMove();
}

void PurpleDinosaurStateAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius),
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
	//武器の座標と当たり判定を攻撃に紐図ける
	m_attack = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, kKnockBackPower,m_owner);
}

void PurpleDinosaurStateAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	//左腕を形成するカプセルの座標をセット
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void PurpleDinosaurStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PurpleDinosaurStateAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attack->Delete();
	attackManager->Exit(m_attack);
}

void PurpleDinosaurStateAttack::AttackMove()
{
	//移動フレーム中は前に進む
	if (m_attackCountFrame <= kMoveFrame)
	{
		//向いてる方向に移動
		m_owner->GetCollidable()->GetRb()->SetMoveVec(m_owner->GetModel()->GetDir() * kMoveSpeed);
	}
}