#include "AreaOfEffectAttack.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../Character/CharacterBase.h"

AreaOfEffectAttack::AreaOfEffectAttack(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
}

AreaOfEffectAttack::~AreaOfEffectAttack()
{
}

void AreaOfEffectAttack::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update();
}

void AreaOfEffectAttack::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}
void AreaOfEffectAttack::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	if (m_isHit)
	{
		auto otherPos = std::dynamic_pointer_cast<CharacterBase>(other)->GetPos();
		auto centerPos = (otherPos + m_rb->m_pos) / 2.0f;
		//�q�b�g�G�t�F�N�g
		auto eff = EffekseerManager::GetInstance().CreateEffect("ImpactHitEff", centerPos);
	}
}
