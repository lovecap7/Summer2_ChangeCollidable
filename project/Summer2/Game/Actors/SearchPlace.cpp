#include "SearchPlace.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"
#include "Actor.h"
#include <DxLib.h>

SearchPlace::SearchPlace(std::weak_ptr<Actor> actor, float rang) :
	m_target(actor),
	m_rang(rang)
{
	if (m_target.expired())return;
	m_pos = m_target.lock()->GetPos();
}

SearchPlace::~SearchPlace()
{
}

void SearchPlace::Draw() const
{
	DrawSphere3D(m_pos.ToDxLibVector(), m_rang, 16, 0xffffff, 0xffffff, false);
}

bool SearchPlace::IsInSearchPlace() const
{
	//‹ó‚È‚ç
	if (m_target.expired())return false;
	//”ÍˆÍ“à‚É‚¢‚é‚©
	float dist = Vector3(m_target.lock()->GetPos() - m_pos).Magnitude();
	if (dist <= m_rang)
	{
		return true;
	}
	//‚±‚±‚Ü‚Å—ˆ‚½‚ç
	return false;
}
	