#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
/// //new�ł��Ȃ��Ȃ�
class Actor abstract
{
protected:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//���ꔻ��
	std::shared_ptr<HurtPoint> m_hurtPoint;
	//����
	bool m_isDelete;
	//�A�N�^�[�̎��ʔԍ�
	int m_id;
	//ID���Z�b�g���ꂽ���Ƃ��L�^����t���O
	bool m_isSetId;
	//�`��p�̃N���X���ǂ���
	bool m_isDrawOnly;
public:
	Actor();
	virtual ~Actor() {};
	//(�������z�֐�)
	/// <summary>
	/// �o�^����
	/// </summary>
	virtual void Entry(std::shared_ptr<ActorManager> actorManager)abstract;
	/// <summary>
	/// �o�^����
	/// </summary>
	/// <param name="actorManager"></param>
	virtual void Exit(std::shared_ptr<ActorManager> actorManager)abstract;
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��
	/// </summary>
	/// <param name="">���̓I�u�W�F�N�g</param>
	virtual void Update(const Input& input, const std::unique_ptr<Camera>& camera, std::shared_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// �d�͂��󂯂�
	/// </summary>
	/// <param name="gravity"></param>
	virtual void Gravity(const Vector3& gravity)abstract;
	/// <summary>
	/// �Փ˂����Ƃ��̃C�x���g�֐�
	/// </summary>
	/// <param name="other"></param>
	virtual void OnHitColl(const std::shared_ptr<Collidable>& other)abstract;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\��
	/// </summary>
	virtual void Draw()const abstract;
	/// <summary>
	/// Update�̏����ɂ���ċN�������Փˏ����Ȃǂ̏����̊m��
	/// </summary>
	virtual void Complete() abstract;
	/// <summary>
	/// �����蔻��ƏՓˏ����Ɏg��
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<Collidable> GetCollidable() const { return m_collidable; }
	/// <summary>
	/// ���ꔻ��
	/// </summary>
	/// <returns></returns>
	const std::shared_ptr<HurtPoint> GetHurtPoint() const { return m_hurtPoint; }
	/// <summary>
	/// �폜�t���O
	/// </summary>
	/// <returns></returns>
	bool IsDelete() { return m_isDelete; };
	/// <summary>
	/// �폜�\��
	/// </summary>
	/// <returns></returns>
	void Delete() { m_isDelete = true; };
	/// <summary>
	/// ID���擾
	/// </summary>
	/// <returns></returns>
	int GetID() const { return m_id; };
	/// <summary>
	/// ID���Z�b�g
	/// </summary>
	/// <returns></returns>
	void SetID(int id);
	/// <summary>
	/// �`��p�N���X
	/// </summary>
	/// <returns></returns>
	bool IsDrawOnly() { return m_isDrawOnly; };
};

