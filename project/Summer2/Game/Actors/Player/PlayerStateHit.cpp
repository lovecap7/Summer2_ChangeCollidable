#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "PlayerStateDeath.h"
#include "Player.h"
#include "../../../General/game.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Input.h"
#include "../../../General/Model.h"
#include "../../../General/Animator.h"
#include "../../../Game/Camera/Camera.h"
#include "../../Attack/AttackManager.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/MeleeAttack.h"
#include "../../Attack/HurtPoint.h"
#include <DxLib.h>
namespace
{
	//アニメーション
	const char* kHit1Anim = "Player|Hit1";
	const char* kHit2Anim = "Player|Hit2";
	//アニメーション速度
	constexpr float kHit1AnimSpeed = 1.1f;
	constexpr float kHit2AnimSpeed = 1.1f;
	//減速率
	constexpr float kDeceRate = 0.95f;
}


PlayerStateHit::PlayerStateHit(std::shared_ptr<Player> player):
	PlayerStateBase(player)
{
	//ランダムにヒットアニメーションを選ぶ
	RandHitAnim();
	m_player->GetCollidable()->SetState(State::None);
}

PlayerStateHit::~PlayerStateHit()
{
}

void PlayerStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateHit::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//死亡
	if (m_player->GetHurtPoint()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_player));
		return;
	}
	//やられリアクション中に攻撃を食らったらアニメーションを初めから
	if (m_player->GetHurtPoint()->IsHitReaction())
	{
		//ランダムでヒットアニメーションを選ぶ
		RandHitAnim();
		//リプレイ
		m_player->GetModel()->ReplayAnim();
	}

	//モデルのアニメーションが終わったら
	if (m_player->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_player));
		return;
	}
	//だんだん減速
	SpeedDown();
}

void PlayerStateHit::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}

void PlayerStateHit::RandHitAnim()
{
	//ランダムでヒットアニメーションを選ぶ
	if (GetRand(1))//0か1でランダム
	{
		m_player->GetModel()->SetAnim(kHit1Anim, false, kHit1AnimSpeed);
	}
	else
	{
		m_player->GetModel()->SetAnim(kHit2Anim, false, kHit2AnimSpeed);
	}
}
