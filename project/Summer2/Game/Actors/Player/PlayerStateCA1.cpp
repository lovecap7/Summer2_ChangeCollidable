#include "PlayerStateCA1.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//チャージ攻撃の段階別ダメージ
	constexpr int kCA1AnimDamage = 100.0f;
	//チャージ攻撃の持続
	constexpr float kCA1KeepFrame = 30.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 2.0f;
	//アニメーション
	const char* kAnim = "Player|CA";//チャージ攻撃
	//チャージ攻撃の段階別アニメーションの速度
	constexpr float kCA1AnimSpeed = 1.5f;
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//左足の半径
	constexpr float kLeftLegRadius = 20.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 1;
}

PlayerStateCA1::PlayerStateCA1(std::weak_ptr<Player> player) :
	PlayerStateBase(player)
{
	auto coll = m_player.lock();
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, kCA1AnimSpeed);
	model->SetFixedLoopFrame(kCA1KeepFrame);//指定ループ
}

PlayerStateCA1::~PlayerStateCA1()
{
}
void PlayerStateCA1::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA1::Update(const std::weak_ptr<Camera> camera)
{

	auto model = m_player.lock()->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (model->IsFinishAnim())
	{
		
	}
	//少しずつ減速する
	SpeedDown();

}

void PlayerStateCA1::SpeedDown()
{
	auto collidable = m_player.lock();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}
