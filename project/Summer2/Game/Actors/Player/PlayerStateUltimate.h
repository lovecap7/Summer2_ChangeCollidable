#pragma once
#include "PlayerStateBase.h"
class Player;
class Camera;
class PlayerStateUltimate :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateUltimate>
{
public:
    PlayerStateUltimate(std::weak_ptr<Player> player);
    ~PlayerStateUltimate();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //アニメーションのフレーム
    int m_animCountFrame;
    //アニメーションの速度
    float m_animSpeed;
    //減速していく
    void SpeedDown();
};


