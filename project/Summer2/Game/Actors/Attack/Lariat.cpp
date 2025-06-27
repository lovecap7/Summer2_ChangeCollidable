#include "Lariat.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
Lariat::Lariat(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius) :
	CapsuleAttackBase(ownerState, modelHandle, startIndex, endIndex, radius)
{
}

Lariat::~Lariat()
{
}

void Lariat::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Attack, false, true);
	//Physicsに登録
	Collidable::Init();
}

void Lariat::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	if (m_ownerState.expired())
	{
		m_isDelete = true; //所有者のステートが無くなったら削除フラグを立てる
		return; //所有者のステートが無くなったら何もしない
	}
	//モデルのインデックスから位置更新
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	//始点と終点の座標
	VECTOR startPos = MV1GetFramePosition(m_modelHandle, m_startIndex);
	VECTOR endPos = MV1GetFramePosition(m_modelHandle, m_endIndex);
	//更新
	m_rb->SetPos(startPos);
	cap->SetEndPos(Vector3(endPos));
}

void Lariat::Draw() const
{
}
