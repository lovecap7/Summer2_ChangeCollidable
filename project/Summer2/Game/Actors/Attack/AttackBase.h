#pragma once
#include "../Actor.h"
#include "../../General/Battle.h"
class ActorStateBase;
class AttackBase abstract:
    public Actor 
{
public:
	AttackBase(Shape shape,int damage,int keepFrame,float knockBackPower, Battle::AttackWeight aw, std::weak_ptr<ActorStateBase> ownerState);
	virtual ~AttackBase();
	//ノックバック
	virtual Vector3 GetKnockBackVec(Vector3 other) abstract;
	//ダメージ
	int GetDamage() { return m_damage; };
	void SetDamage(int damage) { m_damage = damage; };
	//持続フレーム
	void SetKeepFrame(int keepFrame) { m_keepFrame = keepFrame; };
	//ノックバックの大きさ
	float GetKnockBackPower() { return m_knockBackPower; };
	void SetKnockBackPower(float knockBackPower) { m_knockBackPower = knockBackPower; };
	//攻撃の強さ
	Battle::AttackWeight GetAttackPower() { return m_attackWeight; };
	void SetAttackPower(Battle::AttackWeight ap) { m_attackWeight = ap; };
protected:
	//ダメージ
	int m_damage;
	//持続フレーム
	int m_keepFrame;
	//ノックバックの大きさ
	float m_knockBackPower;
	//攻撃の強さ
	Battle::AttackWeight m_attackWeight;
	//持ち主のステート
	std::weak_ptr<ActorStateBase> m_ownerState;
};

