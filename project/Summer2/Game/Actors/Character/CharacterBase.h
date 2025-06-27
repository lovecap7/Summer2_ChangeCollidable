#pragma once
#include "../Actor.h"
#include <memory>
class ActorStateBase;
class HitPoints;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
    //体力
    std::shared_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
protected:
    //キャラクターの状態
    std::shared_ptr<ActorStateBase> m_state;
    //体力
    std::shared_ptr<HitPoints> m_hitPoints;
};

