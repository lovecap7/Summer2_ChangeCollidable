#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"

EnemyBase::EnemyBase(Shape shape, EnemyGrade grade) :
	CharacterBase(shape),
	m_attackCoolTime(0),
	m_hpUIPos{},
	m_enemyGrade(grade)
{
}

void EnemyBase::UpdateAttackCoolTime()
{
	m_attackCoolTime--;
	if (m_attackCoolTime < 0)
	{
		m_attackCoolTime = 0;
	}
}