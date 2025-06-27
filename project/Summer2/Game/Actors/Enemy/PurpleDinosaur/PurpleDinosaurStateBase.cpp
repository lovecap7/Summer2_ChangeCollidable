#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaur.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::weak_ptr<Actor> owner):
	ActorStateBase(owner)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}

