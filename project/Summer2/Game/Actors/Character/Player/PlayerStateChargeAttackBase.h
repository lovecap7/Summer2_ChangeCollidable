#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
class Actor;
class Strike;
class PlayerStateChargeAttackBase abstract :
    public PlayerStateBase
{
public:
    PlayerStateChargeAttackBase(std::weak_ptr<Actor> player);
    virtual ~PlayerStateChargeAttackBase();
protected:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
    //�U���̎Q��
    std::weak_ptr<Strike> m_attack;
    //�U���̍쐬
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //���ɂ��U���̈ʒu���X�V����
    void UpdateStrikeAttackPos(std::weak_ptr<Strike> attack);
};

