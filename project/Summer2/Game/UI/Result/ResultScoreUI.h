#pragma once
#include "../ScoreUIBase.h"
class Score;
class ResultScoreUI :
    public ScoreUIBase
{
public:
    ResultScoreUI(int scoreValue, Vector2 basePos, float scale, float digitMargin);
    virtual ~ResultScoreUI();
    void Update()override;
    void Draw()const override;
private:
    int m_scoreValue;
};

