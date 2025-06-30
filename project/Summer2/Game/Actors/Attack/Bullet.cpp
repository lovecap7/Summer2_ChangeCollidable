#include "Bullet.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../Character/CharacterBase.h"
#include "../../../General/HitPoints.h"
#include "../Character/Player/Player.h"
#include "../Character/Player/UltGage.h"
Bullet::Bullet(std::weak_ptr<Actor> owner):
	SphereAttackBase(owner)
{
}

void Bullet::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//�����傪�s�݂Ȃ�
	if (m_owner.expired())
	{
		m_isDelete = true;
		return;
	}

	//�ړ�
	m_rb->m_pos = m_rb->GetNextPos();
	//���ʂ̏���������
	AttackBase::Update();
}

void Bullet::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//�����Ɠ�����ނȂ疳��
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//�v���C���[���G�Ȃ�
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints().lock()->IsNoDamege())
		{
			//�_���[�W���󂯂Ȃ���ԂȂ疳��
			return;
		}
	}
	else
	{
		//����ȊO��Actor�͖���
		return;
	}
	//�v���C���[�̍U���̏ꍇ�K�E�Z�Q�[�W�����Z����
	if (ownerColl->GetGameTag() == GameTag::Player)
	{
		std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage()->AddPedingUltGage();//�\�񂳂�Ă������Z�Q�[�W�ʂ𔽉f
	}
	//�U�����󂯂��Ƃ��̏���
	std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	//����
	m_isDelete = true;
}

void Bullet::Draw() const
{
#if _DEBUG
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->m_radius, 16, 0xff0000, 0xff0000, false);
#endif
}


void Bullet::SetVec(Vector3 vec)
{
	//�ړ���
	m_rb->m_vec = vec;
}