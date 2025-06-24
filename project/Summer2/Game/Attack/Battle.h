#pragma once
namespace Battle
{
	//�U���̏d��
	enum class AttackWeight : int
	{
		Light		= 0,
		Middle		= 1,
		Heavy		= 2,
		Heaviest	= 3,
	};
	//�U���łЂ�܂Ȃ�����
	typedef AttackWeight Armor;

	//�U���ƃA�[�}�[�̔�r
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
