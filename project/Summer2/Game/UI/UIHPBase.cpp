#include "UIHPBase.h"
#include "../Attack/HurtPoint.h"

UIHPBase::UIHPBase(std::shared_ptr<HurtPoint> hurtPoint):
	UIBase(),
	m_maxHp(hurtPoint->GetHp()),
	m_nowHp(hurtPoint->GetHp()),
	m_ownerHP(hurtPoint)
{
}

UIHPBase::~UIHPBase()
{
}