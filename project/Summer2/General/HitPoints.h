#pragma once
#include "Battle.h"
//体力やダメージを受けた時の処理などを行うクラス
class HitPoints
{
public:
	HitPoints(int hp, Battle::Armor armor);
	~HitPoints();
	//初期化処理
	void Init();
	//体力
	int GetHp() { return m_hp; };
	//回復
	void Heal(int heal);
	//ダメージ
	void Damage(int damage);
	//体力がなくなって死亡したかどうか
	bool IsDead() { m_hp <= 0.0f; };
	//アーマー
	Battle::Armor GetArmor() { return m_armor; };
	void SetArmor(Battle::Armor am) { m_armor = am; };
	//攻撃を受けたか
	bool IsHit() { return m_isHit; };
	//ヒットリアクションをするか
	bool IsHitReaction() const { return m_isHitReaction; };
	//ダメージカット
	void SetDamageCutRate(float damageCutRate) { m_damageCutRate = damageCutRate; };
	//無敵
	bool IsNoDamege() { return m_isNoDamage; };
	void SetIsNoDamege(bool isNoDamage) { m_isNoDamage = isNoDamage; };
private:
	//体力
	int m_hp;
	//最大体力
	int m_maxHp;
	//攻撃を受けた
	bool m_isHit;
	//攻撃を受けたかどうか
	bool m_isHitReaction;
	//アーマー
	Battle::Armor m_armor;
	//ダメージカット率
	float m_damageCutRate;
	//無敵
	bool m_isNoDamage;
};

