#pragma once
#include "SphereAttackBase.h"
//”ÍˆÍUŒ‚
class Actor;
class AreaOfEffectAttack :
    public SphereAttackBase
{
public:
    AreaOfEffectAttack(std::weak_ptr<Actor> owner);
    ~AreaOfEffectAttack();
    //•`‰æ
    void Draw()const override;
};

