#pragma once
#include "PlayerStateBase.h"
class Actor;
class Input;
class Camera;
class ActorManager;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::weak_ptr<Actor>  player);
    ~PlayerStateWalk();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //入力の大きさに合わせて速度を返す
    float InputValueSpeed(const Input& input);
};

