#include "EnemyBase.h"

#include "../../../General/Model.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Rigidbody.h"

EnemyBase::EnemyBase(Shape shape) :
	Actor(shape),
	m_model(nullptr)
{
}
