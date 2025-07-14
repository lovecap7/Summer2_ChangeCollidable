#include "AttackBase.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/CharacterStateBase.h"
#include "../Character/Player/UltGage.h"
#include "../../../General/HitPoints.h"
#include "../../../General/AttackPoints.h"
#include "../../../General/Effect/EffekseerManager.h"
#include "../../../General/Rigidbody.h"
#include "../ActorManager.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<Actor> owner) :
	Actor(shape),
	m_owner(owner),
	m_damage(0.0f),
	m_keepFrame(0.0f),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_hitStopFrame(0),
	m_shakePower(ShakePower::None),
	m_ownerTag(owner.lock()->GetGameTag()),
	m_isHit(false),
	m_isSuccessAttack(false)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init()
{
	AllSetting(CollisionState::Normal, Priority::Low, GameTag::Attack, false, true,false);
	Collidable::Init();
}

void AttackBase::Update(const std::weak_ptr<ActorManager> actorManager)
{
	//�U�������������Ȃ�
	if (m_isHit)
	{
		//�q�b�g�X�g�b�v
		actorManager.lock()->HitStop(m_shakePower, m_hitStopFrame);
	}
	//������̎Q�Ƃ��؂ꂽ��
	if (m_owner.expired())
	{
		m_isDelete = true; //���L�҂̃X�e�[�g�������Ȃ�����폜�t���O�𗧂Ă�
		return; //���L�҂̃X�e�[�g�������Ȃ����牽�����Ȃ�
	}
	//�����t���[�������炷
	--m_keepFrame;
	//�����t���[����0�ɂȂ�����폜
	if (m_keepFrame < 0)
	{
		m_isDelete = true;	//�폜�t���O�𗧂Ă�
		m_isThrough = true;	//�����蔻������Ȃ�
		return; //�������Ȃ�
	}
	//������
	m_isHit = false;
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	if (m_owner.expired())return;
	m_isSuccessAttack = false;
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
		//�v���C���[�̍U���̏ꍇ�K�E�Z�Q�[�W�����Z����
		if (ownerColl->GetGameTag() == GameTag::Player)
		{
			std::dynamic_pointer_cast<Player>(m_owner.lock())->GetUltGage().lock()->AddPedingUltGage();//�\�񂳂�Ă������Z�Q�[�W�ʂ𔽉f
		}
		//�U�����󂯂��Ƃ��̏���
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
		//�U���ɐ��������̂�
		m_isSuccessAttack = true;
		//�U���𓖂Ă��̂�
		m_isHit = true;
	}
}

void AttackBase::End()
{
	Collidable::End();
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//�m�b�N�o�b�N
	Vector3 knockBackVec = other - m_owner.lock()->GetNextPos();
	knockBackVec.y = 0.0f;//Y�����͂Ȃ�
	knockBackVec = knockBackVec.Normalize() * m_knockBackPower;//�m�b�N�o�b�N
	return knockBackVec;
}

void AttackBase::AttackSetting(int damage, int keepFrame, int knockBackPower, Battle::AttackWeight aw, int hitStopFrame, ShakePower sp)
{
	m_damage = damage;
	m_keepFrame = keepFrame;
	m_knockBackPower = knockBackPower;
	m_attackWeight = aw;
	m_hitStopFrame = hitStopFrame;
	m_shakePower = sp;
}

int AttackBase::GetDamage()
{
	float damage = m_damage;
	if (!m_owner.expired())
	{
		//�_���[�W�{����������
		auto owner = m_owner.lock();
		if (owner->GetGameTag() == GameTag::Player ||
			owner->GetGameTag() == GameTag::Enemy)
		{
			damage *= std::dynamic_pointer_cast<CharacterBase>(owner)->GetAttackPoints().lock()->GetDamageRate();
		}
	}
	return damage;
}

Battle::AttackWeight AttackBase::GetAttackWeight()
{
	Battle::AttackWeight aw = m_attackWeight;
	if (!m_owner.expired())
	{
		//�U���̏d�����Œ�l�������ǂ���
		auto owner = m_owner.lock();
		if (owner->GetGameTag() == GameTag::Player ||
			owner->GetGameTag() == GameTag::Enemy)
		{
			//������̍Œ�l
			auto ownerAw = std::dynamic_pointer_cast<CharacterBase>(owner)->GetAttackPoints().lock()->GetLowestAW();
			//������̃A�[�}�[��菬�����Ȃ�
			if (!Battle::CheckFlinchAttackAndArmor(aw, ownerAw))
			{
				aw = ownerAw;
			}
		}
	}
	return aw;
}
