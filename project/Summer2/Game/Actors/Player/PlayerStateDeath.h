#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateDeath :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateDeath>
{
public:
    PlayerStateDeath(std::weak_ptr<Player>  player);
    ~PlayerStateDeath();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    
};

