#include "HitPoints.h"
#include "../Game/Actors/Attack/AttackBase.h"
#include "Math/MyMath.h"

HitPoints::HitPoints(int hp, Battle::Armor armor) :
	m_hp(hp),
	m_maxHp(hp),
	m_isHit(false),
	m_isHitReaction(false),
	m_isNoDamage(false),
	m_buffCountFrame(0)
{
	m_defence.armor = armor;
	m_defence.damageCutRate = 1.0f;
	m_initDefence = m_defence;
}

HitPoints::~HitPoints()
{
}

void HitPoints::Update()
{
	if (m_buffCountFrame < 0)
	{
		//�o�t���؂ꂽ���̏���
		if (m_initDefence.armor != m_defence.armor ||
			m_initDefence.damageCutRate != m_defence.damageCutRate )
		{
			m_defence = m_initDefence;
		}
	}
	else
	{
		//�o�t�̃t���[��
		--m_buffCountFrame;
	}
	ResetHitFlags();
}

void HitPoints::ResetHitFlags()
{
	m_isHit = false;
	m_isHitReaction = false;
}

void HitPoints::Heal(int heal)
{
	if (m_hp >= m_maxHp)return;	//�ő�Ȃ�
	if (heal < 0)heal *= -1;
	m_hp += heal;
	m_hp = MathSub::ClampInt(m_hp, 0, m_maxHp);
}

void HitPoints::Damage(int damage)
{
	if (m_isNoDamage)return;	//���G�Ȃ�
	if (damage < 0)damage *= -1;
	m_hp -= static_cast<float>(damage) * m_defence.damageCutRate;
	if (m_hp <= 0)
	{
		m_hp = 0;
	}
}

void HitPoints::DefenseBuff(Battle::Armor armor, float damageCutRate, int buffFrame)
{
	m_defence.armor = armor;
	m_defence.damageCutRate = damageCutRate;
	m_buffCountFrame = buffFrame;
}
