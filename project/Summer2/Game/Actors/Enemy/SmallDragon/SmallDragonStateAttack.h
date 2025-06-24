#pragma once
#include "SmallDragonStateBase.h"
class Input;
class Camera;
class ActorManager;
class Collidable;
class BulletAttack;
class SmallDragonStateAttack :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateAttack>
{
public:
	SmallDragonStateAttack(std::shared_ptr<SmallDragon> owner);
	~SmallDragonStateAttack();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
    //UŒ‚
    std::shared_ptr<BulletAttack> m_attack;
    //UŒ‚‚Ì“–‚½‚è”»’èì¬
    void CreateAttack();
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};