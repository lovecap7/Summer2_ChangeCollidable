#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class UltGage;
class Camera;
class CharacterStateBase;
class HitPoints;
class Player :
	public CharacterBase
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
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager) override;
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//�̗�
	std::shared_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
	//���W�b�h�{�f�B
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//�R���W����
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//�R���W�����̏�Ԃ�ݒ�
	void SetCollState(CollisionState collState) { m_collState = collState; }
	//�^�[�Q�b�g�̃f�[�^
	TargetData GetTargetData() const { return m_targetData; };
	//�_�b�V���L�[�v�t���O
	bool IsRunKeep() { return m_isRunKeep; };
	void SetIsRunKeep(bool isRunKeep) { m_isRunKeep = isRunKeep; };
private:
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
	//�_�b�V����Ԃ��L�[�v����t���O
	bool m_isRunKeep;
	//�_�b�V����Ԃ���������t���[��
	int m_cancelRunFrame;

	//���������Ƃ��ɖ߂����W
	Vector3 m_initPos;
};

