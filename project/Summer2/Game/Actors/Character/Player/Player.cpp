#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "../../Attack/AttackBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"
#include "UltGage.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f; //カプセルの半径
	//体力
	constexpr int kHp = 1000; 
	//必殺技ゲージの最大値
	constexpr int kMaxUltGage = 100;
	//トリガーの半径
	constexpr float kSearchTriggerRadius = 200.0f;
	//視野角
	constexpr float kSearchAngle = 20.0f * MyMath::DEG_2_RAD;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f)
{
	//座標
	m_rb->m_pos = firstPos;
	//カプセルの終点の設定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(endPos);
	cap->SetRadius(kCapsuleRadius);
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//必殺技ゲージ
	m_ultGage = std::make_shared<UltGage>(kMaxUltGage);
	//体力
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Middle);
}

Player::~Player()
{
}

void Player::Init()
{
	//コンストラクタで全部初期化したかったけど
	//shared_from_this()を使う場合コンストラクタ時点では
	//メモリが確定していないので使うことができない
	//対策としてInitを使う

	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false);
	//Physicsに登録
	Collidable::Init();

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//次の状態を待機状態に
	m_state->ChangeState(m_state);
}

void Player::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto& input = Input::GetInstance();
	//スティックの向きを入れる
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);

#if _DEBUG

#endif
	//状態に合わせた更新
	m_state->Update(camera,actorManager);
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
}
void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
	//攻撃を受けたときの処理
	if (other->GetGameTag() == GameTag::Attack)
	{
		OnHitFromAttack(other);
		return;
	}
}

void Player::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false//地面にいると塗りつぶされる
	);

#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_rb->m_pos += m_rb->m_vec;//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}
