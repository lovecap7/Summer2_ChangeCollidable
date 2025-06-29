#include "PlayerHpUI.h"
#include "../../General/HitPoints.h"
#include <DxLib.h>

namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 50.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerHpUI::PlayerHpUI(std::weak_ptr<HitPoints> hp):
	UIBase(),
	m_hitPoints(hp),
	m_viewHp(0.0f),
	m_viewMaxHp(0.0f)
{
}

PlayerHpUI::~PlayerHpUI()
{
}

void PlayerHpUI::Init()
{
	//‰Šú‰»
	if (!m_hitPoints.expired())
	{
		m_viewHp = m_hitPoints.lock()->GetHp();
		m_viewMaxHp = m_viewHp;
	}
}

void PlayerHpUI::Update()
{
	//XV
	if (!m_hitPoints.expired())
	{
		m_viewHp = m_hitPoints.lock()->GetHp();
	}
}

void PlayerHpUI::Draw() const
{
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0xff5555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewHp / m_viewMaxHp) * kBarWidth, kRightPosY, 0x55ff55, true);
}

void PlayerHpUI::End()
{
}
