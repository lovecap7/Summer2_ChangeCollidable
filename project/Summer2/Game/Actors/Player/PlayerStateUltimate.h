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
    //�A�j���[�V�����̃t���[��
    int m_animCountFrame;
    //�A�j���[�V�����̑��x
    float m_animSpeed;
    //�������Ă���
    void SpeedDown();
};


