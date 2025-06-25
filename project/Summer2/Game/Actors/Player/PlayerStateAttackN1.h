#pragma once
#include "PlayerStateBase.h"
class Player;
class Input;
class Camera;
class PlayerStateAttackN1 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN1>
{
public:
    PlayerStateAttackN1(std::weak_ptr<Player>  player);
    ~PlayerStateAttackN1();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //�U���̃t���[���𐔂���
    int m_attackCountFrame;
private:
    //�������Ă���
    void SpeedDown();
};

