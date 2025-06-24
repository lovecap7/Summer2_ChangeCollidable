#pragma once
#include "BomberStateBase.h"

class Input;
class Camera;
class ActorManager;
class Collidable;
class MeleeAttack;
class ItemGenerator;
class BomberStateAttack :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateAttack>
{
public:
    BomberStateAttack(std::shared_ptr<Bomber> owner);
    ~BomberStateAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //���e���쐬
    void CreateBomb(const std::shared_ptr<ItemGenerator> actorManager);
    //�������Ă���
    void SpeedDown();
};

