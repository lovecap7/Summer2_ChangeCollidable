#include "PlayerUIBase.h"

PlayerUIBase::PlayerUIBase(int handle, std::weak_ptr<Player> player):
	UIBase(handle),
	m_player(player)
{
}

PlayerUIBase::~PlayerUIBase()
{
}
