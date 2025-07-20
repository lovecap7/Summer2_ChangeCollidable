#pragma once
#include "UIBase.h"
#include <memory>
#include <string>

class ActorManager;
class Timer;
class TutorialUI :
    public UIBase
{
public:
    TutorialUI(const std::wstring& text);
    ~TutorialUI();
    void Update()override;
    void Draw() const override;
private:
    //フレーム
    int m_countFrame;
    //表示している文字数
    int m_chatCount;
    //テキスト
    std::wstring m_text;
};


