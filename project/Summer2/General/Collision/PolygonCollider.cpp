#include "PolygonCollider.h"

PolygonCollider::PolygonCollider():
	ColliderBase(Shape::Polygon),
	m_modelHandle(-1),
	m_nearWallHitPos(),
	m_nearFloorHitPos(),
	m_isFloor(false),
	m_isWall(false)
{
}
