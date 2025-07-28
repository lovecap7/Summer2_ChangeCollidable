#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"
#include "../../../General/Input.h"
#include "../../../General/StringUtil.h"
namespace
{
	//入力のインターバル
	constexpr int kCanInputFrame = 20;
}

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text):
	UIBase(),
	m_scoreValue(scoreValue),
	m_basePos(basePos),
	m_countFrame(0),
	m_textHandle(UIManager::GetInstance().GetTextHandle("メイリオ")),
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

	std::wstring text = m_text + std::to_wstring(m_scoreValue);
	//テキスト
	DrawStringToHandle(m_basePos.x, m_basePos.y, text.c_str(), 0x00ffff, m_textHandle);
}