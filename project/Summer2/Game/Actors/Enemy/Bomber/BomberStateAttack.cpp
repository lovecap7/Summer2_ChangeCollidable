#include "BomberStateAttack.h"
#include "BomberStateDeath.h"
#include "BomberStateHit.h"
#include "BomberStateIdle.h"
#include "Bomber.h"
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
#include "../../Item/Bomb.h"
#include "../../Item/ItemGenerator.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//左腕と左手のインデックス
	constexpr int kLeftHandIndex = 17;
	//左腕の当たり判定の大きさ(攻撃の大きさ)
	constexpr float kLeftArmRadius = 20.0f;
	//攻撃の発生フレーム
	constexpr int kAttackStartFrame = 40;
	//アニメーション
	const char* kAnim = "CharacterArmature|Weapon";
	//アニメーションの速度
	constexpr float kAnimSpeed = 0.3f;
	//次の攻撃フレーム
	constexpr int kAttackCoolTime = 150;//2.5秒くらいの感覚で攻撃
	//爆弾の上昇量
	constexpr float kBombUpVecY = 10.0f; //爆弾の上昇量
	//爆弾の移動量
	constexpr float kBombMoveVecPower = 3.0f; //爆弾の移動量
}

BomberStateAttack::BomberStateAttack(std::shared_ptr<Bomber> owner) :
	BomberStateBase(owner),
	m_attackCountFrame(0)
{
	//通常攻撃
	m_owner->GetCollidable()->SetState(State::None);
	//攻撃
	m_owner->GetModel()->SetAnim(kAnim, false, kAnimSpeed);
	//モデルの向きをプレイヤーに向ける
	LookPlayer();
}

BomberStateAttack::~BomberStateAttack()
{
	//攻撃のクールタイム
	m_owner->SetAttackCoolTime(kAttackCoolTime);
}

void BomberStateAttack::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void BomberStateAttack::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager)
{
	//死んでるなら
	if (m_owner->GetHurtPoint()->IsDead())
	{
		//死亡状態
		ChangeState(std::make_shared<BomberStateDeath>(m_owner));
		return;
	}
	if (m_owner->GetHurtPoint()->IsHitReaction())
	{
		//やられ状態
		ChangeState(std::make_shared<BomberStateHit>(m_owner));
		return;
	}
	
	//カウント
	++m_attackCountFrame;
	//攻撃発生フレーム
	if (m_attackCountFrame == kAttackStartFrame)
	{
		//爆弾の作成
		CreateBomb(actorManager->GetItemGenerator());
	}
	//アニメーション終了後
	if (m_owner->GetModel()->IsFinishAnim())
	{
		//待機状態に戻す
		ChangeState(std::make_shared<BomberStateIdle>(m_owner));
		return;
	}

	//減速
	SpeedDown();
}

void BomberStateAttack::CreateBomb(const std::shared_ptr<ItemGenerator> itemGenerator)
{
	//生成位置
	VECTOR leftHand = MV1GetFramePosition(m_owner->GetModel()->GetModelHandle(), kLeftHandIndex);//手の指先
	//移動量
	Vector3 moveVec = m_owner->GetModel()->GetDir();
	moveVec *= kBombMoveVecPower;
	moveVec.y = kBombUpVecY; //上昇量を追加
	itemGenerator->GenerateBomb(leftHand, moveVec);
}

void BomberStateAttack::SpeedDown()
{
	auto collidable = m_owner->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
