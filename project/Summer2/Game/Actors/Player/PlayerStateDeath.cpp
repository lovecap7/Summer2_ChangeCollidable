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
namespace
{
	//アニメーション
	const char* kAnim = "Player|Dead";
	//減速率
	constexpr float kDeceRate = 0.95f;
}

PlayerStateDeath::PlayerStateDeath(std::shared_ptr<Player> player) :
	PlayerStateBase(player)
{
	//待機状態
	m_player->GetModel()->SetAnim(kAnim, false);
	m_player->GetCollidable()->SetState(State::Dead);
}

PlayerStateDeath::~PlayerStateDeath()
{
}

void PlayerStateDeath::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateDeath::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//アニメーション終了後
	if (m_player->GetModel()->IsFinishAnim())
	{
		m_player->Delete();//削除
	}
	//減速
	SpeedDown();
}

void PlayerStateDeath::SpeedDown()
{
	auto collidable = m_player->GetCollidable();
	//減速
	Vector3 vec = collidable->GetRb()->GetVec();
	vec.x *= kDeceRate;
	vec.z *= kDeceRate;
	collidable->GetRb()->SetVec(vec);
}

