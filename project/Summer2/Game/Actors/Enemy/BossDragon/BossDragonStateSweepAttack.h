#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class BossDragon;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //���r
    std::shared_ptr<Collidable> m_leftArm;
    //�U��
    std::shared_ptr<MeleeAttack> m_attack;
    //�U���̓����蔻��쐬
    void CreateAttack();
    //�U���̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
    //�폜
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};

