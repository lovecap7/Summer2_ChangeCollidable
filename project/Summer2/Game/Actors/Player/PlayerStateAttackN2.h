#pragma once
#include "PlayerStateBase.h"
class Player;
class Camera;
class PlayerStateAttackN2 :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAttackN2>
{
public:
    PlayerStateAttackN2(std::weak_ptr<Player>  player);
    ~PlayerStateAttackN2();
    void Init() override;
    void Update(const std::weak_ptr<Camera> camera) override;
private:
    //UŒ‚‚ÌƒtƒŒ[ƒ€‚ğ”‚¦‚é
    int m_attackCountFrame;
private:
    //Œ¸‘¬‚µ‚Ä‚¢‚­
    void SpeedDown();
};

