#pragma once
#include "PlayerStateAttackNormalBase.h"
class Actor;
class Camera;
class PlayerStateAttackN2 :
    public PlayerStateAttackNormalBase, public std::enable_shared_from_this<PlayerStateAttackN2>
{
public:
    PlayerStateAttackN2(std::weak_ptr<Actor>  player);
    ~PlayerStateAttackN2();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
  
};

