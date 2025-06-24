#pragma once
#include "ColliderBase.h"
class SphereCollider :
    public ColliderBase
{
public:
	SphereCollider();
	virtual ~SphereCollider() {};
    //”¼Œa
    float m_radius;
};

