#pragma once
#include "UIHPBase.h"
class HurtPoint;
class UIPlayerHP :
    public UIHPBase
{
public:
    UIPlayerHP(std::shared_ptr<HurtPoint> hurtPoint);
    ~UIPlayerHP();
    void Update() override;
    void Draw() const override;
private:
};

