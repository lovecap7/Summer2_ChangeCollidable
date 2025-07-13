#include "AttackUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/TrackActorEffect.h"
#include "../ActorManager.h"
#include "../Character/Player/Player.h"
#include "../../GameRule/Score.h"

namespace
{
	//ダメージ倍率
	constexpr float kDamageRate = 1.5f;
	//持続フレーム
	constexpr int kAttackUpKeepFrame = 60 * 15;//15秒
	//ジャンプ力
	constexpr float kJumpPower = 10.0f;
	//当たり判定の半径
	constexpr float kCollRadius = 80.0f;
	//回転量
	constexpr float kRotaAngle = 1.0f;
	//最初の当たらないフレーム
	constexpr int kNoHitFrame = 30;
}

AttackUp::AttackUp(int modelHandle, Vector3 pos) :
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

AttackUp::~AttackUp()
{
}

void AttackUp::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Item, true, false, true);
	//Physicsに登録
	Collidable::Init();
}

void AttackUp::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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

void AttackUp::OnCollide(const std::shared_ptr<Collidable> other)
{
	//消滅フラグが立ってるならリターン
	if (m_isDelete)return;
	//プレイヤーに当たった時の処理
	if (other->GetGameTag() == GameTag::Player)
	{
		//攻撃アップバフ
		auto player = std::dynamic_pointer_cast<Player>(other);
		player->GetAttackPoints().lock()->AttackBuff(kDamageRate, Battle::AttackWeight::Heavy, kAttackUpKeepFrame);
		//パワーアップエフェクト
		EffekseerManager::GetInstance().CreateTrackActorEffect("GetAttackUpEff", player);
		auto eff = EffekseerManager::GetInstance().CreateTrackActorEffect("AttackUpEff", player);
		eff.lock()->SpecificFrame(kAttackUpKeepFrame);
		//削除
		m_isDelete = true;
	}
}

void AttackUp::Draw() const
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

void AttackUp::Complete()
{
	//次の座標へ
	m_rb->m_pos = m_rb->GetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void AttackUp::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//スコア加算
	score.lock()->AddKillOrItemScore(ScoreDataName::kAttackUp);
}

void AttackUp::End()
{
	Collidable::End();
	m_model->End();
}
