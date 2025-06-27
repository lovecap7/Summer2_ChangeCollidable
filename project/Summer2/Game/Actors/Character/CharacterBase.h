#pragma once
#include "../Actor.h"
#include <memory>
class CharacterStateBase;
class HitPoints;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
    //体力
    std::shared_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
    //攻撃を受けたときの処理
    virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
    //体力
    std::shared_ptr<HitPoints> m_hitPoints;
};

