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
    PurpleDinosaurStateChase(std::shared_ptr<PurpleDinosaur> owner);
    ~PurpleDinosaurStateChase();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
  
};

