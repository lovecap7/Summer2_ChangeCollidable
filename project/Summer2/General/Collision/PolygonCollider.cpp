#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(int modelHandle):
	ColliderBase(Shape::Polygon),
	m_modelHandle(modelHandle),
	m_nearWallHitPos(),
	m_nearFloorHitPos(),
	m_isFloor(false),
	m_isWall(false)
{
}
