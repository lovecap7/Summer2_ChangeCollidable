#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class EnemyBase;
class BossHPUI :
    public EnemyUIBase
{
public:
    BossHPUI(int handle, std::weak_ptr<EnemyBase> enemy);
    ~BossHPUI();
    void Init()override;
    void Update(const std::weak_ptr<ActorManager> actorManager)override;
    void Draw() const override;
    void End()override;
private:
    float m_viewHp;
    float m_viewMaxHp;

};

