#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
#include <DxLib.h>

//���f���̃|���S���������蔻��
class PolygonCollider :
    public ColliderBase
{
public:
	PolygonCollider();
	virtual ~PolygonCollider();
	MV1_COLL_RESULT_POLY_DIM& GetHitDim() { return m_hitDim; }
	void SetHitDim(MV1_COLL_RESULT_POLY_DIM& dim){ m_hitDim = dim; }
    //�ǂƏ��̋߂����W
	Vector3 GetNearWallHitPos() const { return m_nearWallHitPos; }
	Vector3 GetNearFloorHitPos() const { return m_nearFloorHitPos; }
    //���f��
	int GetModelHandle() const{ return m_modelHandle; }
public:
    //�����蔻������郂�f��
    int m_modelHandle;
    //���������|���S���̏��
    MV1_COLL_RESULT_POLY_DIM m_hitDim;
    //���������ۂ̍ł��߂��ǃ|���S���̍��W
    Vector3 m_nearWallHitPos;
    //���������ۂ̍ł��߂����|���S���̍��W
    Vector3 m_nearFloorHitPos;
};

