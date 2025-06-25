#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::weak_ptr<Player>  player);
    ~PlayerStateWalk();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //“ü—Í‚Ì‘å‚«‚³‚É‡‚í‚¹‚Ä‘¬“x‚ğ•Ô‚·
    float InputValueSpeed(const Input& input);
};

