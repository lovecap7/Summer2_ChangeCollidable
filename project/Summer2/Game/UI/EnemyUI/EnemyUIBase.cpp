#include "EnemyUIBase.h"

EnemyUIBase::EnemyUIBase(UIData uiData, int handle, std::weak_ptr<EnemyBase> enemy):
	UIBase(uiData,handle),
	m_enemy(enemy)
{
}

EnemyUIBase::~EnemyUIBase()
{
}
