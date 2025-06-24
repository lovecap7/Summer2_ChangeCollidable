#pragma once
namespace Battle
{
	//UŒ‚‚Ìd‚³
	enum class AttackWeight : int
	{
		Light		= 0,
		Middle		= 1,
		Heavy		= 2,
		Heaviest	= 3,
	};
	//UŒ‚‚Å‚Ğ‚é‚Ü‚È‚¢‹­‚³
	typedef AttackWeight Armor;

	//UŒ‚‚ÆƒA[ƒ}[‚Ì”äŠr
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
