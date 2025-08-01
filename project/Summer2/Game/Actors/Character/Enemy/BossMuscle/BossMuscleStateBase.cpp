#include "BossMuscleStateBase.h"

BossMuscleStateBase::BossMuscleStateBase(std::weak_ptr<Actor> owner, bool isAngry):
	CharacterStateBase(owner),
	m_isAngry(isAngry)
{
}

BossMuscleStateBase::~BossMuscleStateBase()
{
}
