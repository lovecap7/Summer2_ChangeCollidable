#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
#include "../../General/Collision/Collidable.h"

class ActorManager;
class Camera;
class Collidable;
class Model;
/// <summary>
/// �Q�[�����ɔz�u�\�ȕ��̂̊��N���X
/// </summary>
/// //new�ł��Ȃ��Ȃ�
class Actor abstract:
	public Collidable
{
protected:
	//����
	bool m_isDelete;
	//�A�N�^�[�̎��ʔԍ�
	int m_id;
	//ID���Z�b�g���ꂽ���Ƃ��L�^����t���O
	bool m_isSetId;
	//���f��
	std::shared_ptr<Model> m_model;
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
	virtual void Update(const std::weak_ptr<Camera> camera,const std::weak_ptr<ActorManager> actorManager) abstract;
	/// <summary>
	/// �Ώۂ̃A�N�^�[��\��
	/// </summary>
	virtual void Draw()const abstract;
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
	/// ���W
	/// </summary>
	Vector3 GetPos() const;
	//���f���N���X
	std::shared_ptr<Model> GetModel() const { return m_model; };
};

