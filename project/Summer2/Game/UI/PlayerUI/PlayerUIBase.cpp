#include "PlayerUIBase.h"

PlayerUIBase::PlayerUIBase(UIData uiData, int handle, std::weak_ptr<Player> player):
	UIBase(uiData,handle),
	m_player(player)
{
}

PlayerUIBase::~PlayerUIBase()
{
}
