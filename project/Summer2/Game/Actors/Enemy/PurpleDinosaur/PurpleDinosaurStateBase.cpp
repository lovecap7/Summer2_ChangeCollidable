#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaur.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::weak_ptr<PurpleDinosaur> owner):
	m_owner(owner),
	m_nextState(nullptr)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}


void PurpleDinosaurStateBase::ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState)
{
	//èÛë‘ïœâª
	m_nextState = move(nextState);
}