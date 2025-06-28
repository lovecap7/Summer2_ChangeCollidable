#include "BossDragonStateBase.h"
#include "BossDragon.h"

BossDragonStateBase::BossDragonStateBase(std::weak_ptr<Actor> owner):
	CharacterStateBase(owner)
{
}

BossDragonStateBase::~BossDragonStateBase()
{
}
