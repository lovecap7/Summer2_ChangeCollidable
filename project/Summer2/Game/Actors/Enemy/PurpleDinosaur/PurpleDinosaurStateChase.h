#pragma once
#include "PurpleDinosaurStateBase.h"
class Input;
class Camera;
class ActorManager;
class PurpleDinosaur;
class PurpleDinosaurStateChase :
    public PurpleDinosaurStateBase, public std::enable_shared_from_this<PurpleDinosaurStateChase>
{
public:
    PurpleDinosaurStateChase(std::weak_ptr<PurpleDinosaur> owner);
    ~PurpleDinosaurStateChase();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
  
};

