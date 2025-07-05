#include "UltGageUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../Character/Player/UltGage.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"
namespace
{
	//ゲージ上昇量
	constexpr float kGageValue = 20.0f;
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 50.0f;
	//回転量
	constexpr float kRotaAngle = 2.0f;
	//最初の当たらないフレーム
	constexpr int kNoHitFrame = 30;
}

UltGageUp::UltGageUp(int modelHandle, Vector3 pos) :
	ItemBase(Shape::Sphere)
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

UltGageUp::~UltGageUp()
{
}

void UltGageUp::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physicsに登録
	Collidable::Init();
}

void UltGageUp::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//移動量を初期化
	m_rb->SetMoveVec(Vector3::Zero());
	//回る
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//当たり判定をする
		m_isThrough = false;
	}
	m_model->Update();
}

void UltGageUp::OnCollide(const std::shared_ptr<Collidable> other)
{
	//消滅フラグが立ってるならリターン
	if (m_isDelete)return;
	//プレイヤーに当たった時の処理
	if (other->GetGameTag() == GameTag::Player)
	{
		//ゲージアップ
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetUltGage().lock()->AddUltGage(kGageValue);
		//削除
		m_isDelete = true;
	}
}

void UltGageUp::Draw() const
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

void UltGageUp::Complete()
{
	//次の座標へ
	m_rb->m_pos = m_rb->GetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void UltGageUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//スコア加算
	score.lock()->AddKillScore(ScoreDataName::kUltGageUp);
}

void UltGageUp::End()
{
	Collidable::End();
	m_model->End();
}
