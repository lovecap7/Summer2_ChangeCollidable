#pragma once
#include "CapsuleAttackBase.h"
class Strike :
    public CapsuleAttackBase
{
public:
    Strike(std::weak_ptr<Actor> owner);
    ~Strike();
    //更新処理
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    //描画
    void Draw()const override;
    //当たった時
    void OnCollide(const std::shared_ptr<Collidable> other) override;
};

