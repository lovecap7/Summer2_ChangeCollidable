#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class UltGage;
class Camera;
class CharacterStateBase;
class HitPoints;
class TrackActorEffect;
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
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//�I������
	void End()override;
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//�K�E�Z�Q�[�W
	std::weak_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
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
	//�Q�[���N���A�A�j���[�V�����I��
	bool IsFinishClearAnim();
	//�^�[�Q�b�g�̍��G
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos) override;
private:
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
	//�_�b�V����Ԃ��L�[�v����t���O
	bool m_isRunKeep;
	//�_�b�V����Ԃ���������t���[��
	int m_cancelRunFrame;
	//�K�E�Q�[�WMAX�G�t�F�N�g
	std::weak_ptr<TrackActorEffect> m_ultMaxEff;

	//���������Ƃ��ɖ߂����W
	Vector3 m_initPos;
private:
	//������p�����邩
	void CheckRunKeep();
	//�K�E�Q�[�W���ő傩�m�F
	void CheckUltMax();
};

