#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class AttackBase;
class MeleeAttack;
class AttackManager;
class UIManager;
class UltGage;
class SearchTrigger;
class PlayerStateBase;
class Player :
	public Actor, public std::enable_shared_from_this<Player>
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//�o�^����
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//�o�^����
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//����������
	void Init()override;
	//�X�V����
	void Update(const Input& input,const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
	//�d��
	void Gravity(const Vector3& gravity)override;
	//�Փ˃C�x���g�֐�
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
	//�n�ʂɕt���Ă��邩
	bool IsGround() { return m_isGround; };
	void NoIsGround() { m_isGround = false; };//�n�ʂɕt���Ă��Ȃ��Ɣ��f�����Ƃ��ɌĂ�
	//���͒��̕����L�[
	Vector2 GetStickVec() { return m_stickVec; };
	//���f���N���X
	std::shared_ptr<Model> GetModel() const{ return m_model; };
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//���G
	std::shared_ptr<SearchTrigger> GetSearchTrigger() { return m_searchTrigger; };
	//�A�C�e��
	//�_���[�W�J�b�g
	void SetDamageCut(float cutRate, int keepFrame);
private:
	//�v���C���[�̏��
	std::shared_ptr<PlayerStateBase> m_state;
	//���f��
	std::shared_ptr<Model> m_model;
	//�X�e�B�b�N�̌��������x�N�g��
	Vector2 m_stickVec;
	//�n�ʂɕt���Ă��邩�ǂ���
	bool m_isGround;
	//�K�E�Z�Q�[�W
	std::shared_ptr<UltGage> m_ultGage;
	//���G�g���K�[
	std::shared_ptr<SearchTrigger> m_searchTrigger;
	//�_���[�W�J�b�g�t���[��
	int m_damageCutFrame;
	//�_���[�W�A�b�v��
	float m_damageUpRate;
	//�_���[�W�A�b�v�̎����t���[��
	int m_damageUpKeepFrame;	
private:
	//���ꔻ��̍X�V
	void UpdateHurtPoint();
};

