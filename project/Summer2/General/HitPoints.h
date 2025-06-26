#pragma once
#include "Battle.h"
class HitPoints
{
private:
	//体力
	int m_hp;
	//攻撃を受けた
	bool m_isHit;
	//アーマー
	Battle::Armor GetArmor() { return m_armor; };
	void SetArmor(Battle::Armor am) { m_armor = am; };
};

