#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"
#include "../../../General/Input.h"
#include "../../../General/StringUtil.h"
#include <format>//0埋め用

namespace
{
	//入力のインターバル
	constexpr int kCanInputFrame = 20;
	//数字の座標オフセット
	constexpr int kNumOffsetX = 600;
}

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text):
	UIBase(),
	m_scoreValue(scoreValue),
	m_basePos(basePos),
	m_countFrame(0),
	m_textHandle(UIManager::GetInstance().GetTextHandle("メイリオ64")),
	m_text(text)
{
}

ResultScoreUI::~ResultScoreUI()
{
}

void ResultScoreUI::Update()
{
	auto& input = Input::GetInstance();
	++m_countFrame;
	if (input.IsTrigger("A") && m_countFrame >= kCanInputFrame)
	{
	
		return;
	}
	//スコア更新
	
}

void ResultScoreUI::Draw() const
{
	std::wstring num = L": " + std::format(L"{:06}", m_scoreValue);
	//テキスト
	DrawStringToHandle(m_basePos.x, m_basePos.y, m_text.c_str(), 0x000000, m_textHandle);
	DrawStringToHandle(m_basePos.x + kNumOffsetX, m_basePos.y, num.c_str(), 0x000000, m_textHandle);
}