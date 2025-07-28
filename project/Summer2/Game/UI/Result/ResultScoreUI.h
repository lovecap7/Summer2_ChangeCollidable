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
    //スコアの加算を終了したか
    bool IsFinishScore()const;
private:
    int m_scoreValue;
	//スコアの加算を終了
    void FinishScore();
    //フレームを数える
	int m_countFrame;
};

