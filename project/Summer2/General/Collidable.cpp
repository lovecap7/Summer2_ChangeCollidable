#include "Collidable.h"

Collidable::Collidable(std::shared_ptr<ColliderBase> coll, std::shared_ptr<Rigidbody> rid):
	m_coll(coll),
	m_rb(rid),
	m_isCollide(true),
	m_state(State::None),
	m_owner(nullptr),
	m_priority(Priority::Middle),
	m_tag(GameTag::None)
{
}

//‰Šú‰»
void Collidable::Init(State state, Priority priority, GameTag gameTag)
{
	SetState(state);
	SetPriority(priority);
	SetGameTag(gameTag);
}
