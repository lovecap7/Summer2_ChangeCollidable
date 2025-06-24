#include "StageObjectDraw.h"
#include "../../../General/Collision/PolygonCollider.h"
#include "../../../General/Rigidbody.h"
#include "../../../General/Collidable.h"
#include "../../../General/Model.h"
#include "../ActorManager.h"

StageObjectDraw::StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle)
{
	//描画用のクラス
	m_isDrawOnly = true;
	//モデル
	m_model = std::make_shared<Model>(modelHandle, pos);
	m_model->SetScale(scale);
	m_model->SetRot(angle);
}


StageObjectDraw::~StageObjectDraw()
{
	//なし
}

void StageObjectDraw::Entry(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャーに登録
	actorManager->Entry(shared_from_this());
}

void StageObjectDraw::Exit(std::shared_ptr<ActorManager> actorManager)
{
	//アクターマネージャー解除
	actorManager->Exit(shared_from_this());
}

void StageObjectDraw::Init()
{
	//なし
}

void StageObjectDraw::Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager)
{
	//なし
}

void StageObjectDraw::Gravity(const Vector3& gravity)
{
	//なし
}

void StageObjectDraw::OnHitColl(const std::shared_ptr<Collidable>& other)
{
	//なし
}

void StageObjectDraw::Draw() const
{
	m_model->Draw();
}

void StageObjectDraw::Complete()
{
	//なし
}
