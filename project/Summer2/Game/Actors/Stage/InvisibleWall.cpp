#include "InvisibleWall.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../ActorManager.h"

InvisibleWall::InvisibleWall(int modelHandle, Vector3 pos, VECTOR scale, VECTOR angle):
	m_modelHandle(modelHandle)
{
	//初期位置
	m_collidable = std::make_shared<Collidable>(std::make_shared<PolygonCollider>(modelHandle), std::make_shared<Rigidbody>(pos));
	//優先度
	//コライダブルの初期化
	m_collidable->Init(State::None, Priority::Static, GameTag::Object);
	DxLib::MV1SetScale(m_modelHandle, scale);
	DxLib::MV1SetRotationXYZ(m_modelHandle, angle);
}

InvisibleWall::~InvisibleWall()
{
}

void InvisibleWall::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void InvisibleWall::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
}

void InvisibleWall::Init()
{
	//コライダーに自分のポインタを持たせる
	m_collidable->SetOwner(shared_from_this());
}

void InvisibleWall::Update(const Input& input, const std::unique_ptr<Camera>& camera,const std::shared_ptr<ActorManager> actorManager)
{
	DxLib::MV1SetPosition(m_modelHandle, m_collidable->GetRb()->GetPos().ToDxLibVector());
}

void InvisibleWall::Gravity(const Vector3& gravity)
{
}

void InvisibleWall::OnHitColl(const std::shared_ptr<Collidable>& other)
{
}

void InvisibleWall::Draw() const
{
#if _DEBUG
	//DxLib::MV1DrawModel(m_modelHandle);
#endif
}

void InvisibleWall::Complete()
{
}
