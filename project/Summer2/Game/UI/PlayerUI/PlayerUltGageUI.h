#pragma once
#include "PlayerUIBase.h"
#include <memory>
class Player;
class PlayerUltGageUI :
	public PlayerUIBase
{
public:
	PlayerUltGageUI(std::weak_ptr<Player> player);
	~PlayerUltGageUI();
	void Update()override;
	void Draw()const override;
private:
	//ÉQÅ[ÉW
	float m_viewUltGageValue;
	float m_viewMaxUltGageValue;
};
