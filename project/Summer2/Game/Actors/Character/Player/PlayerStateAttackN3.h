#pragma once
#include "PlayerStateAttackNormalBase.h"
class Actor;
class Camera;
class ActorManager;
class PlayerStateAttackN3 :
    public PlayerStateAttackNormalBase, public std::enable_shared_from_this<PlayerStateAttackN3>
{
public:
    PlayerStateAttackN3(std::weak_ptr<Actor>  player);
    ~PlayerStateAttackN3();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
};

