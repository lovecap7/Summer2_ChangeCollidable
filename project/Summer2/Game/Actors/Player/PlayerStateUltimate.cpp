#include "PlayerStateUltimate.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//Ultのダメージと持続フレーム
	constexpr int kUltDamege = 30.0f;
	constexpr int kUltKeepFrame = 180;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 2.0f;
	//攻撃判定をリセットする頻度
	constexpr int kUltInitFrame = 10;
	//アニメーション
	const char* kAnim = "Player|Ult2";
	//アニメーションの速度の初期値
	constexpr float kAnimSpeed = 0.7f;
	//アニメーションの速度の変化量
	constexpr float kAddAnimSpeed = 0.2f;
	//アニメーションの最高速度
	constexpr float kMaxAnimSpeed = 2.0f;
	//武器の座標と当たり判定の情報
	constexpr float kAttackDistance = 200.0f;//プレイヤーの前方に攻撃判定を出す際の距離
	constexpr float kAttackRadius = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateUltimate::PlayerStateUltimate(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_animCountFrame(0),
	m_animSpeed(kAnimSpeed)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	//必殺技
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, m_animSpeed);
	model->SetFixedLoopFrame(kUltKeepFrame);//指定ループ
	//向きの更新
	Vector2 dir = coll->GetStickVec();
	model->SetDir(dir);
}


PlayerStateUltimate::~PlayerStateUltimate()
{
}

void PlayerStateUltimate::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateUltimate::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	++m_animCountFrame;
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//攻撃判定をリセット
	if (m_animCountFrame % kUltInitFrame == 0)
	{
	
	}
	//アニメーションが一周するたびに再生速度を上げる
	if (model->IsFinishAnim())
	{
		//アニメーションの速度を速くしていく
		if (m_animSpeed < kMaxAnimSpeed)
		{
			m_animSpeed += kAddAnimSpeed;
			model->SetAnimSpeed(m_animSpeed);
		}
	}
	//少しずつ減速する
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}