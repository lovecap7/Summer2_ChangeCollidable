#pragma once
#include "CapsuleAttackBase.h"
class Strike :
    public CapsuleAttackBase
{
public:
    Strike(std::weak_ptr<Actor> owner);
    ~Strike();
    //XVˆ—
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //•`‰æ
    void Draw()const override;
    //“–‚½‚Á‚½
    void OnCollide(const std::shared_ptr<Collidable> other) override;
};

