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
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<Strike> m_attack;
    //UŒ‚‚Ìì¬
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //‘«‚É‚æ‚éUŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
    void UpdateStrikeAttackPos(std::weak_ptr<Strike> attack);
};

