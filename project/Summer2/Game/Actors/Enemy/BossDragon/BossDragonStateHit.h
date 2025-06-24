#pragma once
#include "BossDragonStateBase.h"
class BossDragon;
class Input;
class Camera;
class ActorManager;
class BossDragonStateHit :
    public BossDragonStateBase, public std::enable_shared_from_this<BossDragonStateHit>
{
public:
    BossDragonStateHit(std::shared_ptr<BossDragon> owner);
    ~BossDragonStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    void SpeedDown();
};

