#pragma once
#include "PurpleDinosaurStateBase.h"

class Camera;
class Actor;
class ActorManager;
class Collidable;
class PurpleDinosaurStateAttack :
	public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateAttack>
{
public:
	PurpleDinosaurStateAttack(std::weak_ptr<Actor> owner);
	~PurpleDinosaurStateAttack();
	void Init()override;
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
};

