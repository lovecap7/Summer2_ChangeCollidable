#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class Collidable;
/// <summary>
/// �X�e�[�W�̃I�u�W�F�N�g�̓����蔻��p�̃N���X
/// </summary>
class StageObjectCollision :
    public Actor
{
public:
    StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);//�|���S���̃R���W����
    StageObjectCollision(VECTOR pos, float radius);//���̃R���W����
    StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius);//�J�v�Z���̃R���W����
    ~StageObjectCollision();
    void Init()override;
    void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
    void Draw()const override;
    void Complete() override;
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    void End()override;
    //���蔲���邩
    void SetIsThrough(bool isThrough);
private:
    int m_collisionHandle;
};


