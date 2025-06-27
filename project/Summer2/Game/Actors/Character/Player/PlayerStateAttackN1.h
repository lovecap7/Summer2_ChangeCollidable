#pragma once
#include "PlayerStateAttackNormalBase.h"
class Actor;
class Camera;
class Model;
class ActorManager;
class PlayerStateAttackN1 :
    public PlayerStateAttackNormalBase, public std::enable_shared_from_this<PlayerStateAttackN1>
{
public:
    PlayerStateAttackN1(std::weak_ptr<Actor>  player);
    ~PlayerStateAttackN1();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};

