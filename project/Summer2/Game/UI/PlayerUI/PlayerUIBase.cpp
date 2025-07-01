#include "PlayerUIBase.h"

PlayerUIBase::PlayerUIBase(std::weak_ptr<Player> player):
	m_player(player)
{
}

PlayerUIBase::~PlayerUIBase()
{
}
