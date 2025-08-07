#include "BossKingStateBase.h"

BossKingStateBase::BossKingStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

BossKingStateBase::~BossKingStateBase()
{
}
