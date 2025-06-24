#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class Collidable;
class MeleeAttack;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::shared_ptr<Player> player, const std::shared_ptr<ActorManager> actorManager);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //アニメーションのフレーム
    int m_animCountFrame;
    //アニメーションの速度
    float m_animSpeed;
    //攻撃
    std::shared_ptr<MeleeAttack> m_attackUlt;//必殺技
    //剣(攻撃)の当たり判定作成
    void CreateAttack();
    //剣(攻撃)の位置更新
    void UpdateAttack();
    //減速していく
    void SpeedDown();
    //削除
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};


