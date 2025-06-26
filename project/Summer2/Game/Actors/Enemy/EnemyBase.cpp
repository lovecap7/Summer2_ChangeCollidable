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
	vecXZ.y = 0.0f; // Y成分をゼロにしてXZ平面のベクトルにする
	if (vecXZ.SqMagnitude() > 0.0f)
	{
		vecXZ = vecXZ.Normalize(); // 正規化して方向ベクトルにする
	}
	return vecXZ;
}

float EnemyBase::GetDistanceToPlayer(const std::weak_ptr<Player> player) const
{
	return GetPlayerVecXZ(player).Magnitude();
}
