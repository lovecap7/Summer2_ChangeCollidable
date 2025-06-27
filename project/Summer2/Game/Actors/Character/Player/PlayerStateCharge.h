#pragma once
#include "PlayerStateBase.h"
class Actor;
class Camera;
class PlayerStateCharge :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCharge>
{
public:
    PlayerStateCharge(std::weak_ptr<Actor> player);
    ~PlayerStateCharge();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //ƒ^ƒŠÔ
    int m_chargeFrame;
    
};



