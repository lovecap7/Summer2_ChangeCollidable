#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../../../General/Model.h"
#include "../../../General/Collision/Collidable.h"
#include "../../../General/Rigidbody.h"

EnemyBase::EnemyBase(Shape shape) :
	Actor(shape)
{
}

Vector3 EnemyBase::GetPlayerVec(const std::weak_ptr<Player> player) const
{
	return (GetPos() - player.lock()->GetPos());
}

Vector3 EnemyBase::GetPlayerNomVecXZ(const std::weak_ptr<Player> player) const
{
	Vector3 vecXZ = GetPlayerVec(player);
	vecXZ.y = 0.0f; // Y�������[���ɂ���XZ���ʂ̃x�N�g���ɂ���
	if (vecXZ.SqMagnitude() > 0.0f)
	{
		vecXZ = vecXZ.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
	}
	return vecXZ;
}

float EnemyBase::GetDistanceToPlayer(const std::weak_ptr<Player> player) const
{
	return GetPlayerVecXZ(player).Magnitude();
}
