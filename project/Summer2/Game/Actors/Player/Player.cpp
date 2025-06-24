#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateWalk.h"
#include "PlayerStateDeath.h"
#include "PlayerStateIdle.h"
#include "../../../General/game.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Collision/SearchTrigger.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/AttackManager.h"
#include "../ActorManager.h"
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
	m_stickVec(0.0f,0.0f),
	m_isGround(false),
	m_damageCutFrame(0),
	m_damageUpRate(1.0f),
	m_damageUpKeepFrame(0)
{
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//衝突判定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	m_collidable = std::make_shared<Collidable>(std::make_shared<CapsuleCollider>(endPos, kCapsuleRadius), std::make_shared<Rigidbody>(firstPos));
	//必殺技ゲージ
	m_ultGage = std::make_shared<UltGage>(kMaxUltGage);
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Middle, GameTag::Player);
}

Player::~Player()
{
}

void Player::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void Player::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーから登録解除
	actorManager->Exit(shared_from_this());
}

void Player::Init()
{
	//コンストラクタで全部初期化したかったけど
	//shared_from_this()を使う場合コンストラクタ時点では
	//メモリが確定していないので使うことができない
	//対策としてInitを使う

	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
	//索敵範囲
	m_searchTrigger = std::make_shared<SearchTrigger>(kSearchTriggerRadius,kSearchAngle,shared_from_this());
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = shared_from_this();
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//次の状態を待機状態に
	m_state->ChangeState(m_state);
	//やられ判定(衝突判定と同じにする)
	m_hurtPoint = std::make_shared<HurtPoint>(m_collidable, kHp, thisPointer);
}

void Player::Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//スティックの向きを入れる
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);

#if _DEBUG
	if (input.IsTrigger("Max"))
	{
		//ゲージマックス
		m_ultGage->AddUltGage(1000000);
		//体力マックス
		m_hurtPoint->AddHp(1000000);
	}
#endif

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

void Player::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Player::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//地面に当たった時の処理
	if (other->GetColl()->GetShape() == Shape::Polygon)
	{
		if (std::dynamic_pointer_cast<PolygonCollider>(other->GetColl())->IsFloor())
		{
			m_isGround = true;
		}
	}
}

void Player::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawCapsule3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		m_isGround//地面にいると塗りつぶされる
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
	printf("POS = %2f, %2f, %2f\n", m_collidable->GetRb()->GetPos().x, m_collidable->GetRb()->GetPos().y, m_collidable->GetRb()->GetPos().z);
	printf("VEC = %2f, %2f, %2f\n", m_collidable->GetRb()->GetVec().x, m_collidable->GetRb()->GetVec().y, m_collidable->GetRb()->GetVec().z);
	DrawSphere3D(
		m_searchTrigger->GetCollidable()->GetRb()->GetPos().ToDxLibVector(),
		kSearchTriggerRadius,
		16,
		0x00ff00,
		0x00ff00,
		false
	);
#endif
	m_model->Draw();
}

void Player::Complete()
{
	m_collidable->GetRb()->SetNextPos();//次の座標へ
	Vector3 endPos = m_collidable->GetRb()->GetPos();
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
	//索敵位置更新
	m_searchTrigger->SetPos(m_collidable->GetRb()->GetPos());
	//索敵の前を更新
	m_searchTrigger->SetViewForward(m_model->GetDir());
}

void Player::SetDamageCut(float cutRate, int keepFrame)
{
	//ダメージカット
	m_hurtPoint->SetDamageCutRate(cutRate);
	m_damageCutFrame = keepFrame;
	//アーマーを強く
	m_hurtPoint->SetArmor(Battle::Armor::Heavy);
}

void Player::UpdateHurtPoint()
{
	//移動量を取得
	m_hurtPoint->GetCollidable()->GetRb()->SetVec(m_collidable->GetRb()->GetVec());
	//座標更新
	m_hurtPoint->GetCollidable()->GetRb()->SetPos(m_collidable->GetRb()->GetPos());
	std::dynamic_pointer_cast<CapsuleCollider>(m_hurtPoint->GetCollidable()->GetColl())->SetEndPos(std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos());
	//ダメージカットのフレームの更新
	if (m_damageCutFrame > 0)
	{
		--m_damageCutFrame;
	}
	else
	{
		//もとに戻る
		m_hurtPoint->SetDamageCutRate(1.0f);
		m_hurtPoint->SetArmor(Battle::Armor::Middle);
	}
}