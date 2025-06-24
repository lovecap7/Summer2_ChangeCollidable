#pragma once
#include "BomberStateBase.h"

class Input;
class Camera;
class AttackManager;
class ActorManager;
class PurpleDinosaur;
class BomberStateChase :
    public BomberStateBase, public std::enable_shared_from_this<BomberStateChase>
{
public:
    BomberStateChase(std::shared_ptr<Bomber> owner);
    ~BomberStateChase();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:

};

