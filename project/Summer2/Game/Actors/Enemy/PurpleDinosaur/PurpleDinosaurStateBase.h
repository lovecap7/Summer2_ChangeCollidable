#pragma once
#include "../../ActorStateBase.h"
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class Actor;
class Camera;
class ActorManager;
class PurpleDinosaurStateBase abstract :
	public ActorStateBase
{
public:
	PurpleDinosaurStateBase(std::weak_ptr<Actor> owner);
	virtual ~PurpleDinosaurStateBase();
};

