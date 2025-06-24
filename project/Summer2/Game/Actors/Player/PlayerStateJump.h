#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateJump :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateJump>
{
public:
    PlayerStateJump(std::shared_ptr<Player>  player);
    ~PlayerStateJump();
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
private:
    //“ü—Í‚É‘Î‰ž‚µ‚½ˆÚ“®—Ê
    float InputValueSpeed(const Input& input);
};

