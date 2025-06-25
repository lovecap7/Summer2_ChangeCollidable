#include "Actor.h"
#include "Enemy/EnemyBase.h"

Actor::Actor(Shape shape):
	Collidable(shape),
	m_hurtPoint(nullptr),
	m_isDelete(false),
	m_id(0),
	m_isSetId(false),
	m_isDrawOnly(false)
{
}

void Actor::SetID(int id)
{
	//‚·‚Å‚É“o˜^‚µ‚Ä‚é‚È‚ç
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}
