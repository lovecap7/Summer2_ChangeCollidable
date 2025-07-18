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
    int m_viewScore;	//���ݕ\�����Ă���X�R�A
    int m_viewMaxScore;	//���݂̍ő�X�R�A
    std::weak_ptr<Score> m_score;
};

