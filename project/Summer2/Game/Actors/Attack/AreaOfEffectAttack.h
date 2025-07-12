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
    //�X�V����
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //�`��
    void Draw()const override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
};

