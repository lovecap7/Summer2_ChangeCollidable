#pragma once
#include "Battle.h"
#include <memory>
//�̗͂�_���[�W���󂯂����̏����Ȃǂ��s���N���X
class AttackBase;
class HitPoints
{
public:
	HitPoints(int hp, Battle::Armor armor);
	~HitPoints();
	//�X�V����
	void Update();
	//����������
	void ResetHitFlags();
	//�̗�
	int GetHp() { return m_hp; };
	//��
	void Heal(int heal);
	//�_���[�W
	void Damage(int damage);
	//�̗͂��Ȃ��Ȃ��Ď��S�������ǂ���
	bool IsDead()const { return m_hp <= 0; };
	//�A�[�}�[
	Battle::Armor GetArmor() { return m_defence.armor; };
	void SetArmor(Battle::Armor am) { m_defence.armor = am; };
	//�U�����󂯂���
	bool IsHit() { return m_isHit; };
	void SetIsHit(bool isHit) { m_isHit = isHit; };
	//�q�b�g���A�N�V���������邩
	bool IsHitReaction() const { return m_isHitReaction; };
	void SetIsHitReaction(bool isHitReaction) { m_isHitReaction = isHitReaction; };
	//�_���[�W�J�b�g
	void SetDamageCutRate(float damageCutRate) { m_defence.damageCutRate = damageCutRate; };
	//���G
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
	//�A�C�e�����Ƃ������ɂ�Ԋ֐�
	void DefenseBuff(Battle::Armor armor, float damageCutRate, int buffFrame);
private:
	//�̗�
	int m_hp;
	//�ő�̗�
	int m_maxHp;
	//�U�����󂯂�
	bool m_isHit;
	//�U�����󂯂����ǂ���
	bool m_isHitReaction;
	//���G
	bool m_isNoDamage;

	//�h��ʂ̃X�e�[�^�X
	struct Defense
	{
		//�A�[�}�[
		Battle::Armor armor;
		//�_���[�W�J�b�g��
		float damageCutRate;
	};
	Defense m_defence;
	Defense m_initDefence;	//�o�t�I����̏������p�̒l
	//�_���[�W�J�b�g�A�C�e�����Ƃ����ۂɃJ�E���g����t���[��
	int m_buffCountFrame;
};

