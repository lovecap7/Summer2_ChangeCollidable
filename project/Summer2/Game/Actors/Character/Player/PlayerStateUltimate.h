#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
#include "../../../../General/CSVDataLoader.h"
class Actor;
class Slash;
class Camera;
class ActorManager;
class MyEffect;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Actor> player, const std::weak_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //アニメーションのフレーム
    int m_animCountFrame;
    //アニメーションの速度
    float m_animSpeed;
    //攻撃の参照
    std::weak_ptr<Slash> m_attack;
    //攻撃の作成
    void CreateAttack(float radius, int damage, int keepFrame, float knockBackPower, Battle::AttackWeight aw, const std::weak_ptr<ActorManager> actorManager);
    //攻撃データ
    AttackData m_attackData;
};


