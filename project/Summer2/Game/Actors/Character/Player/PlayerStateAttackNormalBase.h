#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include <memory>
class Slash;
class ActorManager;
class PlayerStateAttackNormalBase abstract :
    public PlayerStateBase
{
public:
    PlayerStateAttackNormalBase(std::weak_ptr<Actor>  player);
    virtual ~PlayerStateAttackNormalBase();
protected:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //�U���̎Q��
    std::weak_ptr<Slash> m_attack;
    //�U���̍쐬
    void CreateAttack(float radius,int damage,int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //���ɂ��U���̈ʒu���X�V����
    void UpdateAttackPos();
    //�O�i
    void AttackMove(float speed);
};


