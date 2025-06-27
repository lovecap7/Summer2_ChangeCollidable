#include "PlayerStateCA2.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../Game/Camera/Camera.h"

namespace
{
	//チャージ攻撃の段階別ダメージ
	constexpr int kCA2AnimDamage = 150.0f;
	//チャージ攻撃の持続
	constexpr float kCA2KeepFrame = 60.0f;
	//ノックバックの大きさ
	constexpr float kKnockBackPower = 2.0f;
	//アニメーション
	const char* kAnim = "Player|CA";//チャージ攻撃
	//チャージ攻撃の段階別アニメーションの速度
	constexpr float kCA2AnimSpeed = 2.5f;
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
	//左足の半径
	constexpr float kLeftLegRadius = 20.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//加算ゲージ量
	constexpr int kAddUltGage = 2;
}

PlayerStateCA2::PlayerStateCA2(std::weak_ptr<Actor> player) :
	PlayerStateChargeAttackBase(player)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	coll->SetCollState(CollisionState::Normal);
	//チャージ攻撃1
	auto model = coll->GetModel();
	model->SetAnim(kAnim, true, kCA2AnimSpeed);
	model->SetFixedLoopFrame(kCA2KeepFrame);//指定ループ
}

PlayerStateCA2::~PlayerStateCA2()
{
}
void PlayerStateCA2::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCA2::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	auto coll = std::dynamic_pointer_cast<Player>(m_owner.lock());
	auto model = coll->GetModel();
	//アニメーションが終了したら
	if (model->IsFinishFixedLoop())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_owner));
		return;
	}
	//アニメーションが一周するたびに攻撃判定のリセット
	if (model->IsFinishAnim())
	{

	}
	//減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);

}

