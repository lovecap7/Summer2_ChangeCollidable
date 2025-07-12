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
	//���Z�q�I�[�o�[���[�h
	Battle::AttackWeight operator+(const Battle::AttackWeight& left, const Battle::AttackWeight& right);
	Battle::AttackWeight operator+(const Battle::AttackWeight& left, const int right);

	/// <summary>
	/// �U���ƃA�[�}�[�̔�r
	/// �U���̂ق����傫���Ȃ�true
	/// </summary>
	/// <param name="aw"></param>
	/// <param name="am"></param>
	/// <returns></returns>
	bool CheckFlinchAttackAndArmor(AttackWeight aw, Armor am);
}
