#include "BossDragonStateBreathAttack.h"
#include "BossDragonStateDeath.h"
#include "BossDragonStateHit.h"
#include "BossDragonStateIdle.h"
#include "BossDragon.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackBase.h"
#include "../../../Attack/BulletAttack.h"
#include "../../../Attack/AttackManager.h"
#include "../../ActorManager.h"
#include "../../../../General/Math/Quaternion.h"

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
	constexpr int kBulletFireFrame = 25;
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
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 100;
}

BossDragonStateBreathAttack::BossDragonStateBreathAttack(std::shared_ptr<BossDragon> owner) :
	BossDragonStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//攻撃
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//プレイヤーを向く
	LookPlayer();
}

BossDragonStateBreathAttack::~BossDragonStateBreathAttack()
{
	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}
void BossDragonStateBreathAttack::Init()
{
	// 次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BossDragonStateBreathAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//攻撃マネージャー
	auto attackManager = actorManager->GetAttackManager();
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡
		ChangeState(std::make_shared<BossDragonStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BossDragonStateHit>(m_owner));
		return;
	}

	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kBulletFireFrame)
	{
		//攻撃判定の準備
		CreateAttack();
		//攻撃を入れる
		AppearAttack(m_bullet1, attackManager);
		AppearAttack(m_bullet2, attackManager);
		AppearAttack(m_bullet3, attackManager);
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BossDragonStateIdle>(m_owner));
	}

	//減速
	SpeedDown();
}

void BossDragonStateBreathAttack::CreateAttack()
{
	auto model = m_owner->GetModel();
	//生成位置
	Vector3 bulletPos = m_owner->GetCollidable()->GetRb()->GetPos();
	bulletPos.y += kBulletCreatePosY;
	//弾の作成
	CreateBullet(bulletPos, model,m_bullet1);
	CreateBullet(bulletPos, model,m_bullet2);
	CreateBullet(bulletPos, model,m_bullet3);
	//弾の進行方向とスピード
	Vector3 bulletDir = model->GetDir();
	m_bullet1->SetDirAndSpeed(bulletDir, kBulletSpeed);//真っすぐ飛ぶ
	bulletDir = Quaternion::AngleAxis(kBulletAngle,Vector3::Up()) *model->GetDir();
	m_bullet2->SetDirAndSpeed(bulletDir, kBulletSpeed);//斜めに飛ぶ
	bulletDir = Quaternion::AngleAxis(-kBulletAngle, Vector3::Up()) * model->GetDir();
	m_bullet3->SetDirAndSpeed(bulletDir, kBulletSpeed);//斜めに飛ぶ
}

void BossDragonStateBreathAttack::CreateBullet(Vector3& bulletPos, std::shared_ptr<Model>& model, std::shared_ptr<BulletAttack>& bullet)
{
	//弾の当たり判定作成
	auto coll = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kBulletRadius),
		std::make_shared<Rigidbody>(bulletPos));
	//弾の座標と当たり判定を攻撃に紐図ける
	bullet = std::make_shared<BulletAttack>(coll, kBulletDamage, kBulletKeepFrame, kKnockBackPower, m_owner);
}

void BossDragonStateBreathAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
