#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
	SphereCollider();
	virtual ~SphereCollider() {};
    //���a
    float m_radius;
};

