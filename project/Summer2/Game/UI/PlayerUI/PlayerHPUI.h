#pragma once
#include "PlayerUIBase.h"
#include <memory>
class Player;
class PlayerHPUI :
    public PlayerUIBase
{
public:
    PlayerHPUI(std::weak_ptr<Player> player);
    ~PlayerHPUI();
    void Init()override;
    void Update()override;
    void Draw() const override;
    void End()override;
private:
    float m_viewHp;
    float m_viewMaxHp;
};

