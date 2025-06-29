#pragma once
#include "BomberStateBase.h"

class Camera;
class ActorManager;
class Actor;
class BomberStateChase :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateChase>
{
public:
    BomberStateChase(std::weak_ptr<Actor> owner);
    ~BomberStateChase();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

