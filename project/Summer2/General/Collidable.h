#pragma once
#include <memory>
#include "Math/MyMath.h"
#include "Collision/ColliderBase.h"


//��Ԃɍ��킹�ē����蔻����s�����Ɏg��
enum class CollisionState
{
	Normal,
	Jump,
	Fall,
	Dead
};

// �ʒu�␳�̗D��x�̔��ʂɎg��
enum class Priority : int
{
	Low,		// ��
	Middle,		// ��
	High,		// ��
	Static,		// �����Ȃ��i�ō��j
};

// �Q�[�����̃^�O�t��
enum class GameTag
{
	None,
	Player,		//�v���C���[
	Enemy,		//�G
	Item,		//�A�C�e��
	Object,		//��Q��
	Field,		//�t�B�[���h
};

//���C���[
enum class Layer
{
	Actor,		//�A�N�^�[
	Effect,		//�G�t�F�N�g
	Attack,		//�U��
	HurtPoint,	//���ꔻ��
};


class ColliderBase;
class Actor;
class Rigidbody;
class Physics;
class CollisionChecker;
class CollisionProcess;
class Collidable abstract : 
	public std::enable_shared_from_this<Collidable>
{
public:
	Collidable(Shape shape);
	virtual ~Collidable() {};

	/// <summary>
	/// �Փ˂����Ƃ��̃C�x���g�֐�
	/// </summary>
	/// <param name="other"></param>
	virtual void OnCollide(const std::shared_ptr<Collidable> other)abstract;
	/// <summary>
	/// Update�̏����ɂ���ċN�������Փˏ����Ȃǂ̏����̊m��
	/// </summary>
	virtual void Complete() abstract;
	
	/// <summary>
	/// Physics�ɓo�^
	/// </summary>
	virtual void Init();
	/// <summary>
	/// Physics�������
	/// </summary>
	virtual void End();
	//�^�O
	GameTag GetGameTag() { return m_tag; };
	void SetGameTag(GameTag gameTag) { m_tag = gameTag; };
	//�R���C�_�[�̌`
	Shape GetShape() const { return m_collisionData->m_shape; };

	//�n�ʂɓ����������ǂ���
	bool IsFloor() { return m_isFloor; };
	bool IsWall() { return m_isWall; };
	void SetIsFloor(bool isFloor) { m_isFloor = isFloor; };
	void SetIsWall(bool isWall) { m_isWall = isWall; };
	//���ƕǂ̃t���O
	void ResetHitFlag() { m_isFloor = m_isWall = false; };
private:
	void CreateCollider(Shape shape);
	//Physics��Collidable�����R�ɊǗ��ł���悤��
	friend Physics;
	friend CollisionChecker;
	friend CollisionProcess;
protected:
	//�����蔻��
	std::shared_ptr<ColliderBase> m_collisionData;
	//���W�ƃx�N�g��
	std::shared_ptr<Rigidbody> m_rb;
	//�Փ˔�����s��Ȃ�(true�Ȃ�)
	bool m_isThrough;
	//�g���K�[
	bool m_isTrigger;
	//���
	CollisionState m_collState;
	//�D��x
	Priority m_priority;
	//�^�O
	GameTag m_tag;
	//���ɓ��������Ȃ�true
	bool m_isFloor;
	//�ǂɓ��������Ȃ�true
	bool m_isWall;
};



