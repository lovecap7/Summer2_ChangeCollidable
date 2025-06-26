#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class UltGage;
class Camera;
class PlayerStateBase;
class Player :
	public Actor
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g�֐�
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
private:
	//�v���C���[�̏��
	std::shared_ptr<PlayerStateBase> m_state;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
};

