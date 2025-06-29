#pragma once
#include "BomberStateBase.h"
#include "../../../../../General/Battle.h"

class Camera;
class Actor;
class ActorManager;
class BomberStateAttack :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateAttack>
{
public:
    BomberStateAttack(std::weak_ptr<Actor> owner);
    ~BomberStateAttack();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //���e�̐���
    void CreateBomb(const std::weak_ptr<ActorManager> actorManager);
};