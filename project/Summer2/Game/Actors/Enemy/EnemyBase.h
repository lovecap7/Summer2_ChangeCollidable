#pragma once
#include "../Actor.h"
#include <memory>
#include "../../../General/Model.h"
class Player;
class EnemyBase abstract :
    public Actor
{
public:
    EnemyBase(Shape shape);
    virtual ~EnemyBase() {};
    //���f���N���X
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //���f��
    std::shared_ptr<Model> m_model;
    //���ꔻ��̍X�V
    virtual void UpdateHurtPoint()abstract;
};

