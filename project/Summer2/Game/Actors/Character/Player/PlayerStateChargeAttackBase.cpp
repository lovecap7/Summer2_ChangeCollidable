#include "PlayerStateChargeAttackBase.h"

namespace
{
	//左足の根本と足先のインデックス
	constexpr int kRootIndex = 64;
	constexpr int kToeIndex = 68;
}

PlayerStateChargeAttackBase::PlayerStateChargeAttackBase(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_attackCountFrame(0)
{
}

PlayerStateChargeAttackBase::~PlayerStateChargeAttackBase()
{
}

void PlayerStateChargeAttackBase::CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager)
{
}

void PlayerStateChargeAttackBase::UpdateStrikeAttackPos(std::weak_ptr<Slash> attack)
{
}
