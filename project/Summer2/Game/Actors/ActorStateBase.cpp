#include "ActorStateBase.h"

ActorStateBase::ActorStateBase(std::weak_ptr<Actor> owner):
	m_owner(owner)
{
}

ActorStateBase::~ActorStateBase()
{
}

void ActorStateBase::ChangeState(std::shared_ptr<ActorStateBase> nextState)
{
	//ó‘Ô•Ï‰»
	m_nextState = move(nextState);
}
