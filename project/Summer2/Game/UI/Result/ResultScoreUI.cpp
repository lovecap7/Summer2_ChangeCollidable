#include "ResultScoreUI.h"
#include "../../../General/game.h"
#include "../../GameRule/Score.h"
#include "../UIManager.h"

ResultScoreUI::ResultScoreUI(int scoreValue, Vector2 basePos, float scale, float digitMargin):
	ScoreUIBase(basePos, scale, digitMargin),
	m_scoreValue(scoreValue)
{
}

ResultScoreUI::~ResultScoreUI()
{
}

void ResultScoreUI::Update()
{
	//スコア更新
	UpdateViewScore(m_scoreValue);
}

void ResultScoreUI::Draw() const
{
	ScoreUIBase::Draw();
}
