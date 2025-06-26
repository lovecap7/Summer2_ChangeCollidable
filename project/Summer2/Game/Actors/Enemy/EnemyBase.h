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
    //モデルクラス
    std::shared_ptr<Model> GetModel() const { return m_model; };
protected:
    //モデル
    std::shared_ptr<Model> m_model;
    //やられ判定の更新
    virtual void UpdateHurtPoint()abstract;
};

