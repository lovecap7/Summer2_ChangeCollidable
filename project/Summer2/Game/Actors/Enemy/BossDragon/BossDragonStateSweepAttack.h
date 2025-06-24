#pragma once
#include "BossDragonStateBase.h"
class Input;
class Camera;
class AttackManager;
class ActorManager;
class Collidable;
class MeleeAttack;
class BossDragon;
class BossDragonStateSweepAttack :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateSweepAttack>
{
public:
    BossDragonStateSweepAttack(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateSweepAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //¶˜r
    std::shared_ptr<Collidable> m_leftArm;
    //UŒ‚
    std::shared_ptr<MeleeAttack> m_attack;
    //UŒ‚‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //UŒ‚‚ÌˆÊ’uXV
    void UpdateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
    //íœ
    void DeleteAttack(const std::shared_ptr<AttackManager> attackManager);
};

