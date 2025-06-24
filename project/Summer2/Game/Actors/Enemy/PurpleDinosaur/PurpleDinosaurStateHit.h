#pragma once
#include "PurpleDinosaurStateBase.h"
class PurpleDinosaur;
class Input;
class Camera;
class ActorManager;
class PurpleDinosaurStateHit :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateHit>
{
public:
    PurpleDinosaurStateHit(std::shared_ptr<PurpleDinosaur> owner);
    ~PurpleDinosaurStateHit();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    void SpeedDown();
};
