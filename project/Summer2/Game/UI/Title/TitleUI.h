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

};

