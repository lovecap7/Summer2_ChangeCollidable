#include "PurpleDinosaurStateChase.h"
#include "PurpleDinosaurStateDeath.h"
#include "PurpleDinosaurStateIdle.h"
#include "PurpleDinosaur.h"
#include "../EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/ColliderBase.h"

#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"

#include "../../../../Game/Camera/Camera.h"
namespace
{
	//プレイヤー戦闘状態になる距離
	constexpr float kBattleDistance = 200.0f;
	//減速率
	constexpr float kMoveDeceRate = 0.8f;
	//アニメーション
	const char* kAnim = "CharacterArmature|Run";
	//プレイヤーを追いかける速度
	constexpr float kChaseSpeed = 5.0f;
}

PurpleDinosaurStateChase::PurpleDinosaurStateChase(std::weak_ptr<PurpleDinosaur> owner):
	PurpleDinosaurStateBase(owner)
{
	m_owner.lock()->SetCollState(CollisionState::Normal);
	//アニメーション
	m_owner.lock()->GetModel()->SetAnim(kAnim, true);
}

PurpleDinosaurStateChase::~PurpleDinosaurStateChase()
{
}

void PurpleDinosaurStateChase::Init()
{
	//次の状態を今の状態に更新
	ChangeState(shared_from_this());
}

void PurpleDinosaurStateChase::Update(const std::weak_ptr<Camera> camera)
{

}
