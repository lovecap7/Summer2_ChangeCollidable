#include "PlayerUIBase.h"

PlayerUIBase::PlayerUIBase(std::weak_ptr<Player> player):
	UIBase(),
	m_player(player)
{
}

PlayerUIBase::~PlayerUIBase()
{
}
