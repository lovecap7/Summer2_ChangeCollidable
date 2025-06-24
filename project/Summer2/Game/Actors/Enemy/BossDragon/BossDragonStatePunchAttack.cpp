#include "BossDragonStatePunchAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragonStateBase.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
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
	//右手のインデックス
	constexpr int kRightHandIndex = 36;
	//パンチの当たり判定の大きさ(攻撃の大きさ)
	constexpr float kPunchRadius = 100.0f;
	//攻撃のダメージ
	constexpr int kAttackDamage = 100;
	//攻撃の持続フレーム
	constexpr int kAttackKeepFrame = 3;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 26;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 20.0f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Punch";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.4f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 40;
}

BossDragonStatePunchAttack::BossDragonStatePunchAttack(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//攻撃
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//攻撃判定の準備
	CreateAttack();
	//プレイヤーを向く
	LookPlayer();
}

BossDragonStatePunchAttack::~BossDragonStatePunchAttack()
{
	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BossDragonStatePunchAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStatePunchAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
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
		//攻撃を入れる
		AppearAttack(m_attack, attackManager);
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
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

void BossDragonStatePunchAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//右手の座標
	VECTOR rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//右手
	auto punch = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kPunchRadius),
		std::make_shared<Rigidbody>(rightHandPos));
	//武器の座標と当たり判定を攻撃に紐図ける
	m_attack = std::make_shared<MeleeAttack>(punch, kAttackDamage, kAttackKeepFrame, kKnockBackPower,m_owner);
}

void BossDragonStatePunchAttack::UpdateAttack()
{
	auto model = m_owner->GetModel();
	//右手の座標
	VECTOR rightHandPos = MV1GetFramePosition(model->GetModelHandle(), kRightHandIndex);//右手
	//右手の座標をセット
	m_attack->GetCollidable()->GetRb()->SetPos(rightHandPos);
}

void BossDragonStatePunchAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
void BossDragonStatePunchAttack::DeleteAttack(const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃判定を消す
	m_attack->Delete();
	attackManager->Exit(m_attack);
}
