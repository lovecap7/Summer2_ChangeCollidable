#pragma once
#include "PlayerStateBase.h""
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateRun :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateRun>
{
public:
    PlayerStateRun(std::weak_ptr<Player>  player);
    ~PlayerStateRun();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
  
};

