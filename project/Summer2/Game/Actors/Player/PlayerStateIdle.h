#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateIdle :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateIdle>
{
public:
    PlayerStateIdle(std::weak_ptr<Player>  player);
    ~PlayerStateIdle();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

