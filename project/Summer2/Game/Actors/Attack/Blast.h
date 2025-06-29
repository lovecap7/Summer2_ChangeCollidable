#pragma once
#include "SphereAttackBase.h"
//�͈͍U��
class Actor;
class Blast :
    public SphereAttackBase
{
public:
    Blast(std::weak_ptr<Actor> owner);
    ~Blast();
    //�X�V����
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
    //�`��
    void Draw()const override;
    //�m�b�N�o�b�N
    Vector3 GetKnockBackVec(Position3 other) override;
};
