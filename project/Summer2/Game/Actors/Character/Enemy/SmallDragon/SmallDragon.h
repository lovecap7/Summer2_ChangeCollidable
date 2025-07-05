#pragma once
#include "../EnemyBase.h"
class ActorManager;
class Camera;
class Collidable;
class SmallDragon :
	public EnemyBase
{
public:
	SmallDragon(int modelHandle, Vector3 pos);
	~SmallDragon();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//�I������
	void End() override;
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
	//�^�[�Q�b�g�̃f�[�^
	TargetData GetTargetData() const { return m_targetData; };
private:
};

