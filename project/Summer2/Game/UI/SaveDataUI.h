#pragma once
#include "UIBase.h"
class SaveDataUI :
    public UIBase
{
public:
    SaveDataUI();
    ~SaveDataUI();
    void Update()override;
    void Draw() const override;
private:
    //ƒoƒbƒN‰æ‘œ
    int m_backHandle;
};

