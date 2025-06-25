#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class Model;
/// <summary>
/// �X�e�[�W�̃I�u�W�F�N�g��`�悷��p�̃N���X
/// </summary>
class StageObjectDraw :
    public Actor
{
public:
    StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);
    ~StageObjectDraw();
    void Init()override {};
    void Update(const std::weak_ptr<Camera> camera) override {};
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override;
    void Complete() override {};
private:
    //���f��
    std::shared_ptr<Model> m_model;
};

