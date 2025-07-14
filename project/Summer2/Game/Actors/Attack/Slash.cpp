#include "Slash.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Effect/MyEffect.h"
#include "../ActorManager.h"
#include "../../Camera/Camera.h"

Slash::Slash(std::weak_ptr<Actor> owner):
	CapsuleAttackBase(owner)
{
}

Slash::~Slash()
{
}

void Slash::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	AttackBase::Update(actorManager);
}

void Slash::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	DrawCapsule3D(m_rb->m_pos.ToDxLibVector(), coll->m_endPos.ToDxLibVector(),
		coll->m_radius,16,0xff0000,0xff0000,false);
#endif
}

void Slash::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
	//�U�������������Ȃ�
	if (m_isSuccessAttack)
	{
		//�q�b�g�G�t�F�N�g
		auto coll = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
		auto HitPos = (m_rb->m_pos + coll->m_endPos) / 2.0f;
		auto eff = EffekseerManager::GetInstance().CreateEffect("SlashHitEff", HitPos);
	}
}
