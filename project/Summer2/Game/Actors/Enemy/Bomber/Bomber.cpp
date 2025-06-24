#include "Bomber.h"
#include "BomberStateBase.h"
#include "BomberStateIdle.h"
#include "../EnemyManager.h"
#include <memory>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../Attack/HurtPoint.h"
#include "../../../Attack/MeleeAttack.h"
#include "../../../../General/Collision/SearchTrigger.h"
#include "../../ActorManager.h"
#include "../../../../General/game.h"
namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,120.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 40.0f; //カプセルの半径
	//トリガーの半径
	constexpr float kSearchTriggerRadius = 500.0f;
	//プレイヤーを追いかける距離
	constexpr float kRunDistance = 150.0f;
	//体力
	constexpr int kHp = 500;
}

Bomber::Bomber(int modelHandle, Vector3 pos):
	EnemyBase(),
	m_attackCoolTime(0)
{
	//モデルの初期化
	m_model = std::make_unique<Model>(modelHandle, pos.ToDxLibVector());
	//衝突判定
	Vector3 endPos = pos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(pos));
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Middle, GameTag::Enemy);
}
Bomber::~Bomber()
{
}

void Bomber::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
	//登録
	actorManager->GetEnemyManager()->Entry(shared_from_this());
}

void Bomber::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
	//登録解除
	actorManager->GetEnemyManager()->Exit(shared_from_this());
}

void Bomber::Init()
{
	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
	//索敵範囲
	m_searchTrigger = std::make_shared<SearchTrigger>(kSearchTriggerRadius, shared_from_this());
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = shared_from_this();
	m_state = std::make_shared<BomberStateIdle>(thisPointer);
	//次の状態を待機状態に
	m_state->ChangeState(m_state);
	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, kHp, thisPointer);
}

void Bomber::Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager)
{
	//攻撃のクールタイムを減らす
	UpdateAttackCoolTime();
	//状態に合わせた更新
	m_state->Update(input, camera, actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//やられ判定の更新
	UpdateHurtPoint();
	//攻撃を喰らったならモデルを赤くする
	if (m_hurtPoint->IsHit())
	{
		//赤色に
		m_model->ModelHit();
	}
}

void Bomber::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomber::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//なし
}

void Bomber::Draw() const
{
#if _DEBUG
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false
	);
	//やられ判定
	DrawCapsule3D(
		m_hurtPoint->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->GetRadius(),
		32,
		0x0000ff,
		0x0000ff,
		m_hurtPoint->IsNoDamege()//無敵の時は塗りつぶされる
	);
	//索敵範囲
	DrawSphere3D(
		m_searchTrigger->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		kSearchTriggerRadius,
		16,
		0xff00ff,
		0xff00ff,
		false
	);
#endif
	m_model->Draw();
}

void Bomber::Complete()
{
	//コライダー
	m_collidable->GetRb()->SetNextPos();//次の座標へ
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//カプセルの移動
	//プレイヤーを探すトリガー
	m_searchTrigger->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomber::UpdateHurtPoint()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
}

void Bomber::UpdateAttackCoolTime()
{
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}
