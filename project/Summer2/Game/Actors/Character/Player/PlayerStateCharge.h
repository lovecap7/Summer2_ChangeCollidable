#pragma once
#include "PlayerStateBase.h"
class Actor;
class Camera;
class TrackActorEffect;
class PlayerStateCharge :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateCharge>
{
public:
    PlayerStateCharge(std::weak_ptr<Actor> player);
    ~PlayerStateCharge();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
private:
    //�^������
    int m_chargeFrame;
    //�G�t�F�N�g
    std::weak_ptr<TrackActorEffect> m_eff;
};



