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

	/// <summary>
	/// UŒ‚‚ÆƒA[ƒ}[‚Ì”äŠr
	/// UŒ‚‚Ì‚Ù‚¤‚ª‘å‚«‚¢‚È‚çtrue
	/// </summary>
	/// <param name="aw"></param>
	/// <param name="am"></param>
	/// <returns></returns>
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
