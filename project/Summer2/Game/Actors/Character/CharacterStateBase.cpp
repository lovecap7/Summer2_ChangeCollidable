#include "CharacterStateBase.h"

CharacterStateBase::CharacterStateBase(std::weak_ptr<Actor> owner):
	m_owner(owner)
{
}

CharacterStateBase::~CharacterStateBase()
{
}

void CharacterStateBase::ChangeState(std::shared_ptr<CharacterStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}
