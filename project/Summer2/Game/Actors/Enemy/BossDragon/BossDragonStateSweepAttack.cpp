#include "BossDragonStateSweepAttack.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
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
#include "../../../../General/Math/Quaternion.h"
namespace
{
	//攻撃の全体フレーム
	constexpr int kAllAttackFrame = 110;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftArmIndex = 31;
	constexpr int kLeftHandIndex = 36;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kLeftArmRadius = 20.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 40;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = kAllAttackFrame - (kAttackStartFrame + 30);
	//予備動作中の回転
	constexpr float kStartRotaAngle = 1.0f;
	//攻撃の回転量
	constexpr float kAttackRotaAngle = -(360.0f + kStartRotaAngle * kAttackStartFrame) / static_cast<float>(kAttackKeepFrame);
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 24.0f;
	//アニメーション
	const char* kAttackAnim = "CharacterArmature|Punch";
	const char* kStartAnim = "CharacterArmature|No";
	//アニメーションの速度
	constexpr float kAttackAnimSpeed = 0.6f;
	constexpr float kStartAnimSpeed = 0.5f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 30;
}


BossDragonStateSweepAttack::BossDragonStateSweepAttack(std::shared_ptr<BossDragon> owner):
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//予備動作
	m_owner->GetModel()->SetAnim(kStartAnim, false, kStartAnimSpeed);
	//攻撃判定の準備
	CreateAttack();
	//プレイヤーを向く
	LookPlayer();
}

BossDragonStateSweepAttack::~BossDragonStateSweepAttack()
{

	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStateSweepAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}
void BossDragonStateSweepAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//削除
		DeleteAttack(attackManager);
		//死亡状態
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//削除
		DeleteAttack(attackManager);
		//やられ状態
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}
	//攻撃の位置更新
	UpdateAttack();
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//予備動作
		m_owner->GetModel()->SetAnim(kAttackAnim, false, kAttackAnimSpeed);
		//攻撃を入れる
		AppearAttack(m_attack, attackManager);
	}

	//攻撃が終了するまで回転
	auto model = m_owner->GetModel();
	if (m_attackCountFrame >= kAttackStartFrame)
	{
		if (!m_attack->IsDelete())
		{
			model->SetRot(VGet(0.0f, kAttackRotaAngle, 0.0f));
		}
	}
	else
	{
		//最初の予備動作中も少し回転
		model->SetRot(VGet(0.0f, kStartRotaAngle, 0.0f));
	}

	//攻撃終了
	if (m_attackCountFrame >= kAllAttackFrame)
	{
		//削除
		DeleteAttack(attackManager);
		//待機状態に戻す
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
		return;
	}
	//減速
	SpeedDown();
}

void BossDragonStateSweepAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	m_leftArm = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(Vector3(leftArm.x, leftArm.y, leftArm.z), kLeftArmRadius),
		std::make_shared<Rigidbody>(Vector3(leftHand.x, leftHand.y, leftHand.z)));
	//武器の座標と当たり判定を攻撃に紐図ける
	m_attack = std::make_shared<MeleeAttack>(m_leftArm, kAttackDamage, kAttackKeepFrame, kKnockBackPower, m_owner);
}

void BossDragonStateSweepAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//腕と手の座標
	VECTOR leftArm = MV1GetFramePosition(model->GetModelHandle(), kLeftArmIndex);//左腕
	VECTOR leftHand = MV1GetFramePosition(model->GetModelHandle(), kLeftHandIndex);//手の指先
	//左腕を形成するカプセルの座標をセット
	m_leftArm->GetRb()->SetPos(Position3(leftArm.x, leftArm.y, leftArm.z));
	std::dynamic_pointer_cast<CapsuleCollider>(m_leftArm->GetColl())->SetEndPos(Position3(leftHand.x, leftHand.y, leftHand.z));
}

void BossDragonStateSweepAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void BossDragonStateSweepAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attack->Delete();
	attackManager->Exit(m_attack);
}