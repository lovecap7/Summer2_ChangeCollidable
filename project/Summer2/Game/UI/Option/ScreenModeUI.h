#pragma once
#include "../UIBase.h"
class ScreenModeUI :
    public UIBase
{
public:
    ScreenModeUI();
    ~ScreenModeUI();
    void Update()override;
    void Draw() const override;
    void SetWindowMode(bool isWindow) { m_isWindowMode = isWindow; };
private:
    bool m_isWindowMode;
    int m_windowModeHandle;
    int m_fullScreenModeHandle;
};

