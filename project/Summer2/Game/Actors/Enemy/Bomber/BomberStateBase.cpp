#include "BomberStateBase.h"
#include "Bomber.h"
#include "../../../../General/Input.h"
#include "../../../../Game/Camera/Camera.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Attack/AttackBase.h"
BomberStateBase::BomberStateBase(std::shared_ptr<Bomber> owner):
	m_owner(owner),
	m_nextState(nullptr)
{

}

BomberStateBase::~BomberStateBase()
{
}

void BomberStateBase::AppearAttack(std::shared_ptr<AttackBase> attack, const std::shared_ptr<AttackManager> attackManager)
{
	//攻撃を入れる
	attack->Init();//状態をリセット
	attackManager->Entry(attack);//登録
}


void BomberStateBase::ChangeState(std::shared_ptr<BomberStateBase> nextState)
{
	//状態変化
	m_nextState = move(nextState);
}

void BomberStateBase::LookPlayer()
{
	//モデルの向きをプレイヤーに向ける
	auto dir = m_owner->GetPlayerNomVecXZ().XZ();
	m_owner->GetModel()->SetDir(dir);
}