#include "Bomber.h"
#include "BomberStateBase.h"
#include "BomberStateIdle.h"
#include "../../CharacterStateBase.h"
#include "../../../ActorManager.h"
#include "../../Player/Player.h"
#include <memory>
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../Game/Camera/Camera.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Collision/CapsuleCollider.h"
#include "../../../../../General/Collision/SphereCollider.h"
#include "../../../../../General/game.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../General/AttackPoints.h"
#include "../../../../../General/Effect/EffekseerManager.h"
#include "../../../../GameRule/Score.h"

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 40.0f; //カプセルの半径
	//体力
	constexpr int kHp = 500;
	//プレイヤーを発見する距離
	constexpr float kSearchDistance = 900.0f;
	//プレイヤーを発見する視野角
	constexpr float kSearchAngle = 180.0f * MyMath::DEG_2_RAD;
	//体力バー表示位置
	constexpr float kHPBarY = 30.0f;
	//モデルの旋回速度
	constexpr int kRotaSpeed = 30;
}
Bomber::Bomber(int modelHandle, Vector3 pos) :
	EnemyBase(Shape::Capsule, EnemyGrade::Normal)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	m_model->SetRotSpeed(kRotaSpeed);
	//衝突判定
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(kCapsuleRadius);
	cap->SetEndPos(endPos);
	//リジッドボディの初期化
	m_rb->SetPos(pos);
	//体力ステータス
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Light);
	//攻撃ステータス
	m_attackPoints = std::make_shared<AttackPoints>();
}

Bomber::~Bomber()
{
}

void Bomber::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Enemy, false, false, true);
	//Physicsに登録
	Collidable::Init();

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Bomber>(shared_from_this());
	m_state = std::make_shared<BomberStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
}

void Bomber::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//攻撃のクールタイムを減らす
	UpdateAttackCoolTime();
	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetPlayer();
	if (!target.expired())
	{
		TargetSearch(kSearchDistance, kSearchAngle, target.lock()->GetPos());
	}
	//状態に合わせた更新
	m_state->Update(camera, actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//体力クラスのフラグリセット
	m_hitPoints->ResetHitFlags();
	//頭の位置
	m_hpUIPos = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->m_endPos;
	m_hpUIPos.y += kHPBarY;
}

void Bomber::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Bomber::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//探索範囲
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(), kSearchDistance, 4, 0x0000ff, 0x0000ff, false);
	//見てる方向
	auto forward = m_model->GetDir();
	forward = forward * kSearchDistance;
	//視野角
	auto viewDir1 = Quaternion::AngleAxis(kSearchAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-kSearchAngle / 2.0f, Vector3::Up()) * forward;
	//描画
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir2).ToDxLibVector(), 0xff0000);
#endif
	m_model->Draw();
}

void Bomber::Complete()
{
	m_rb->m_pos = m_rb->GetNextPos();//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->GetPos().ToDxLibVector());
}

void Bomber::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	if (!m_hitPoints->IsDead())return;//体力がなくなっていない場合は無視
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kBomber);
	//アイテムをランダムで落とす
	auto actorM = actorManager.lock();
	if (GetRand(1))
	{
		//必殺ゲージアップ
		actorM->CreateItem(ItemType::UltGageUp, m_rb->GetPos());
	}
	else
	{
		//防御バフ
		actorM->CreateItem(ItemType::DefenseUp, m_rb->GetPos());
	}
	//死亡エフェクト
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
}

void Bomber::End()
{
	Collidable::End();
	m_model->End();
}

