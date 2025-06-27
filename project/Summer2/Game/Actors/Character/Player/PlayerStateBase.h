#pragma once
#include "../CharacterStateBase.h"
#include <memory>
#include "../../../../General/Math/MyMath.h"
class Actor;
class Slash;
class Camera;
class ActorManager;
class PlayerStateBase abstract:
	public CharacterStateBase
{
public:
	PlayerStateBase(std::weak_ptr<Actor> player);
	virtual ~PlayerStateBase();
protected:
	//is•ûŒü‚ğ•Ô‚·ŠÖ”
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
};

