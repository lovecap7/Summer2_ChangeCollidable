#pragma once
#include "SmallDragonStateBase.h"
class SmallDragon;
class Input;
class Camera;
class AttackManager;
class ActorManager;
class SmallDragonStateHit :
    public SmallDragonStateBase, public std::enable_shared_from_this<SmallDragonStateHit>
{
public:
    SmallDragonStateHit(std::shared_ptr<SmallDragon> owner);
    ~SmallDragonStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    void SpeedDown();
};

