#include "TitleSelectMenuUI.h"
#include "../UIManager.h"

namespace
{
	//選択中の座標をずらす値
	constexpr float kSelectOffsetPosX = 100.0f;
	//lerp率
	constexpr float kLerpRate = 0.1f;
}

TitleSelectMenuUI::TitleSelectMenuUI(Vector2 pos, const std::wstring& text):
	UIBase(),
	m_pos(pos),
	m_textHandle(UIManager::GetInstance().GetTextHandle("MSPゴシック48")),
	m_text(text),
	m_isSelect(false)
{
	//初期位置
	m_firstPos = pos;
	//選択中の位置
	m_selectPos = m_pos;
	m_selectPos.x += kSelectOffsetPosX;
}

TitleSelectMenuUI::~TitleSelectMenuUI()
{

}


void TitleSelectMenuUI::Update()
{
	if (m_isSelect)
	{
		m_pos = Vector2::Lerp(m_pos, m_selectPos, kLerpRate);
	}
	else
	{
		m_pos = Vector2::Lerp(m_pos, m_firstPos, kLerpRate);
	}
}

void TitleSelectMenuUI::Draw() const
{
	if (!m_isDraw)return;
	//テキスト
	DrawStringToHandle(m_pos.x, m_pos.y, m_text.c_str(), 0xffffff, m_textHandle);
}
