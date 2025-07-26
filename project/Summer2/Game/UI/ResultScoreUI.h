#pragma once
#include "ScoreUI.h"
class ResultScoreUI :
    public ScoreUI
{
public:
    ResultScoreUI(const std::weak_ptr<Score> score);
    ~ResultScoreUI();
    void Update()override;
    void Draw() const override;
};

