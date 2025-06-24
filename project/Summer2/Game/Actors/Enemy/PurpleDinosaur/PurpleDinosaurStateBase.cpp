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
	//攻撃を入れる
	attack->Init();//状態をリセット
	attackManager->Entry(attack);//登録
}

void PurpleDinosaurStateBase::ChangeState(std::shared_ptr<PurpleDinosaurStateBase> nextState)
{
	//状態変化
	m_nextState = move(nextState);
}

void PurpleDinosaurStateBase::LookPlayer()
{
	//モデルの向きをプレイヤーに向ける
	auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	m_owner->GetModel()->SetDir(dir);
}
