#pragma once
#include "Battle.h"
class HitPoints
{
private:
	//�̗�
	int m_hp;
	//�U�����󂯂�
	bool m_isHit;
	//�A�[�}�[
	Battle::Armor GetArmor() { return m_armor; };
	void SetArmor(Battle::Armor am) { m_armor = am; };
};

