#pragma once
#include "CapsuleAttackBase.h"
class Strike :
    public CapsuleAttackBase
{
public:
    Strike(std::weak_ptr<Actor> owner);
    ~Strike();
    //�`��
    void Draw()const override;
};

