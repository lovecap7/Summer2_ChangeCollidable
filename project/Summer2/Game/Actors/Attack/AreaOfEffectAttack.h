#pragma once
#include "SphereAttackBase.h"
//�͈͍U��
class Actor;
class AreaOfEffectAttack :
    public SphereAttackBase
{
public:
    AreaOfEffectAttack(std::weak_ptr<Actor> owner);
    ~AreaOfEffectAttack();
    //�`��
    void Draw()const override;
};

