#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
#include <DxLib.h>

//モデルのポリゴンが当たり判定
class PolygonCollider :
    public ColliderBase
{
public:
	PolygonCollider(int modelHandle);
	MV1_COLL_RESULT_POLY_DIM GetHitDim()const { return m_hitDim; }
	void SetHitDim(MV1_COLL_RESULT_POLY_DIM& dim){ m_hitDim = dim; }
    //壁と床の近い座標
	Vector3 GetNearWallHitPos() const { return m_nearWallHitPos; }
	Vector3 GetNearFloorHitPos() const { return m_nearFloorHitPos; }
    //モデル
	int GetModelHandle() const{ return m_modelHandle; }
    //床と壁のフラグ
    void ResetHitFlag() { m_isFloor = m_isWall = false; };
    bool IsFloor() { return m_isFloor; };
    bool IsWall() { return m_isWall; };
    void SetIsFloor(bool isFloor) { m_isFloor = isFloor; };
    void SetIsWall(bool isWall) { m_isWall = isWall; };
private:
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

