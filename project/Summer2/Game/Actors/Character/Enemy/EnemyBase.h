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
    //�v���C���[�ւ̌���
    Vector3 GetToPlayerVec(const std::weak_ptr<Player> player) const;
    Vector3 GetToPlayerNomVecXZ(const std::weak_ptr<Player> player) const;
    //�v���C���[�Ƃ̋������v�Z
    float GetDistanceToPlayer(const std::weak_ptr<Player> player) const;
    //�v���C���[������
    void LookAtPlayer(const std::weak_ptr<Player> player);
};

