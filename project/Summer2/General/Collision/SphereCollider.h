#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
	SphereCollider(float radius);
	float GetRadius() const { return m_radius; }
	void SetRadius(float radius) { m_radius = radius; }
private:
    //���a
    float m_radius;
};

