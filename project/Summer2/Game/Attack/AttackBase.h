#pragma once
#include <memory>
#include <vector>
#include "Battle.h"
#include "../../General/Math/Vector3.h"

class Collidable;
class Actor;
class AttackBase abstract
{
public:
	AttackBase(std::shared_ptr<Collidable> coll, int damege, int keepFrame,float knockBackPower, std::shared_ptr<Actor> owner);
	virtual ~AttackBase() {};
	//������
	virtual void Init() abstract;
	//�X�V����
	virtual void Update() abstract;
	//�`��
	virtual void Draw() abstract;
	//����������
	virtual void OnHit(std::shared_ptr<Actor> actor) abstract;
	//�R���C�_�[
	std::shared_ptr<Collidable> GetCollidable()const { return m_collidable; };
	//����
	bool IsDelete() { return m_isDelete; };
	//�U�����������
	void Delete() { m_isDelete = true; };
	//�_���[�W
	int GetDamage() { return m_damage; };
	void SetDamage(int damage);
	//�����t���[��
	void SetKeepFrame(int keepFrame);
	//�m�b�N�o�b�N
	float GetKnockBackPower() { return m_knockBackPower; };
	virtual Vector3 GetKnockBackVec(Vector3 other);
	//�U���̋���
	Battle::AttackWeight GetAttackPower() { return m_attackPower; };
	void SetAttackPower(Battle::AttackWeight ap) { m_attackPower = ap; };
protected:
	//�����蔻�����W���������N���X
	std::shared_ptr<Collidable> m_collidable;
	//�_���[�W
	int m_damage;
	//�����t���[��
	int m_keepFrame;
	//�m�b�N�o�b�N�̑傫��
	float m_knockBackPower;
	//���Ńt���O
	bool m_isDelete;
	//���Ă����Ƃ̂���Collidable���o���Ă���
	std::vector<int> m_hitId;
	//������
	std::shared_ptr<Actor> m_owner;
	//�������p
	int m_initDamage;//�������p
	int m_initKeepFrame;//�������p
	//�U���̋���
	Battle::AttackWeight m_attackPower;
};

