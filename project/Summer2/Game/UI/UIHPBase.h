#pragma once
#include "UIBase.h"
#include <memory>
class HurtPoint;
class UIHPBase abstract:
    public UIBase
{
public:
    UIHPBase(std::shared_ptr<HurtPoint> hurtPoint);
    ~UIHPBase();
protected:
    float m_maxHp;//Å‘å‘Ì—Í
    float m_nowHp;//Œ»İ‚Ì‘Ì—Í
    std::shared_ptr<HurtPoint> m_ownerHP;
};

