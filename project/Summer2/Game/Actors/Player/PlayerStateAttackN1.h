#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class AttackManager;
class Collidable;
class MeleeAttack;
class PlayerStateAttackN1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN1>
{
public:
    PlayerStateAttackN1(std::shared_ptr<Player>  player);
    ~PlayerStateAttackN1();
    void Init() override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //•Ší
    std::shared_ptr<Collidable> m_rightSword;
    //UŒ‚
    std::shared_ptr<MeleeAttack> m_attackN1;//’Êí1
private:
    //Œ•(UŒ‚)‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //Œ•(UŒ‚)‚ÌˆÊ’uXV
    void UpdateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
    //UŒ‚íœ
    void DeleteAttack(const std::shared_ptr<AttackManager>& attackManager);
    //UŒ‚‚Ì‘Oiˆ—
    void AttackMove();
};

