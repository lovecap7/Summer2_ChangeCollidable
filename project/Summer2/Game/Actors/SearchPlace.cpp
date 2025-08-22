#include "SearchPlace.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Rigidbody.h"
#include "Actor.h"
#include <DxLib.h>

SearchPlace::SearchPlace(Vector3 pos, float rang) :
	m_pos(pos),
	m_rang(rang)
{
}

SearchPlace::~SearchPlace()
{
}

void SearchPlace::Draw() const
{
	DrawSphere3D(m_pos.ToDxLibVector(), m_rang, 16, 0xffffff, 0xffffff, false);
}

bool SearchPlace::IsInSearchPlace(Vector3 targetPos) const
{
	//”ÍˆÍ“à‚É‚¢‚é‚©
	float dist = Vector3(targetPos - m_pos).Magnitude();
	if (dist <= m_rang)
	{
		return true;
	}
	//‚±‚±‚Ü‚Å—ˆ‚½‚ç
	return false;
}
	