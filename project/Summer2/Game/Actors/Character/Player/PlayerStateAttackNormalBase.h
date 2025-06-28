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
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃の参照
    std::weak_ptr<Slash> m_attack;
    //攻撃の作成
    void CreateAttack(float radius,int damage,int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //剣による攻撃の位置を更新する
    void UpdateAttackPos();
    //前進
    void AttackMove(float speed);
};


