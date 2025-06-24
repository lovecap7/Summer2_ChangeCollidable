#include "BulletAttack.h"
#include "MeleeAttack.h"
#include "../Actors/Actor.h"
#include "HurtPoint.h"
#include "../../General/Collidable.h"
#include <DxLib.h>
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Rigidbody.h"

BulletAttack::BulletAttack(std::shared_ptr<Collidable> coll, int damage, int keepFrame,float knockBackPower, std::shared_ptr<Actor> owner):
	AttackBase(coll, damage,keepFrame, knockBackPower, owner),
	m_dir(Vector3::Zero()),
	m_speed(0.0f)
{
}

BulletAttack::~BulletAttack()
{
}

void BulletAttack::Init()
{
	//����������
	m_damage = m_initDamage;
	m_keepFrame = m_initKeepFrame;
	m_isDelete = false;
	//ID�̏�����
	if (m_hitId.size() != 0)
	{
		m_hitId.clear();
	}
}

void BulletAttack::Update()
{
	//�ړ�
	if (m_dir.Magnitude() > 0.0f)
	{
		Vector3 moveVec = m_dir.Normalize() * m_speed;
		m_collidable->GetRb()->SetPos(m_collidable->GetRb()->GetPos() + moveVec);
	}

	//�����t���[�������炷
	m_keepFrame--;
	//�����t���[����0�ɂȂ��������
	if (m_keepFrame <= 0)
	{
		m_isDelete = true;
	}
}

void BulletAttack::Draw()
{
#if _DEBUG
	if (m_collidable->GetColl()->GetShape() == Shape::Capsule)
	{
		DrawCapsule3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetEndPos().ToDxLibVector(),
			std::dynamic_pointer_cast<CapsuleCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}
	else if (m_collidable->GetColl()->GetShape() == Shape::Sphere)
	{
		DrawSphere3D(
			m_collidable->GetRb()->GetPos().ToDxLibVector(),
			std::dynamic_pointer_cast<SphereCollider>(m_collidable->GetColl())->GetRadius(),
			32,
			0xff00ff,
			0xff00ff,
			false
		);
	}

#endif
}

void BulletAttack::OnHit(std::shared_ptr<Actor> actor)
{
	auto ownerColl = m_owner->GetCollidable();
	auto otherColl = actor->GetCollidable();
	//�����Ɠ�����ނȂ疳��
	if (ownerColl->GetGameTag() == otherColl->GetGameTag())return;

	bool isFind = false;
	//ID�����łɋL�^����Ă��邩�m�F
	for (auto id : m_hitId)
	{
		if (id == actor->GetID())
		{
			isFind = true;
			break;
		}
	}
	if (!isFind)
	{
		//�L�^����Ă��Ȃ���΋L�^����
		m_hitId.emplace_back(actor->GetID());
		//�U�������������̏���������
		auto hurtPoint = actor->GetHurtPoint();
		hurtPoint->OnHit(shared_from_this());
		//�폜
		m_isDelete = true;
	}
}
