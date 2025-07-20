#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class EnemyBase;
class EnemyHPUI :
    public EnemyUIBase
{
public:
    EnemyHPUI(std::weak_ptr<EnemyBase> enemy);
    ~EnemyHPUI();
    void Update()override;
    void Draw() const override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //ï`âÊç¿ïW
    Vector3 m_pos;
};

