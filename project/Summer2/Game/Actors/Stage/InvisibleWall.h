#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
#include <memory>
class ActorManager;
class Camera;
class InvisibleWall :
    public Actor
{
public:
    InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle);
    ~InvisibleWall();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_modelHandle;
};

