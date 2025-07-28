#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
#include <string>
class Score;
class ResultScoreUI :
    public UIBase
{
public:
    ResultScoreUI(int scoreValue, Vector2 basePos, const std::wstring& text);
    virtual ~ResultScoreUI();
    void Update()override;
    void Draw()const override;
private:
    //スコアの値
    int m_scoreValue;
    //座標
    Vector2 m_basePos;
    //ハンドル
    int m_textHandle;
    //フレームを数える
	int m_countFrame;
    //テキスト
    const std::wstring m_text;
};

