#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
	SphereCollider();
	virtual ~SphereCollider();
	float GetRadius() const { return m_radius; }
	void SetRadius(float radius) { m_radius = radius; }
    //���a
    float m_radius;
};

