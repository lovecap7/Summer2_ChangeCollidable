#pragma once
#include "../UIBase.h"
#include <memory>

class TitleUI :
    public UIBase
{
public:
    TitleUI();
    ~TitleUI();
    void Update()override;
    void Draw() const override;
private:
    int m_countFrame;
    //ƒnƒ“ƒhƒ‹
    int m_titleHandle;
    int m_titleBackHandle;
    int m_pressAnyBottunHandle;
    int m_pressAnyBottunBackHandle;
    //Œˆ’è
    bool m_isDecide;
};

