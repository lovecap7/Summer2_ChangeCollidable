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
    PlayerStateJump(std::weak_ptr<Player>  player);
    ~PlayerStateJump();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //“ü—Í‚É‘Î‰ž‚µ‚½ˆÚ“®—Ê
    float InputValueSpeed(const Input& input);
};

