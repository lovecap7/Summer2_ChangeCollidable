#include "UltGage.h"
#include "../../../../General/Math/MathSub.h"

UltGage::UltGage(int max):
	m_ultGageValue(0),
	m_maxUltGageValue(max),
	m_pendingUltGauge(0)
{
}

UltGage::~UltGage()
{
}

void UltGage::AddUltGage(int add)
{
	//最大なら
	if (IsMaxUlt())return;
	//加算
	m_ultGageValue += add;
	//クランプ
	m_ultGageValue = MathSub::ClampInt(m_ultGageValue, 0, m_maxUltGageValue);
}
