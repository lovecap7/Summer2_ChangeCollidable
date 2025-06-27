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
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<Slash> m_attack;
    //UŒ‚‚Ìì¬
    void CreateAttack(float radius,int damage,int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //Œ•‚É‚æ‚éUŒ‚‚ÌˆÊ’u‚ğXV‚·‚é
    void UpdateSlashAttackPos(std::weak_ptr<Slash> attack);
};


