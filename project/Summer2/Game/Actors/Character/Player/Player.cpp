#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "PlayerStateRun.h"
#include "PlayerStateWin.h"
#include "../../Attack/AttackBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/AttackPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../ActorManager.h"
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
	//索敵距離
	constexpr float kSearchDistance = 200.0f;
	//視野角
	constexpr float kSearchAngle = 30.0f;
	//ダッシュ持続状態解除
	constexpr int kCancelRunFrame = 5;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f),
	m_isRunKeep(false),
	m_cancelRunFrame(0),
	m_initPos{}
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
	//体力ステータス
	m_hitPoints = std::make_shared<HitPoints>(kHp, Battle::Armor::Light);
	//攻撃ステータス
	m_attackPoints = std::make_shared<AttackPoints>();
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
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false,true);
	//Physicsに登録
	Collidable::Init();

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//初期化座標
	m_initPos = m_rb->m_pos;
}

void Player::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//落下した際の処理
	if (m_rb->m_pos.y < -500.0f)m_rb->m_pos = m_initPos;

	auto& input = Input::GetInstance();
	//スティックの向きを入れる
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);

	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetNearestEnemy();
	if (!target.expired())
	{
		TargetSearch(kSearchDistance, kSearchAngle, target.lock()->GetPos());
	}
	//走りを継続するか
	CheckRunKeep();
	//必殺ゲージが最大の時エフェクトをつける
	CheckUltMax();

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
	//体力ステータスの更新
	m_hitPoints->Update();
	//攻撃ステータスの更新
	m_attackPoints->Update();

#if _DEBUG
	if (input.IsTrigger("Max"))
	{
		m_ultGage->AddUltGage(10000);
		m_hitPoints->Heal(10000);
	}
#endif
}

void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
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

void Player::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
}

void Player::End()
{
	Collidable::End();
	m_model->End();
}

bool Player::IsFinishClearAnim()
{
	//勝利状態以外はfalse
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	//勝利状態の時にアニメーションが終了したらtrue
	return m_model->IsFinishAnim();
}

void Player::CheckRunKeep()
{
	//ダッシュ状態じゃないとき
	if (std::dynamic_pointer_cast<PlayerStateRun>(m_state) == nullptr)
	{
		++m_cancelRunFrame;
		//解除
		if (m_cancelRunFrame > kCancelRunFrame)
		{
			m_isRunKeep = false;
			m_cancelRunFrame = 0;
		}
	}
	else
	{
		m_cancelRunFrame = 0;
	}
}

void Player::CheckUltMax()
{
	//エフェクトをだしてなくて必殺ゲージ最大ならエフェクトをつける
	if (m_ultMaxEff.expired() && m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff = EffekseerManager::GetInstance().CreateTrackActorEffect("UltGageMaxEff", std::dynamic_pointer_cast<Actor>(shared_from_this()));
	}
	else if (!m_ultMaxEff.expired() && !m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff.lock()->Delete();
	}
}
