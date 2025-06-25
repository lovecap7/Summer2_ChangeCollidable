#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class ActorManager;
class PlayerStateFall :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateFall>
{
public:
    PlayerStateFall(std::weak_ptr<Player>  player);
    ~PlayerStateFall();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //入力でスピードを返す
    float InputValueSpeed(const Input& input);
};

