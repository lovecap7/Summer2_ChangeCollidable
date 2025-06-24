#include "UIPlayerUltGage.h"
#include "../Actors/Player/UltGage.h"
#include <DxLib.h>

UIPlayerUltGage::UIPlayerUltGage(std::shared_ptr<UltGage> ultGage):
	m_ultGage(ultGage),
	m_nowUltGage(0),
	m_maxUltGage(m_ultGage->GetMaxUltGage())
{
}

UIPlayerUltGage::~UIPlayerUltGage()
{
}

void UIPlayerUltGage::Update()
{
	//更新が必要なら
	auto nowGage = m_ultGage->GetUltGage();
	if (static_cast<int>(m_nowUltGage) != nowGage)
	{
		//必殺技ゲージの更新
		m_nowUltGage = nowGage;
	}
}

void UIPlayerUltGage::Draw() const
{
	DrawBoxAA(50.0f, 110.0f, 50.0f + (m_maxUltGage / m_maxUltGage) * 400.0f, 150.0f, 0x555555, true);
	DrawBoxAA(50.0f, 110.0f, 50.0f + (m_nowUltGage / m_maxUltGage) * 400, 150, 0x5555ff, true);
}
