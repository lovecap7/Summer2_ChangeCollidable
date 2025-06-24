#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class Camera;
class AttackManager;
class UIManager;
/// <summary>
/// �X�e�[�W�̃I�u�W�F�N�g�̓����蔻��p�̃N���X
/// </summary>
class StageObjectCollision :
    public Actor, public std::enable_shared_from_this<StageObjectCollision>
{
public:
    StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);//�|���S���̃R���W����
    StageObjectCollision(VECTOR pos, float radius);//���̃R���W����
    StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius);//�J�v�Z���̃R���W����
    ~StageObjectCollision();
    //�o�^����
    void Entry(std::shared_ptr<ActorManager> actorManager) override;
    //�o�^����
    void Exit(std::shared_ptr<ActorManager> actorManager) override;
    void Init()override;
    void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
    void Gravity(const Vector3& gravity)override;
    void OnHitColl(const std::shared_ptr<Collidable>& other)override;
    void Draw()const override;
    void Complete() override;
private:
    int m_collisionHandle;
};


