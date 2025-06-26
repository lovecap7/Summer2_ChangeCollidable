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
    //�v���C���[�ւ̌���
    Vector3 GetPlayerVec(const std::weak_ptr<Player> player) const;
    Vector3 GetPlayerNomVecXZ(const std::weak_ptr<Player> player) const;
    //�v���C���[�Ƃ̋������v�Z
    float GetDistanceToPlayer(const std::weak_ptr<Player> player) const;
};

