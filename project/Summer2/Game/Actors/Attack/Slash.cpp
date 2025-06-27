#include "Slash.h"
#include "../../../General/Collision/CapsuleCollider.h"
#include "../../../General/Rigidbody.h"
Slash::Slash(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius, float swordLength):
	CapsuleAttackBase(ownerState, modelHandle,startIndex,endIndex,radius),
	m_swordLength(swordLength)
{
}

Slash::~Slash()
{
}

void Slash::Init()
{
	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::High, GameTag::Attack, false, true);
	//Physicsに登録
	Collidable::Init();
}

void Slash::Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager)
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
	//武器の矛先
	VECTOR swordDir = VNorm(VSub(startPos, startPos));
	swordDir = VScale(swordDir, m_swordLength);	//武器の長さ
	swordDir = VAdd(startPos, swordDir);		//持ち手の座標に加算して剣先の座標を出す
	//武器の持ち手をセット
	m_rb->SetPos(startPos);
	//武器の剣先をセット
	cap->SetEndPos(swordDir);

	//持続フレームを減らす
	m_keepFrame--;
	//持続フレームが0になったら削除
	if (m_keepFrame <= 0)
	{
		m_isDelete = true; //削除フラグを立てる
		return; //何もしない
	}
}

void Slash::Draw() const
{
}
