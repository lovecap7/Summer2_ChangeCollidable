#pragma once
#include "../CharacterBase.h"
#include <memory>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape);
    virtual ~EnemyBase() {};
	//攻撃のクールタイムを更新
	virtual void UpdateAttackCoolTime();
	//攻撃のクールタイムを取得
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃のクールタイムをセット
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
protected:
	//攻撃できるまでのクールタイム
	int m_attackCoolTime;
};

