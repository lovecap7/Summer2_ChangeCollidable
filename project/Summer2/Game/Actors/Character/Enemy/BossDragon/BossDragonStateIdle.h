#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class ActorManager;
class Actor;
class BossDragonStateIdle :
	public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateIdle>
{
public:
	BossDragonStateIdle(std::weak_ptr<Actor> owner);
	~BossDragonStateIdle();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
	//UŒ‚‚ğ‘I‚Ô
	void ThinkAttack(const std::weak_ptr<ActorManager> actorManager);
};