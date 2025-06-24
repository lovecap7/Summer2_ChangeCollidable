#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
#include <DxLib.h>

//���f���̃|���S���������蔻��
class PolygonCollider :
    public ColliderBase
{
public:
	PolygonCollider(int modelHandle);
	MV1_COLL_RESULT_POLY_DIM GetHitDim()const { return m_hitDim; }
	void SetHitDim(MV1_COLL_RESULT_POLY_DIM& dim){ m_hitDim = dim; }
    //�ǂƏ��̋߂����W
	Vector3 GetNearWallHitPos() const { return m_nearWallHitPos; }
	Vector3 GetNearFloorHitPos() const { return m_nearFloorHitPos; }
    //���f��
	int GetModelHandle() const{ return m_modelHandle; }
    //���ƕǂ̃t���O
    void ResetHitFlag() { m_isFloor = m_isWall = false; };
    bool IsFloor() { return m_isFloor; };
    bool IsWall() { return m_isWall; };
    void SetIsFloor(bool isFloor) { m_isFloor = isFloor; };
    void SetIsWall(bool isWall) { m_isWall = isWall; };
private:
    //�����蔻������郂�f��
    int m_modelHandle;
    //���������|���S���̏��
    MV1_COLL_RESULT_POLY_DIM m_hitDim;
    //���������ۂ̍ł��߂��ǃ|���S���̍��W
    Vector3 m_nearWallHitPos;
    //���������ۂ̍ł��߂����|���S���̍��W
    Vector3 m_nearFloorHitPos;
    //���ɓ��������Ȃ�true
    bool m_isFloor;
    //�ǂɓ��������Ȃ�true
    bool m_isWall;
};

