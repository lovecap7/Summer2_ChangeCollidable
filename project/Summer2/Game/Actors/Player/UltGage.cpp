#include "UltGage.h"
#include "../../../General/Math/MathSub.h"

UltGage::UltGage(int max):
	m_ultGage(0),
	m_maxUltGage(max),
	m_pendingUltGauge(0)
{
}

UltGage::~UltGage()
{
}

void UltGage::AddUltGage(int add)
{
	//Å‘å‚È‚ç
	if (IsMaxUlt())return;
	//‰ÁZ
	m_ultGage += add;
	//ƒNƒ‰ƒ“ƒv
	m_ultGage = MathSub::ClampInt(m_ultGage, 0, m_maxUltGage);
}
