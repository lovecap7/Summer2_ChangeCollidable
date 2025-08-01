#include "BossMuscleStateBase.h"

BossMuscleStateBase::BossMuscleStateBase(std::weak_ptr<Actor> owner) :
	CharacterStateBase(owner)
{
}

BossMuscleStateBase::~BossMuscleStateBase()
{
}
