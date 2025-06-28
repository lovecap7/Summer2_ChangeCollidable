#pragma once
#include "../../CharacterStateBase.h"
#include <memory>
#include <vector>
#include "../../../../../General/Math/Vector3.h"
class Actor;
class Camera;
class ActorManager;
class BossDragonStateBase abstract :
	public CharacterStateBase
{
public:
	BossDragonStateBase(std::weak_ptr<Actor> owner);
	virtual ~BossDragonStateBase();
};

