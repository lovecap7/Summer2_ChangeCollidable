#include "AttackBase.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../ActorStateBase.h"
#include "../Player/UltGage.h"
#include "../../../General/HitPoints.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<ActorStateBase> ownerState):
	Actor(shape),
	m_damage(0.0f),
	m_keepFrame(0.0f),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_ownerState(ownerState)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_ownerState.lock()->GetOwner().lock();
	auto otherColl = other;

	//�����Ɠ�����ނȂ疳��
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//�v���C���[���G�Ȃ�
	if (otherColl->GetGameTag() == GameTag::Player)
	{
		if (std::dynamic_pointer_cast<Player>(otherColl)->GetHitPoints()->IsNoDamege())
		{
			//�_���[�W���󂯂Ȃ���ԂȂ疳��
			return;
		}
	}
	else if (otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<EnemyBase>(otherColl)->GetHitPoints()->IsNoDamege())
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
	if (!isFind)
	{
		//�L�^����Ă��Ȃ���΋L�^����
		m_hitId.emplace_back(otherActor->GetID());
		//�v���C���[�̍U���̏ꍇ�K�E�Z�Q�[�W�����Z����
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			auto gage = std::dynamic_pointer_cast<Player>(ownerColl)->GetUltGage();
			gage->AddPedingUltGage();//�\�񂳂�Ă������Z�Q�[�W�ʂ𔽉f
		}
	}
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//�m�b�N�o�b�N
	Vector3 knockBackVec = other - m_ownerState.lock()->GetOwner().lock()->GetNextPos();
	knockBackVec.y = 0.0f;//Y�����͂Ȃ�
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//�m�b�N�o�b�N
	return knockBackVec;
}

void AttackBase::AttackSetting(int damage, int keepFrame, int knockBackPower, Battle::AttackWeight aw)
{
	m_damage = damage;
	m_keepFrame = keepFrame;
	m_knockBackPower = knockBackPower;
	m_attackWeight = aw;
}
