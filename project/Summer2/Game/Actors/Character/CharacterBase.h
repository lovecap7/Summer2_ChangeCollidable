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
    //�̗�
    std::shared_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
protected:
    //�L�����N�^�[�̏��
    std::shared_ptr<ActorStateBase> m_state;
    //�̗�
    std::shared_ptr<HitPoints> m_hitPoints;
};

