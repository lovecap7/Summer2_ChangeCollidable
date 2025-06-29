#pragma once
#include "Battle.h"
class AttackPoints
{
public:
	AttackPoints();
	~AttackPoints();
	void Update();
	//ダメージ倍率
	void SetDamageRate(float rate) { m_attack.damageRate = m_initAttack.damageRate = rate;};
	float GetDamageRate() {return m_attack.damageRate; };
	//攻撃の重さの最低値
	void SetLowestAW(Battle::AttackWeight aw) { m_attack.lowestAttackWeight = m_initAttack.lowestAttackWeight = aw; };
	Battle::AttackWeight GetLowestAW() { return	m_attack.lowestAttackWeight; };
	//攻撃のバフを受けた時
	void AttackBuff(float damageRate, Battle::AttackWeight lowestAttackWeight, int buffFrame);
private:
	struct Attack
	{
		//ダメージ倍率
		float damageRate;
		//攻撃の重さの最低値
		Battle::AttackWeight lowestAttackWeight;
	};
	Attack m_attack;
	Attack m_initAttack;
	//バフの効果時間
	int m_buffCountFrame;
};

