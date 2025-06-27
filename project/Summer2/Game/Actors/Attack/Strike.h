#pragma once
#include "CapsuleAttackBase.h"
class Strike :
    public CapsuleAttackBase
{
public:
    Strike(std::weak_ptr<Actor> owner);
    ~Strike();
    //•`‰æ
    void Draw()const override;
};

