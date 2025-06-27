#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class Camera;
class ActorManager;
class PurpleDinosaurStateBase abstract :
	public CharacterStateBase
{
public:
	PurpleDinosaurStateBase(std::weak_ptr<Actor> owner);
	virtual ~PurpleDinosaurStateBase();
};

