#pragma once
#include <memory>
#include "Math/MyMath.h"

//��Ԃɍ��킹�ē����蔻����s�����Ɏg��
enum class State
{
	None,
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

enum class GameTag
{
	None,
	Player,		//�v���C���[
	Enemy,		//�G
	Item,		//�A�C�e��
	Object,	//��Q��
	Field,		//�t�B�[���h
};


class ColliderBase;
class Actor;
class Rigidbody;
class Collidable
{
public:
	Collidable(std::shared_ptr<ColliderBase> coll, std::shared_ptr<Rigidbody> rb);
	virtual ~Collidable() {};
	//����������
	void Init(State state, Priority priority, GameTag gameTag);
	//�����蔻��
	const std::shared_ptr<ColliderBase>& GetColl() const { return m_coll; }
	//���W�ƃx�N�g��
	const std::shared_ptr<Rigidbody>& GetRb()const { return m_rb; }
	//�����蔻����s�����ǂ���
	bool IsCollide() const { return m_isCollide; };
	void SetIsCollide(bool isCollide) { m_isCollide = isCollide; }
	//���
	State GetState() { return m_state; };
	void SetState(State state) { m_state = state; };
	//������
	std::shared_ptr<Actor> GetOwner() { return m_owner; };
	void SetOwner(std::shared_ptr<Actor> owner) { m_owner = owner; };
	//�D��x
	Priority GetPriority() { return m_priority; };
	void SetPriority(Priority priority) { m_priority = priority; };
	//�^�O
	GameTag GetGameTag() { return m_tag; };
	void SetGameTag(GameTag gameTag) { m_tag = gameTag; };
private:
	//�����蔻��
	std::shared_ptr<ColliderBase> m_coll;
	//���W�ƃx�N�g��
	std::shared_ptr<Rigidbody> m_rb;
	//�Փ˔�����s��
	bool m_isCollide;
	//���
	State m_state;
	//������
	std::shared_ptr<Actor> m_owner;
	//�D��x
	Priority m_priority;
	//�^�O
	GameTag m_tag;
};



