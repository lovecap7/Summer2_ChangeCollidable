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
    PlayerStateRun(std::weak_ptr<Player>  player);
    ~PlayerStateRun();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
  
};

