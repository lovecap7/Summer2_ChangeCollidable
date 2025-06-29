#include "PlayerUltGageUI.h"
#include "../Actors/Character/Player/UltGage.h"
#include <DxLib.h>
namespace
{
	constexpr float kBarHeight = 50.0f;
	constexpr float kBarWidth = 400.0f;
	constexpr float kLeftPosX = 50.0f;
	constexpr float kLeftPosY = 110.0f;
	constexpr float kRightPosY = kLeftPosY + kBarHeight;
}

PlayerUltGageUI::PlayerUltGageUI(std::weak_ptr<UltGage> ultGage):
	m_ultGage(ultGage),
	m_viewUltGage(0.0f),
	m_viewMaxUltGage(0.0f)
{
}

PlayerUltGageUI::~PlayerUltGageUI()
{
}

void PlayerUltGageUI::Init()
{
	//‰Šú‰»
	if (!m_ultGage.expired())
	{
		m_viewUltGage = m_ultGage.lock()->GetUltGage();
		m_viewMaxUltGage = m_ultGage.lock()->GetMaxUltGage();
	}
}

void PlayerUltGageUI::Update()
{
	//XV
	if (!m_ultGage.expired())
	{
		auto ultGage = m_ultGage.lock();
		//‘Ì—Í‚É•Ï“®‚ª‚ ‚Á‚½Žž
		if (m_viewUltGage != ultGage->GetUltGage())
		{
			m_viewUltGage = ultGage->GetUltGage();
		}
		if (m_viewMaxUltGage != ultGage->GetMaxUltGage())
		{
			m_viewMaxUltGage = ultGage->GetMaxUltGage();
		}
	}
}

void PlayerUltGageUI::Draw() const
{
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewMaxUltGage / m_viewMaxUltGage) * kBarWidth, kRightPosY, 0x555555, true);
	DrawBoxAA(kLeftPosX, kLeftPosY, kLeftPosX + (m_viewUltGage / m_viewMaxUltGage) * kBarWidth, kRightPosY, 0x5555ff, true);
}

void PlayerUltGageUI::End()
{
}
