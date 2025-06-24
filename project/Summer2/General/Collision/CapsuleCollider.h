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
    Position3 GetEndPos() const { return m_endPos; };
    void SetEndPos(Position3 pos) { m_endPos = pos; };
    //���̍��W
    Position3 GetNextEndPos(Vector3 vec) const { return m_endPos + vec; };
    void SetNextEndPos(Vector3 addVec) { m_endPos += addVec; };
    //2�_�̋��̔��a
    float GetRadius() const { return m_radius; };
    void SetRadius(float radius) { m_radius = radius; };
    //�����������̂Ƌ߂����W
    Vector3 GetNearPos() const { return m_hitNearPos; };
    void SetNearPos(Position3 hitNearPos) { m_hitNearPos = hitNearPos; };
    //�ŒZ����
    float GetShortDis() { return m_shortDis; }
    void SetShortDis(float shortDis) { m_shortDis = shortDis; };
private:
    //�����̍��W��m_endPos��2�_�ō\�����ꂽ�J�v�Z��
    Position3 m_endPos;
    //2�_�̋��̔��a
    float m_radius;
    //�����������̂Ƌ߂����W
    Vector3 m_hitNearPos;
    //�ŒZ����
    float m_shortDis;
};

