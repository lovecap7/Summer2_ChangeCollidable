#pragma once
#include "../CharacterBase.h"
#include <memory>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape);
    virtual ~EnemyBase() {};
	//�U���̃N�[���^�C�����X�V
	virtual void UpdateAttackCoolTime();
	//�U���̃N�[���^�C�����擾
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//�U���̃N�[���^�C�����Z�b�g
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
protected:
	//�U���ł���܂ł̃N�[���^�C��
	int m_attackCoolTime;
};

