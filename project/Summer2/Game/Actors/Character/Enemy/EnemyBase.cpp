#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"

EnemyBase::EnemyBase(Shape shape) :
	CharacterBase(shape),
	m_attackCoolTime(0)
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