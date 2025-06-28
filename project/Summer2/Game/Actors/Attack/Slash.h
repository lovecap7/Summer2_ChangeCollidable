#pragma once
#include "CapsuleAttackBase.h"
class Slash :
    public CapsuleAttackBase
{
public:
    Slash(std::weak_ptr<Actor> owner);
    ~Slash();
    //�X�V����
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //�`��
    void Draw()const override;
};

