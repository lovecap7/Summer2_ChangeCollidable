#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class EnemyBase;
class BossHPUI :
    public EnemyUIBase
{
public:
    BossHPUI(std::weak_ptr<EnemyBase> enemy);
    ~BossHPUI();
    void Update()override;
    void Draw() const override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //ƒtƒŒ[ƒ€
	int m_frameHandle;
	//‘Ì—Í
	int m_hpHandle;
};

