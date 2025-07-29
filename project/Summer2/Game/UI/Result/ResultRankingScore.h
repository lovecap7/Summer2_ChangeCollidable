#pragma once
#include "ResultScoreUI.h"
class ResultRankingScore :
    public ResultScoreUI
{
public:
    ResultRankingScore(int scoreValue, Vector2 basePos, const std::wstring& text);
    virtual ~ResultRankingScore();
	void Draw()const override;
private:
	int m_rankingHandle; // ランキングのハンドル
};

