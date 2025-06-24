#pragma once
#include "ColliderBase.h"
#include "../Math/MyMath.h"
class CapsuleCollider :
    public ColliderBase
{
public:
    CapsuleCollider(Position3 endPos,float radius);
    virtual ~CapsuleCollider() {};
    //�����̍��W��m_endPos��2�_�ō\�����ꂽ�J�v�Z��
    Position3 m_endPos;
    //2�_�̋��̔��a
    float m_radius;
    //�����������̂Ƌ߂����W
    Vector3 m_hitNearPos;
    //�ŒZ����
    float m_shortDis;
};

