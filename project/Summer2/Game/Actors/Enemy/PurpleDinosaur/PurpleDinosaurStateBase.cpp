#include "PurpleDinosaurStateBase.h"
#include "PurpleDinosaur.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"

PurpleDinosaurStateBase::PurpleDinosaurStateBase(std::weak_ptr<PurpleDinosaur> owner):
	m_owner(owner),
	m_nextState(nullptr)
{
}

PurpleDinosaurStateBase::~PurpleDinosaurStateBase()
{
}


void PurpleDinosaurStateBase::ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState)
{
	//��ԕω�
	m_nextState = move(nextState);
}

void PurpleDinosaurStateBase::LookPlayer()
{
	//���f���̌������v���C���[�Ɍ�����
	//auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	//m_owner->GetModel()->SetDir(dir);
}
