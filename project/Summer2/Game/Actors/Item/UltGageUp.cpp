#include "UltGageUp.h"
#include "../../../General/game.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"
#include "../Player/Player.h"
#include "../Player/UltGage.h"

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
	ItemBase(ItemKind::UltGageUp),
	m_noHitFrame(kNoHitFrame)
{
	auto firstPos = pos;
	firstPos.y += kCollRadius;
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//衝突判定
	m_collidable = std::make_shared<Collidable>(std::make_shared<SphereCollider>(kCollRadius), std::make_shared<Rigidbody>(firstPos));
	//力を与える
	m_collidable->GetRb()->SetVecY(kJumpPower);
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Low, GameTag::Item);
	//当たり判定をしない
	m_collidable->SetIsCollide(false);
}

UltGageUp::~UltGageUp()
{
}

void UltGageUp::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void UltGageUp::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
}

void UltGageUp::Init()
{
	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
}

void UltGageUp::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//移動量を初期化
	m_collidable->GetRb()->SetMoveVec(Vector3::Zero());
	//回る
	m_model->SetRot(VGet(0.0f, kRotaAngle, 0.0f));
	if (m_noHitFrame > 0)
	{
		--m_noHitFrame;
	}
	else
	{
		//当たり判定をする
		m_collidable->SetIsCollide(true);
	}
}

void UltGageUp::Gravity(const Vector3& gravity)
{
	//重力がかかりすぎたら止めたいので上限を設ける
	if (m_collidable->GetRb()->GetVec().y >= Gravity::kMaxGravityY)
	{
		//重力
		m_collidable->GetRb()->AddVec(gravity);
	}
}

void UltGageUp::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//消滅フラグが立ってるならリターン
	if (m_isDelete)return;
	//プレイヤーに当たった時の処理
	if (other->GetGameTag() == GameTag::Player)
	{
		//ゲージ上昇
		auto player = std::dynamic_pointer_cast<Player>(other->GetOwner());
		player->GetUltGage()->AddUltGage(kGageValue);
		//削除
		m_isDelete = true;
	}
}

void UltGageUp::Draw() const
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

void UltGageUp::Complete()
{
	//次の座標へ
	m_collidable->GetRb()->SetNextPos();
	//モデルの座標更新
	m_model->SetPos(m_collidable->GetRb()->GetPos().ToDxLibVector());
}

