#include "EnemyBase.h"
#include "../Player/Player.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Rigidbody.h"

EnemyBase::EnemyBase(Shape shape) :
	CharacterBase(shape)
{
}

Vector3 EnemyBase::GetToPlayerVec(const std::weak_ptr<Player> player) const
{
	return (player.lock()->GetPos() - GetPos());
}

Vector3 EnemyBase::GetToPlayerNomVecXZ(const std::weak_ptr<Player> player) const
{
	Vector3 vecXZ = GetToPlayerVec(player);
	vecXZ.y = 0.0f; // Y�������[���ɂ���XZ���ʂ̃x�N�g���ɂ���
	if (vecXZ.SqMagnitude() > 0.0f)
	{
		vecXZ = vecXZ.Normalize(); // ���K�����ĕ����x�N�g���ɂ���
	}
	return vecXZ;
}

float EnemyBase::GetDistanceToPlayer(const std::weak_ptr<Player> player) const
{
	return GetToPlayerVec(player).Magnitude();
}

void EnemyBase::LookAtPlayer(const std::weak_ptr<Player> player)
{
	//���f���̌������v���C���[�Ɍ�����
	m_model->SetDir(GetToPlayerNomVecXZ(player).XZ());
}


