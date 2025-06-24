#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class Model;
/// <summary>
/// ステージのオブジェクトを描画する用のクラス
/// </summary>
class StageObjectDraw :
    public Actor, public std::enable_shared_from_this<StageObjectDraw>
{
public:
    StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);
    ~StageObjectDraw();
    //登録処理
    void Entry(std::shared_ptr<ActorManager> actorManager) override;
    //登録解除
    void Exit(std::shared_ptr<ActorManager> actorManager) override;
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    //モデル
    std::shared_ptr<Model> m_model;
};

