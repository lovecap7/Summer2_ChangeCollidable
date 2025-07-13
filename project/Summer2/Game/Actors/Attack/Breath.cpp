#include "Breath.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../../Camera/Camera.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"

Breath::Breath(std::weak_ptr<Actor> owner) :
	SphereAttackBase(owner)
{
}

void Breath::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//‚¿å‚ª•sİ‚È‚ç
	if (m_owner.expired())
	{
		m_isDelete = true;
		return;
	}
	//ˆÚ“®
	m_rb->m_pos = m_rb->GetNextPos();
	//‹¤’Ê‚Ìˆ—‚ğ‚·‚é
	AttackBase::Update();
}


void Breath::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}


void Breath::SetVec(Vector3 vec)
{
	//ˆÚ“®—Ê
	m_rb->m_vec = vec;
}