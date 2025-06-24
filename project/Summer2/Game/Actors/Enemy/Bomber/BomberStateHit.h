#pragma once
#include "BomberStateBase.h"

class Bomber;
class Input;
class Camera;
class ActorManager;
class BomberStateHit :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateHit>
{
public:
    BomberStateHit(std::shared_ptr<Bomber> owner);
    ~BomberStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    void SpeedDown();
};


