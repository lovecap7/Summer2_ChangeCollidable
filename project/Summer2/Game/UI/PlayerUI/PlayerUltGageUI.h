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
	void Init() override;
	void Update(const std::weak_ptr<ActorManager> actorManager)override;
	void Draw()const override;
	void End()override;
private:
	//ÉQÅ[ÉW
	float m_viewUltGageValue;
	float m_viewMaxUltGageValue;
};
