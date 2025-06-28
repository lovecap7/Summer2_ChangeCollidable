#pragma once
#include "BossDragonStateBase.h"
#include "../../../../../General/Battle.h"
class Camera;
class Actor;
class ActorManager;
class Collidable;
class Strike;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::weak_ptr<Actor> owner);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃の参照
    std::weak_ptr<Strike> m_attack;
    //攻撃の作成
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //攻撃の位置を更新する
    void UpdateAttackPos();
    //攻撃時に前進する
    void AttackMove();
};


