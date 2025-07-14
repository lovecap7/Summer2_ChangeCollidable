#include "Strike.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../../Camera/Camera.h"

Strike::Strike(std::weak_ptr<Actor> owner):
	CapsuleAttackBase(owner)
{
}

Strike::~Strike()
{
}

void Strike::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update(actorManager);
}

void Strike::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	DrawCapsule3D(m_rb->m_pos.ToDxLibVector(), coll->m_endPos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}
void Strike::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	if (m_isHit)
	{
		//ヒットエフェクト
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		auto eff = EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", coll->m_endPos);
	}
}
