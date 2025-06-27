#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class ActorManager;
class Actor;
class PurpleDinosaurStateDeath :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateDeath>
{
public:
	PurpleDinosaurStateDeath(std::weak_ptr<Actor> owner);
	~PurpleDinosaurStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

