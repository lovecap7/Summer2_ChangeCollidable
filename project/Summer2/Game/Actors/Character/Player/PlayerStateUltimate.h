#pragma once
#include "PlayerStateBase.h"
#include "../../../../General/Battle.h"
class Actor;
class AreaOfEffectAttack;
class Camera;
class ActorManager;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Actor> player);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //アニメーションのフレーム
    int m_animCountFrame;
    //アニメーションの速度
    float m_animSpeed;
    //攻撃の参照
    std::weak_ptr<AreaOfEffectAttack> m_attack;
    //攻撃の作成
    void CreateAttack(const std::weak_ptr<ActorManager> actorManager);
    //剣による攻撃の位置を更新する
    void UpdateAttackPos();
};


