#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Attack/Blast.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//爆発までのフレーム数
	constexpr int kBlastFrame = 180;
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 50.0f;
	//最初の当たらないフレーム
	constexpr int kNoHitFrame = 10;

	//爆発寸前のフレーム
	constexpr int kBeforeBlastFrame = 60;
	//モデルの大きさ
	const VECTOR kModelScale = { 0.7f, 0.7f, 0.7f };
	//拡大縮小の大きさ
	const Vector3 kAddScaleSize = { 0.1f, 0.1f, 0.1f };
	//モデルの拡大縮小のスピード
	constexpr float kSlowScaleSpeed = 10.0f;
	constexpr float kFastScaleSpeed = 25.0f;

	//爆発の範囲
	constexpr float kBlastRadius = 170.0f;
	//爆発のダメージ
	constexpr int kBlastDamage = 200;
	//爆発のノックバック力
	constexpr float kBlastKnockBackPower = 25.0f;
	//爆発の持続時間
	constexpr float kBlastKeepFrame = 5.0f;
}

Bomb::Bomb(int modelHandle, Vector3 pos) :
	ItemBase(Shape::Sphere),
	m_blastCountFrame(kBlastFrame),
	m_scaleSpeed(0.0f),
	m_originalScale(kModelScale)
{
	m_noHitFrame = kNoHitFrame;
	//座標
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	m_rb->m_pos = firstPos;
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->m_radius = kCollRadius;
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//力を与える
	m_rb->SetVecY(kJumpPower);
}

Bomb::~Bomb()
{
}

void Bomb::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physicsに登録
	Collidable::Init();
}

void Bomb::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//床にいるとき
	if (m_isFloor)
	{
		//移動量を初期化
		m_rb->SetMoveVec(Vector3::Zero());
	}
	//当たり判定を無視する
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//当たり判定をする
		m_isThrough = false;
	}
	//爆発までのカウント
	if (m_blastCountFrame > 0)
	{
		--m_blastCountFrame;
		//爆弾の自作アニメーション
		AnimBomb();
	}
	else
	{
		//消滅
		m_isDelete = true;
	}
	m_model->Update();
}


void Bomb::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Bomb::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawSphere3D(
		GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->m_radius,
		16,
		0xff0000,
		0xff0000,
		false
	);
#endif
	m_model->Draw();
}

void Bomb::Complete()
{
	//次の座標へ
	m_rb->m_pos = m_rb->GetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void Bomb::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//爆発の攻撃判定を出す
	auto attack = actorManager.lock()->CreateAttack(AttackType::Blast, std::dynamic_pointer_cast<Actor>(shared_from_this()));
	auto blast = std::dynamic_pointer_cast<Blast> (attack.lock());
	blast->SetPos(m_rb->m_pos);
	blast->SetRadius(kBlastRadius);
	blast->AttackSetting(kBlastDamage, kBlastKeepFrame, kBlastKnockBackPower, Battle::AttackWeight::Heavy);
}

void Bomb::End()
{
	Collidable::End();
	m_model->End();
}

void Bomb::SetVec(Vector3 vec)
{
	m_rb->m_vec = vec;
}

void Bomb::AnimBomb()
{
	if (m_blastCountFrame <= kBeforeBlastFrame)
	{
		//爆発までの残り時間が短いとき早くなる
		m_scaleSpeed += kFastScaleSpeed;
	}
	else
	{
		//それ以外はゆっくり拡大縮小
		m_scaleSpeed += kSlowScaleSpeed;
	}
	Vector3 modelScale = m_originalScale + kAddScaleSize * cosf(m_scaleSpeed * MyMath::DEG_2_RAD);
	m_model->SetScale(modelScale.ToDxLibVector());
}