#pragma once
#include "BomberStateBase.h"
class Camera;
class ActorManager;
class Actor;
class BomberStateDeath :
	public BomberStateBase, public std::enable_shared_from_this<BomberStateDeath>
{
public:
	BomberStateDeath(std::weak_ptr<Actor> owner);
	~BomberStateDeath();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

