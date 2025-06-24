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

//攻撃判定を出す
void EnemyBase::AppearAttack(const std::shared_ptr<AttackBase>& attack, const std::unique_ptr<AttackManager> attackManager)
{
	//攻撃を入れる
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
	playerVec.y = 0.0f; //y成分を0にしてXZ平面上のベクトルにする
	if (playerVec.Magnitude() > 0.0f)
	{
		playerVec = playerVec.Normalize(); //正規化
	}
	else
	{
		playerVec = Vector3(0.0f, 0.0f, 0.0f); //ゼロベクトルの場合はデフォルトの方向を返す
	}
	return playerVec;
}
