#include "Bomb.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../../Attack/HurtPoint.h"
#include "../../Attack/BlastAttack.h"
#include "../../Attack/AttackManager.h"
#include <DxLib.h>
#include <cmath>

namespace
{
	//モデルの大きさ
	const VECTOR kModelScale = { 0.5f, 0.5f, 0.5f };
	//当たり判定の半径
	constexpr float kCollRadius = 30.0f;
	//爆発の当たり判定の半径
	constexpr float kBlastRadius = 170.0f;
	//爆発までのフレーム数
	constexpr int kBlastFrame = 180;
	//爆発寸前のフレーム
	constexpr int kBeforeBlastFrame = 60;
	//モデルの拡大縮小のスピード
	constexpr float kSlowScaleSpeed = 10.0f;
	constexpr float kFastScaleSpeed = 25.0f;
	//拡大縮小の大きさ
	const Vector3 kAddScaleSize = { 0.1f, 0.1f, 0.1f };
	//爆発のダメージ
	constexpr int kBlastDamage = 200;
	//爆発のノックバック力
	constexpr float kBlastKnockBackPower = 25.0f;
	//爆発の持続時間
	constexpr float kBlastKeepFrame = 5.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

Bomb::Bomb(int modelHandle, Vector3 pos, Vector3 vec) :
	ItemBase(ItemKind::Heart),
	m_blastCountFrame(kBlastFrame),
	m_scaleSpeed(0.0f)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//モデルの大きさを設定
	m_model->SetScale(kModelScale);
	//衝突判定
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
	//モデルのサイズを保持
	m_originalScale = m_model->GetScale();
	//モデルの向きを180度回転させる
	m_model->SetRot({ 0.0f, 180.0f, 0.0f });
	//移動量
	m_collidable->GetRb()->SetVec(vec);
}

Bomb::~Bomb()
{
}

void Bomb::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void Bomb::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
}

void Bomb::Init()
{
	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
}

void Bomb::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//爆発のカウントを減らす
	m_blastCountFrame--;
	//爆発までのフレームが0になったら爆発
	if (m_blastCountFrame <= 0 && !m_isDelete)
	{
		//爆発の当たり判定を作成
		CreateAttack();
		//攻撃を登録
		actorManager->GetAttackManager()->Entry(m_blastAttack);
		//削除フラグを立てる
		m_isDelete = true;
		return;
	}
	//爆弾のアニメーション
	BombAnim();
	//減速
	SpeedDown();
}

void Bomb::BombAnim()
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

void Bomb::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void Bomb::OnHitColl(const std::shared_ptr<Collidable>& other)
{

}

void Bomb::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawSphere3D(
		m_collidable->GetRb()->GetPos().ToDxLibVector(),
		std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
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
	m_collidable->GetRb()->SetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void Bomb::CreateAttack()
{
	std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->SetRadius(kBlastRadius);
	//爆発の当たり判定を作成
	m_blastAttack = std::make_shared<BlastAttack>(m_collidable, kBlastDamage, kBlastKeepFrame, kBlastKnockBackPower, shared_from_this());
}

void Bomb::SpeedDown()
{
	//減速
	Vector3 vec = m_collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	m_collidable->GetRb()->SetVec(vec);
}
