#pragma once
#include "UIBase.h"
#include <memory>
#include "../../General/Math/MyMath.h"

namespace
{
    //�X�R�A�̌���
    constexpr int kDigitNum = 6;
}

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
    //�X�R�A���ϓ����邽�тɂ͂˂�
    Vector2 m_viewPoses[kDigitNum];
    //�e�ʂ̒l
    int m_digits[kDigitNum];
};

