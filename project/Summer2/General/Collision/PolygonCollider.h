#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
#include <DxLib.h>

//モデルのポリゴンが当たり判定
class PolygonCollider :
    public ColliderBase
{
public:
	PolygonCollider();
	virtual ~PolygonCollider() {};
    //当たり判定をするモデル
    int m_modelHandle;
    //当たったポリゴンの情報
    MV1_COLL_RESULT_POLY_DIM m_hitDim;
    //当たった際の最も近い壁ポリゴンの座標
    Vector3 m_nearWallHitPos;
    //当たった際の最も近い床ポリゴンの座標
    Vector3 m_nearFloorHitPos;
    //床に当たったならtrue
    bool m_isFloor;
    //壁に当たったならtrue
    bool m_isWall;
};

