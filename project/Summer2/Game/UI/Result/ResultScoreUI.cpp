#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"
#include "../../../General/Input.h"

namespace
{
	//入力のインターバル
	constexpr int kCanInputFrame = 20;
}

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, float scale, float digitMargin):
	ScoreUIBase(basePos, scale, digitMargin),
	m_scoreValue(scoreValue),
	m_countFrame(0)
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
		//Aボタンでスコアの加算を終了
		FinishScore();
		return;
	}
	//スコア更新
	UpdateViewScore(m_scoreValue);
}

void ResultScoreUI::Draw() const
{
	ScoreUIBase::Draw();
}

void ResultScoreUI::FinishScore()
{
	m_viewScore = m_scoreValue;
}

bool ResultScoreUI::IsFinishScore() const
{
	return m_viewScore >= m_scoreValue;
}
