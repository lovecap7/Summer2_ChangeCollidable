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
	//�ő�Ȃ�
	if (IsMaxUlt())return;
	//���Z
	m_ultGage += add;
	//�N�����v
	m_ultGage = MathSub::ClampInt(m_ultGage, 0, m_maxUltGage);
}
