#pragma once
#include "../ActorStateBase.h"
#include <memory>
#include "../../../General/Math/MyMath.h"
class Actor;
class Input;
class Camera;
class ActorManager;
class PlayerStateBase abstract:
	public ActorStateBase
{
protected:
	//is•ûŒü‚ğ•Ô‚·ŠÖ”
	virtual Vector3 GetForwardVec(const std::weak_ptr<Camera> camera);
public:
	PlayerStateBase(std::weak_ptr<Actor> player);
	virtual ~PlayerStateBase();
};

