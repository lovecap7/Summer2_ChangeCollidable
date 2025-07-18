#pragma once
#include "UIBase.h"
#include <memory>

class ActorManager;
class Score;
class ScoreUI :
    public UIBase
{
public:
    ScoreUI(int handle, const std::weak_ptr<Score> score);
    ~ScoreUI();
    void Init()override;
    void Update(const std::weak_ptr<ActorManager> actorManager)override;
    void Draw() const override;
    void End()override;
private:
    int m_viewScore;	//現在表示しているスコア
    int m_viewMaxScore;	//現在の最大スコア
    std::weak_ptr<Score> m_score;
};

