#pragma once
#include "../CharacterBase.h"
#include <memory>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
//敵の階級
enum class EnemyGrade
{
	Normal,   //雑魚敵
	Elite,    //中ボス
	Boss      //ボス
};
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape, EnemyGrade grade);
    virtual ~EnemyBase() {};
	//攻撃のクールタイムを更新
	virtual void UpdateAttackCoolTime();
	//攻撃のクールタイムを取得
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃のクールタイムをセット
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
	//敵の体力表示の座標
	Vector3 GetHPUIPos() { return m_hpUIPos; };
	//敵の階級
	EnemyGrade GetEnemyGrade()const { return m_enemyGrade; };
	//プレイヤーを発見できたか
	bool IsSearch()const { return m_targetData.isHitTarget; };
protected:
	//攻撃できるまでのクールタイム
	int m_attackCoolTime;
	//頭の座標
	Vector3 m_hpUIPos;
	//自分の階級
	EnemyGrade m_enemyGrade;
};

