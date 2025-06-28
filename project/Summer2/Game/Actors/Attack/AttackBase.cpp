#include "AttackBase.h"
#include "../Character/Player/Player.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/CharacterStateBase.h"
#include "../Character/Player/UltGage.h"
#include "../../../General/HitPoints.h"

AttackBase::AttackBase(Shape shape, std::weak_ptr<Actor> owner):
	Actor(shape),
	m_owner(owner),
	m_damage(0.0f),
	m_keepFrame(0.0f),
	m_knockBackPower(0.0f),
	m_attackWeight(Battle::AttackWeight::Light),
	m_ownerTag(owner.lock()->GetGameTag())
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

void AttackBase::Update()
{
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
}

void AttackBase::OnCollide(const std::shared_ptr<Collidable> other)
{
	auto ownerColl = m_owner.lock();
	auto otherColl = other;

	//�����Ɠ�����ނȂ疳��
	if (otherColl->GetGameTag() == ownerColl->GetGameTag())return;

	//�v���C���[���G�Ȃ�
	if (otherColl->GetGameTag() == GameTag::Player ||
		otherColl->GetGameTag() == GameTag::Enemy)
	{
		if (std::dynamic_pointer_cast<CharacterBase>(otherColl)->GetHitPoints()->IsNoDamege())
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
			std::dynamic_pointer_cast<Player>(m_owner.lock())->GetUltGage()->AddPedingUltGage();//�\�񂳂�Ă������Z�Q�[�W�ʂ𔽉f
		}
		//�U�����󂯂��Ƃ��̏���
		std::dynamic_pointer_cast<CharacterBase>(otherColl)->OnHitFromAttack(shared_from_this());
	}
}

Vector3 AttackBase::GetKnockBackVec(Position3 other)
{
	//�m�b�N�o�b�N
	Vector3 knockBackVec = other - m_owner.lock()->GetNextPos();
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
