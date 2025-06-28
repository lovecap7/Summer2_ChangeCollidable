#pragma once
#include "CapsuleAttackBase.h"
class Slash :
    public CapsuleAttackBase
{
public:
    Slash(std::weak_ptr<Actor> owner);
    ~Slash();
    //XVˆ—
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //•`‰æ
    void Draw()const override;
};

