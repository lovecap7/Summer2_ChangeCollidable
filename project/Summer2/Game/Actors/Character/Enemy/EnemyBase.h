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
    //プレイヤーへの向き
    Vector3 GetToPlayerVec(const std::weak_ptr<Player> player) const;
    Vector3 GetToPlayerNomVecXZ(const std::weak_ptr<Player> player) const;
    //プレイヤーとの距離を計算
    float GetDistanceToPlayer(const std::weak_ptr<Player> player) const;
    //プレイヤーを見る
    void LookAtPlayer(const std::weak_ptr<Player> player);
};

