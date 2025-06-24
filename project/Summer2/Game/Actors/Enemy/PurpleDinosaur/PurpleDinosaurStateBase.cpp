#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaur.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::shared_ptr<PurpleDinosaur> owner):
	m_owner(owner),
	m_nextState(nullptr)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}
void PurpleDinosaurStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager)
{
	//�U��������
	attack->Init();//��Ԃ����Z�b�g
	attackManager->Entry(attack);//�o�^
}

void PurpleDinosaurStateBase::ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

void PurpleDinosaurStateBase::LookPlayer()
{
	//���f���̌������v���C���[�Ɍ�����
	auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	m_owner->GetModel()->SetDir(dir);
}
