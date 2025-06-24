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
    float m_maxHp;//�ő�̗�
    float m_nowHp;//���݂̗̑�
    std::shared_ptr<HurtPoint> m_ownerHP;
};

