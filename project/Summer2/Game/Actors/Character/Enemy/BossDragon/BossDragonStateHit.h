#pragma once
#include "BossDragonStateBase.h"
class Actor;
class Input;
class Camera;
class ActorManager;
class BossDragonStateHit :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateHit>
{
public:
    BossDragonStateHit(std::weak_ptr<Actor> owner);
    ~BossDragonStateHit();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};
