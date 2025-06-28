#include "Blast.h"
#include "AreaOfEffectAttack.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"

Blast::Blast(std::weak_ptr<Actor> owner) :
	SphereAttackBase(owner)
{
}

Blast::~Blast()
{
}

void Blast::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�����t���[�������炷
	--m_keepFrame;
	//�����t���[����0�ɂȂ�����폜
	if (m_keepFrame < 0)
	{
		m_isDelete = true;	//�폜�t���O�𗧂Ă�
		m_isThrough = true;	//�����蔻������Ȃ�
		return; //�������Ȃ�
	}
}

void Blast::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}

Vector3 Blast::GetKnockBackVec(Position3 other)
{
	//�m�b�N�o�b�N
	Vector3 knockBackVec = other - m_rb->GetPos();
	knockBackVec.y = 0.0f;//Y�����͂Ȃ�
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//�m�b�N�o�b�N
	return knockBackVec;
}


