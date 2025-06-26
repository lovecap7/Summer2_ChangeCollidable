#pragma once
#include "../../ActorStateBase.h"
#include <memory>
#include <vector>
#include "../../../../General/Math/Vector3.h"
class PurpleDinosaur;
class Camera;
class ActorManager;
class PurpleDinosaurStateBase abstract :
	public ActorStateBase
{
protected:
	//Ž©•ª‚Ìƒ|ƒCƒ“ƒ^
	std::weak_ptr<PurpleDinosaur> m_owner;
public:
	PurpleDinosaurStateBase(std::weak_ptr<PurpleDinosaur> owner);
	virtual ~PurpleDinosaurStateBase();
};

