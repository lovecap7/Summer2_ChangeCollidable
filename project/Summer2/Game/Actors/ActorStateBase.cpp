#include "ActorStateBase.h"

ActorStateBase::ActorStateBase()
{
}

ActorStateBase::~ActorStateBase()
{
}

void ActorStateBase::ChangeState(std::shared_ptr<ActorStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}
