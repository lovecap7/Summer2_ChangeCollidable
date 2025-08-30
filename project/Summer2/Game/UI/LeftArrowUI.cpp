#include "LeftArrowUI.h"
#include "UIManager.h"

LeftArrowUI::LeftArrowUI(Vector2 pos,bool isRight):
	m_isSelect(false),
	m_scale(1.0),
	m_baseScale(1.0),
	m_scaleAngle(0.0f),
	m_handle(UIManager::GetInstance().GetImageHandle("LeftArrow")),
	m_pos(pos),
	m_isRight(isRight)
{
}

LeftArrowUI::~LeftArrowUI()
{
}

void LeftArrowUI::Update()
{
	if (m_isSelect)
	{
		m_scaleAngle += 0.1;
		if (m_scaleAngle > MyMath::TwoPI_F)m_scaleAngle -= MyMath::TwoPI_F;
		m_scale = m_baseScale + 0.1 * sinf(m_scaleAngle);
	}
	else
	{
		m_scale = m_baseScale;
		m_scaleAngle = 0.0f;
	}
}

void LeftArrowUI::Draw() const
{
	if (!m_isDraw)return;
	DrawRotaGraphF(m_pos.x, m_pos.y, m_scale, 0.0, m_handle, true, m_isRight);
}
