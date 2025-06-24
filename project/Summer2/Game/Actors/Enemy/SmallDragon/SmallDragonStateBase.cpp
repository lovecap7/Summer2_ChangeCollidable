#include "SmallDragonStateBase.h"
#include "SmallDragon.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"

SmallDragonStateBase::SmallDragonStateBase(std::shared_ptr<SmallDragon> owner) :
	m_owner(owner),
	m_nextState(nullptr)
{
}

SmallDragonStateBase::~SmallDragonStateBase()
{
}

void SmallDragonStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager)
{
	//�U��������
	attack->Init();//��Ԃ����Z�b�g
	attackManager->Entry(attack);//�o�^
}

void SmallDragonStateBase::ChangeState(std::shared_ptr<SmallDragonStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

void SmallDragonStateBase::LookPlayer()
{
	//���f���̌������v���C���[�Ɍ�����
	auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	m_owner->GetModel()->SetDir(dir);
}

