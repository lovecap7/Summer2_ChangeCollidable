#include "BossKingStateStart.h"
#include "BossKingStateIdle.h"
#include "BossKing.h"
#include "../EnemyBase.h"
#include "../../../../../General/Collision/ColliderBase.h"
#include "../../../ActorManager.h"
#include "../../../../../General/Rigidbody.h"
#include "../../../../../General/Collision/Collidable.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Animator.h"
#include "../../../../../General/HitPoints.h"
#include "../../../../../Game/Camera/GameCamera/GameCamera.h"
namespace
{
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "Boss3|Start";
}

BossKingStateStart::BossKingStateStart(std::weak_ptr<Actor> player) :
	BossKingStateBase(player)
{
	//スタート状態
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	coll->GetModel()->SetAnim(kAnim, false);
	coll->SetCollState(CollisionState::Normal);
}

BossKingStateStart::~BossKingStateStart()
{
	
}

void BossKingStateStart::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void BossKingStateStart::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//入力を止める
	auto& input = Input::GetInstance();
	input.StopUpdate();
	auto coll = std::dynamic_pointer_cast<BossKing>(m_owner.lock());
	//モデルのアニメーションが終わったら
	if (coll->GetModel()->IsFinishAnim())
	{
		//入力を開始
		input.StartUpdate();
		//待機
		ChangeState(std::make_shared<BossKingStateIdle>(m_owner));
		return;
	}
	//だんだん減速
	coll->GetRb()->SpeedDown(kMoveDeceRate);
}
