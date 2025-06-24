#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN3 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN3>
{
public:
    PlayerStateAttackN3(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN3();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
    
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //����
    std::shared_ptr<Collidable> m_rightSword;
    //�U��
    std::shared_ptr<MeleeAttack> m_attackN3;//�ʏ�3
    //��(�U��)�̓����蔻��쐬
    void CreateAttack();
    //��(�U��)�̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
    //�폜
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
    //�U�����̑O�i����
    void AttackMove();
};

