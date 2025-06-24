#include "Battle.h"

bool Battle::CheckFlinchAttackAndArmor(AttackWeight ap, Armor am)
{
	//攻撃の大きさがアーマーより大きいならtrue
	return (ap > am);
}
