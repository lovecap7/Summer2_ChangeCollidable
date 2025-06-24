#pragma once
#include <memory>
#include "Math/MyMath.h"
#include "Collision/ColliderBase.h"

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
class Collidable abstract
{
public:
	Collidable(State state, Priority priority, GameTag gameTag, Shape shape, bool isTrigger);
	virtual ~Collidable();
private:
	//�R���C�_�[�̍쐬
	std::shared_ptr<ColliderBase> CreateCollider(Shape shape);
protected:
	//�����蔻��
	std::shared_ptr<ColliderBase> m_coll;
	//���W�ƃx�N�g��
	std::shared_ptr<Rigidbody> m_rb;
	//�g���K�[
	bool m_isTrigger;
	//���
	State m_state;
	//�D��x
	Priority m_priority;
	//�^�O
	GameTag m_tag;
	//�Փ˔���𖳎����邩
	bool m_isThrough;
};



