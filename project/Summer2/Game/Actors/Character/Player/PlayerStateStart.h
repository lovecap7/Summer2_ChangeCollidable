#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class Camera;
class ActorManager;
class PlayerStateStart :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateStart>
{
public:
    PlayerStateStart(std::weak_ptr<Actor>  player);
    ~PlayerStateStart();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:

};

