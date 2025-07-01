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
	//�ő�Ȃ�
	if (IsMaxUlt())return;
	//���Z
	m_ultGageValue += add;
	//�N�����v
	m_ultGageValue = MathSub::ClampInt(m_ultGageValue, 0, m_maxUltGageValue);
}
