#include "EnemyUIBase.h"

EnemyUIBase::EnemyUIBase(std::weak_ptr<EnemyBase> enemy):
	m_enemy(enemy)
{
}

EnemyUIBase::~EnemyUIBase()
{
}
