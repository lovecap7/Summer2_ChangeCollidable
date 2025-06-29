#pragma once
#include "Battle.h"
class AttackPoints
{
public:
	AttackPoints();
	~AttackPoints();
	void Update();
	//�_���[�W�{��
	void SetDamageRate(float rate) { m_attack.damageRate = m_initAttack.damageRate = rate;};
	float GetDamageRate() {return m_attack.damageRate; };
	//�U���̏d���̍Œ�l
	void SetLowestAW(Battle::AttackWeight aw) { m_attack.lowestAttackWeight = m_initAttack.lowestAttackWeight = aw; };
	Battle::AttackWeight GetLowestAW() { return	m_attack.lowestAttackWeight; };
	//�U���̃o�t���󂯂���
	void AttackBuff(float damageRate, Battle::AttackWeight lowestAttackWeight, int buffFrame);
private:
	struct Attack
	{
		//�_���[�W�{��
		float damageRate;
		//�U���̏d���̍Œ�l
		Battle::AttackWeight lowestAttackWeight;
	};
	Attack m_attack;
	Attack m_initAttack;
	//�o�t�̌��ʎ���
	int m_buffCountFrame;
};

