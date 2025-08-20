#pragma once
#include "../Actor.h"
#include <memory>
#include <map>
#include <string>
class CharacterStateBase;
class HitPoints;
class AttackPoints;
class SE;
class Voice;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
    //体力のステータス
    std::weak_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
	//攻撃のステータス
    std::weak_ptr<AttackPoints> GetAttackPoints() const { return m_attackPoints; };
    //攻撃を受けたときの処理
    virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
	//サウンド
	std::weak_ptr<SE>  CharacterOnceSE(std::string name);
	std::weak_ptr<SE>  CharacterLoopSE(std::string name);
	std::weak_ptr<Voice>  CharacterVC(std::string name);
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
    //体力
    std::shared_ptr<HitPoints> m_hitPoints;
	//攻撃のステータス
	std::shared_ptr<AttackPoints> m_attackPoints;
	//サウンド
	std::map<std::string, int> m_soundHandles;
	//サウンド初期化
	virtual void InitSound() abstract;
	//サウンド終了処理
	virtual void EndSound();
};

