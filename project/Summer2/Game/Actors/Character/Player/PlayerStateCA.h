#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Strike;
class TrackActorEffect;
class PlayerStateCA :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCA>
{
public:
    PlayerStateCA(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager,AttackData data);
    virtual ~PlayerStateCA();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
protected:
    //攻撃のフレームを数える
    int m_attackCountFrame;
    //攻撃の参照
    std::weak_ptr<Slash> m_attack;
    //攻撃の作成
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //攻撃の位置を更新する
    void UpdateAttackPos();
    //攻撃による前進
    void AttackMove(float speed);
    //攻撃データ
    AttackData m_attackData;
};

