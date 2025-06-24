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
    PlayerStateDeath(std::shared_ptr<Player>  player);
    ~PlayerStateDeath();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

