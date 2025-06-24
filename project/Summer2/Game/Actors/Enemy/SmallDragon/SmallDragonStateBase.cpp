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
	//攻撃を入れる
	attack->Init();//状態をリセット
	attackManager->Entry(attack);//登録
}

void SmallDragonStateBase::ChangeState(std::shared_ptr<SmallDragonStateBase> nextState)
{
	//状態変化
	m_nextState = move(nextState);
}

void SmallDragonStateBase::LookPlayer()
{
	//モデルの向きをプレイヤーに向ける
	auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	m_owner->GetModel()->SetDir(dir);
}

