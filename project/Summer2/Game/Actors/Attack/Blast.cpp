#include "Blast.h"
#include "AreaOfEffectAttack.h"
#include "../Character/CharacterBase.h"
#include "../../../General/Collision/SphereCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/HitPoints.h"

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

void Blast::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto otherColl = other;
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
	std::shared_ptr<Actor> otherActor = std::dynamic_pointer_cast<Actor>(otherColl);
	bool isFind = false;
	//ID�����łɋL�^����Ă��邩�m�F
	for (auto id : m_hitId)
	{
		if (id == otherActor->GetID())
		{
			isFind = true;
			break;
		}
	}
	//�U������
	if (!isFind)
	{
		//�L�^����Ă��Ȃ���΋L�^����
		m_hitId.emplace_back(otherActor->GetID());
		//�U�����󂯂��Ƃ��̏���
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
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


