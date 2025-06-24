#pragma once
#include "PlayerStateBase.h""
class Player;
class Input;
class Camera;
class AttackManager;
class PlayerStateRun :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateRun>
{
public:
    PlayerStateRun(std::shared_ptr<Player>  player);
    ~PlayerStateRun();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
  
};

