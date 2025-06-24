#include "EnemyBase.h"
#include "../../Attack/AttackBase.h"
#include "../../Attack/AttackManager.h"
#include "../../../General/Model.h"
#include "../../../General/Collidable.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collision/SearchTrigger.h"
EnemyBase::EnemyBase()
{
}

//�U��������o��
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager> attackManager)
{
	//�U��������
	attack->Init();
	attackManager->Entry(attack);
}

Vector3 EnemyBase::GetPlayerVec() const
{
	Vector3 playerVec = m_searchTrigger->GetTargetPos() - m_collidable->GetRb()->GetPos();
	return playerVec;
}

Vector3 EnemyBase::GetPlayerNomVecXZ() const
{
	Vector3 playerVec = m_searchTrigger->GetTargetPos() - m_collidable->GetRb()->GetPos();
	playerVec.y = 0.0f; //y������0�ɂ���XZ���ʏ�̃x�N�g���ɂ���
	if (playerVec.Magnitude() > 0.0f)
	{
		playerVec = playerVec.Normalize(); //���K��
	}
	else
	{
		playerVec = Vector3(0.0f, 0.0f, 0.0f); //�[���x�N�g���̏ꍇ�̓f�t�H���g�̕�����Ԃ�
	}
	return playerVec;
}
