#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
//プレイヤーがボス部屋に入ったことを検知するクラス
class BossArea :
    public Actor
{
public:
    BossArea(VECTOR startPos,VECTOR endPos);
    ~BossArea();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override;
    void Complete() override {};
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
    void End()override {};
private:
    Vector3 m_startPos;
    Vector3 m_endPos;
};

