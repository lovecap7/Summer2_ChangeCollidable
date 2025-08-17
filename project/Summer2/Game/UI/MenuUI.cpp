#include "MenuUI.h"
#include "UIManager.h"

namespace
{
	//選択中の座標をずらす値
	constexpr float kSelectOffsetPosX = 100.0f;
	//lerp率
	constexpr float kLerpRate = 0.1f;
	//通常サイズ
	constexpr float kNormalSize = 1.0f;
	//選択している間のサイズ
	constexpr float kSelectSize = 1.1f;
}

MenuUI::MenuUI(Vector2 pos, int handle) :
	UIBase(true),
	m_pos(pos),
	m_handle(handle),
	m_isSelect(false),
	m_size(kNormalSize)
{
	
}

MenuUI::~MenuUI()
{

}


void MenuUI::Update()
{
	if (m_isSelect)
	{
		m_size = MathSub::Lerp(m_size, kSelectSize, kLerpRate);
	}
	else
	{
		m_size = MathSub::Lerp(m_size, kNormalSize, kLerpRate);
	}
}

void MenuUI::Draw() const
{
	if (!m_isDraw)return;
	//描画
	DrawRotaGraph(m_pos.x, m_pos.y, m_size, 0.0, m_handle, true);
}
