#include "UIPlayerHP.h"
#include "../Attack/HurtPoint.h"
#include <DxLib.h>

UIPlayerHP::UIPlayerHP(std::shared_ptr<HurtPoint> hurtPoint):
	UIHPBase(hurtPoint)
{
}

UIPlayerHP::~UIPlayerHP()
{
}

void UIPlayerHP::Update()
{
	m_nowHp = m_ownerHP->GetHp();
	if (m_nowHp < 0.0f)
	{
		m_nowHp = 0.0f;
	}
}

void UIPlayerHP::Draw() const
{
	DrawBoxAA(50.0f, 50.0f, 50.0f + (m_maxHp / m_maxHp) * 400.0f, 100.0f, 0xff5555, true);
	DrawBoxAA(50.0f, 50.0f, 50.0f + (m_nowHp / m_maxHp) * 400.0f, 100.0f, 0x55ff55, true);
}
