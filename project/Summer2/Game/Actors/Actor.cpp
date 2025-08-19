#include "Actor.h"
#include "../../General/Rigidbody.h"
#include "Character/Enemy/EnemyBase.h"

Actor::Actor(Shape shape):
	Collidable(shape),
	m_isDelete(false),
	m_id(0),
	m_isSetId(false),
	m_groupTag{ "Untagged" },
	m_canAttack(true),
	m_isInGroup(false)
{
}

void Actor::SetID(int id)
{
	//‚·‚Å‚É“o˜^‚µ‚Ä‚é‚È‚ç
	if (m_isSetId)return;
	m_id = id;
	m_isSetId = true;
}

Vector3 Actor::GetPos() const
{
	return m_rb->m_pos;
}

Vector3 Actor::GetNextPos() const
{
	return m_rb->GetNextPos();
}

void Actor::SetGroupTag(std::string& tag)
{
	m_groupTag = tag;
	//ƒOƒ‹[ƒv‚ÉŠ‘®‚µ‚Ä‚¢‚é‚©
	if (m_groupTag != "Untagged")
	{
		m_isInGroup = true;
	}
}
