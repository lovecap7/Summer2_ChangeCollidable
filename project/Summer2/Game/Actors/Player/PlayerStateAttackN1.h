#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN1>
{
public:
    PlayerStateAttackN1(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN1();
    void Init() override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //����
    std::shared_ptr<Collidable> m_rightSword;
    //�U��
    std::shared_ptr<MeleeAttack> m_attackN1;//�ʏ�1
private:
    //��(�U��)�̓����蔻��쐬
    void CreateAttack();
    //��(�U��)�̈ʒu�X�V
    void UpdateAttack();
    //�������Ă���
    void SpeedDown();
    //�U���폜
    void DeleteAttack(const std::shared_ptr<AttackManager>& attackManager);
    //�U�����̑O�i����
    void AttackMove();
};

