#pragma once
#include "EnemyUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class EnemyBase;
class EnemyHPUI :
    public EnemyUIBase
{
public:
    EnemyHPUI(int handle, std::weak_ptr<EnemyBase> enemy);
    ~EnemyHPUI();
    void Init()override;
    void Update(const std::weak_ptr<ActorManager> actorManager)override;
    void Draw() const override;
    void End()override;
private:
    float m_viewHp;
    float m_viewMaxHp;
    //ï`âÊç¿ïW
    Vector3 m_pos;
};

