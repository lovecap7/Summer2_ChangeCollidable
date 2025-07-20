#include "EnemyUIBase.h"

EnemyUIBase::EnemyUIBase(int handle, std::weak_ptr<EnemyBase> enemy):
	UIBase(handle),
	m_enemy(enemy)
{
}

EnemyUIBase::~EnemyUIBase()
{
}
