#pragma once
#include "UIBase.h"
#include <memory>
#include "../../General/Math/MyMath.h"

namespace
{
    //スコアの桁数
    constexpr int kDigitNum = 6;
}

class ActorManager;
class Score;
class ScoreUI :
    public UIBase
{
public:
    ScoreUI(const std::weak_ptr<Score> score);
    ~ScoreUI();
    void Update()override;
    void Draw() const override;
private:
    int m_viewScore;	//現在表示しているスコア
    int m_viewMaxScore;	//現在の最大スコア
    std::weak_ptr<Score> m_score;
    //スコアが変動するたびにはねる
    Vector2 m_viewPoses[kDigitNum];
    Vector2 m_viewVecs[kDigitNum];
    //各位の値
    int m_digits[kDigitNum];
    //ハンドル
    int m_handle;
};

