#include "PlayerStateCharge.h"
#include "PlayerStateRolling.h"
#include "PlayerStateCA1.h"
#include "PlayerStateCA2.h"
#include "PlayerStateCA3.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "PlayerStateUltimate.h"
#include "Player.h"
#include "UltGage.h"
#include "../../../General/game.h"
#include "../../../General/Collision/ColliderBase.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"

namespace
{
	//チャージレベル
	constexpr float kChargeLevel1 = 30.0f;
	constexpr float kChargeLevel2 = 60.0f;
	constexpr float kChargeLevel3 = 100.0f;
	//アニメーション
	const char* kAnim = "Player|Charge";//チャージ
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
}

PlayerStateCharge::PlayerStateCharge(std::weak_ptr<Player> player) :
	PlayerStateBase(player),
	m_chargeFrame(0)
{
	auto coll = m_player.lock();
	//回避状態
	coll->SetCollState(CollisionState::Normal);
	//チャージ
	coll->GetModel()->SetAnim(kAnim, true);
}

PlayerStateCharge::~PlayerStateCharge()
{
}
void PlayerStateCharge::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateCharge::Update(const std::weak_ptr<Camera> camera)
{
	auto& input = Input::GetInstance();
	auto collidable = m_player.lock();
	//回避ボタンを押したら
	if (input.IsTrigger("RB"))
	{
		//回避
		ChangeState(std::make_shared<PlayerStateRolling>(m_player));
		return;
	}
	//少しずつ減速する
	SpeedDown();
	//向きの更新
	Vector2 dir = collidable->GetStickVec();
	collidable->GetModel()->SetDir(dir);
	//溜めてる時
	if (input.IsPress("Y"))
	{
		//タメ攻撃チャージ
		++m_chargeFrame;
		//最大
		if (m_chargeFrame > kChargeLevel3)
		{
			m_chargeFrame = kChargeLevel3;
		}
	}
	//ボタンを離す
	else
	{
		if (m_chargeFrame <= kChargeLevel1)
		{
			//CA1
			ChangeState(std::make_shared<PlayerStateCA1>(m_player));
			return;
		}
		else if (m_chargeFrame <= kChargeLevel2)
		{
			//CA2
			ChangeState(std::make_shared<PlayerStateCA2>(m_player));
			return;
		}
		else if (m_chargeFrame <= kChargeLevel3)
		{
			//CA3
			ChangeState(std::make_shared<PlayerStateCA3>(m_player));
			return;
		}
	}
}
void PlayerStateCharge::SpeedDown()
{
	auto collidable = m_player.lock();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kMoveDeceRate;
	vec.z *= kMoveDeceRate;
	collidable->GetRb()->SetVec(vec);
}