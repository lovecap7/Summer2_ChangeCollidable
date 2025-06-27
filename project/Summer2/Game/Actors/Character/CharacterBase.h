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
    //�̗�
    std::shared_ptr<HitPoints> GetHitPoints() const { return m_hitPoints; };
    //�U�����󂯂��Ƃ��̏���
    virtual void OnHitFromAttack(const std::shared_ptr<Collidable> other);
protected:
    //�L�����N�^�[�̏��
    std::shared_ptr<CharacterStateBase> m_state;
    //�̗�
    std::shared_ptr<HitPoints> m_hitPoints;
};

