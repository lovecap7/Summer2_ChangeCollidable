#pragma once
#include "Battle.h"
//�̗͂�_���[�W���󂯂����̏����Ȃǂ��s���N���X
class HitPoints
{
public:
	HitPoints(int hp, Battle::Armor armor);
	~HitPoints();
	//����������
	void Init();
	//�̗�
	int GetHp() { return m_hp; };
	//��
	void Heal(int heal);
	//�_���[�W
	void Damage(int damage);
	//�̗͂��Ȃ��Ȃ��Ď��S�������ǂ���
	bool IsDead() { m_hp <= 0.0f; };
	//�A�[�}�[
	Battle::Armor GetArmor() { return m_armor; };
	void SetArmor(Battle::Armor am) { m_armor = am; };
	//�U�����󂯂���
	bool IsHit() { return m_isHit; };
	//�q�b�g���A�N�V���������邩
	bool IsHitReaction() const { return m_isHitReaction; };
	//�_���[�W�J�b�g
	void SetDamageCutRate(float damageCutRate) { m_damageCutRate = damageCutRate; };
	//���G
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
private:
	//�̗�
	int m_hp;
	//�ő�̗�
	int m_maxHp;
	//�U�����󂯂�
	bool m_isHit;
	//�U�����󂯂����ǂ���
	bool m_isHitReaction;
	//�A�[�}�[
	Battle::Armor m_armor;
	//�_���[�W�J�b�g��
	float m_damageCutRate;
	//���G
	bool m_isNoDamage;
};

