#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
#include "../../General/Collidable.h"

class ActorManager;
class Input;
class Camera;
class Collidable;
class HurtPoint;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
/// //new�ł��Ȃ��Ȃ�
class Actor abstract:
	public Collidable
{
protected:
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
	Actor(Shape shape);
	virtual ~Actor() {};
	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init()abstract;
	/// <summary>
	/// �ʒu�Ȃǂ̍X�V���s��
	/// </summary>
	/// <param name="">���̓I�u�W�F�N�g</param>
	virtual void Update(const std::weak_ptr<Camera> camera) abstract;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\��
	/// </summary>
	virtual void Draw()const abstract;
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

