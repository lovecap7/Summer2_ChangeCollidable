#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
class CapsuleCollider :
    public ColliderBase
{
public:
    CapsuleCollider(Position3 endPos,float radius);
    virtual ~CapsuleCollider() {};
    //自分の座標とm_endPosの2点で構成されたカプセル
    Position3 m_endPos;
    //2点の球の半径
    float m_radius;
    //当たった物体と近い座標
    Vector3 m_hitNearPos;
    //最短距離
    float m_shortDis;
};

